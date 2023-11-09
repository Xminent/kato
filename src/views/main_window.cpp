#include <QDebug>
#include <QFontDatabase>
#include <QJsonArray>
#include <QJsonDocument>
#include <mozart/api/win_dark.hpp>
#include <mozart/defer.hpp>
#include <mozart/views/main_window.hpp>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>

namespace
{
constexpr QSize WINDOW_SIZE{ 800, 600 };
constexpr QSize MIN_WINDOW_SIZE{ 940, 500 };

#ifdef Q_OS_WIN
mozart::MainWindow *main_window_instance{};
#endif
} // namespace

namespace mozart
{
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow{ parent }
{
	m_udp_socket->bind(QHostAddress::LocalHost);
	m_ws.open(QUrl{ "ws://localhost:8080/gateway" });

	// Look for channels.
	QNetworkRequest req{ QUrl{ "http://localhost:8080/api/channels" } };
	auto *reply = m_network_manager->get(req);

	connect(reply, &QNetworkReply::finished,
		[this, reply] { handle_get_channels(reply); });

	setup_signals();
	setup_audio();
	setup_ui();

#ifdef Q_OS_WIN
	main_window_instance = this;
#endif
}

MainWindow::~MainWindow() = default;

void MainWindow::setup_audio()
{
	QAudioEncoderSettings settings;
	settings.setCodec("audio/pcm");
	settings.setQuality(QMultimedia::HighQuality);

	m_audio_input->setEncodingSettings(settings);

	[[maybe_unused]] const auto worked =
		m_audio_probe->setSource(m_audio_input);

	Q_ASSERT(worked);
}

void MainWindow::setup_signals()
{
	connect(m_audio_probe, &QAudioProbe::audioBufferProbed, this,
		[this](QAudioBuffer buf) {
			// Send audio to the server.
			m_udp_socket->writeDatagram(
				reinterpret_cast<const char *>(buf.data()),
				buf.byteCount(), QHostAddress::LocalHost, 8080);
		});

	connect(m_udp_socket, &QUdpSocket::readyRead, this, [this] {
		while (m_udp_socket->hasPendingDatagrams()) {
			QByteArray datagram;
			datagram.resize(static_cast<int>(
				m_udp_socket->pendingDatagramSize()));
			m_udp_socket->readDatagram(datagram.data(),
						   datagram.size());

			// Play the audio.
		}
	});

	connect(m_navbar, &NavBar::home_clicked, this,
		[this] { qDebug() << "home clicked"; });

	connect(m_left_sidebar, &LeftSidebar::channel_selected, this,
		[this](const ChannelItem *channel) {
			set_middle_content(m_middle_contents[channel->id()]);
		});

	connect(&m_ws, &QWebSocket::connected, this, [this] {
		qDebug() << "connected";
		send_message("Hello world");
	});

	connect(&m_ws, &QWebSocket::disconnected, this, [this] {
		qDebug() << "disconnected";
		m_ws.open(QUrl{ "ws://localhost:8080" });
	});

	connect(&m_ws, &QWebSocket::textMessageReceived, this,
		[this](const QString &message) {
			const auto json =
				QJsonDocument::fromJson(message.toUtf8());

			if (json.isNull()) {
				qDebug() << "Failed to parse JSON: " << message;
				return;
			}

			if (json.isObject()) {
				handle_gateway_event(json.object());
			}
		});
}

void MainWindow::setup_ui()
{
	resize(WINDOW_SIZE);
	setMinimumSize(MIN_WINDOW_SIZE);
	winDark::setDark_Titlebar(reinterpret_cast<HWND>(winId()));
	setWindowTitle("Mozart");

	QPalette palette;
	QBrush background_brush{ QColor(0, 0, 0, 255) };
	QBrush base_brush{ QColor(42, 42, 42, 255) };
	QBrush placeholder_brush{ QColor(255, 255, 255, 128) };
	QBrush text_brush{ Qt::white };

	palette.setBrush(QPalette::Base, base_brush);
	palette.setBrush(QPalette::PlaceholderText, placeholder_brush);
	palette.setBrush(QPalette::WindowText, text_brush);
	palette.setBrush(QPalette::Window, background_brush);
	setPalette(palette);

	QFontDatabase::addApplicationFont(":/fonts/Inter.ttf");

	QFont font;
	font.setFamily("Inter");
	font.setPointSize(14);
	setFont(font);

	m_central_layout->setContentsMargins(0, 0, 0, 0);
	m_central_layout->setSpacing(0);
	m_central_layout->addWidget(m_navbar);
	m_central_layout->addWidget(m_left_sidebar);
	m_central_layout->addSpacerItem(new QSpacerItem(
		0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

	setCentralWidget(m_central_widget);
}

void MainWindow::send_message(const QString &message)
{
	QJsonObject data{
		{ "content", message },
	};
	QJsonDocument doc{ data };

	qDebug() << "Sending: " << doc.toJson(QJsonDocument::Compact);

	auto req = QNetworkRequest{ QUrl{
		"http://localhost:8080/api/channels/0/messages" } };

	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	auto *reply = m_network_manager->post(
		req, doc.toJson(QJsonDocument::Compact));

	connect(reply, &QNetworkReply::finished, reply, [reply] {
		if (reply->error() != QNetworkReply::NoError) {
			qDebug() << "Error: " << reply->errorString();
		}

		reply->deleteLater();
	});
}

void MainWindow::handle_gateway_event(const QJsonObject &json)
{
	qDebug() << "received JSON: " << json;

	const auto op_it = json.find("op");
	const auto d_it = json.find("d");

	if (op_it == json.end() || !op_it->isDouble() || d_it == json.end() ||
	    !d_it->isObject()) {
		qDebug() << "Invalid JSON: " << json;
		return;
	}

	const auto op = op_it->toInt();
	const auto d = d_it->toObject();

	switch (op) {
	case 2: {
		const auto content = d.find("content");

		if (content == d.end() || !content->isString()) {
			qDebug() << "Invalid JSON: " << d;
			return;
		}

		m_middle_content->add_message(content->toString());
		break;
	}
	case 3: {
		const auto id_it = d.find("id");
		const auto name_it = d.find("name");

		if (id_it == json.end() || name_it == json.end() ||
		    !id_it->isDouble() || !name_it->isString()) {
			qDebug() << "Invalid JSON: " << json;
			return;
		}

		const auto id = static_cast<uint64_t>(id_it->toDouble());
		const auto name = name_it->toString();

		add_middle_content(id, name);
		m_channels.emplace(id, name);
		m_left_sidebar->set_channels(m_channels);
		break;
	}
	default:
		qDebug() << "Unknown event: " << json;
	}
}

void MainWindow::handle_get_channels(QNetworkReply *reply)
{
	defer
	{
		reply->deleteLater();
	};

	if (reply->error() != QNetworkReply::NoError) {
		qDebug() << "Error: " << reply->errorString();
		return;
	}

	const auto res = reply->readAll();
	const auto json = QJsonDocument::fromJson(res);

	if (!json.isArray()) {
		return;
	}

	const auto channels = json.array();

	for (const auto &channel : channels) {
		const auto obj = channel.toObject();
		const auto id_it = obj.find("id");
		const auto name_it = obj.find("name");

		if (id_it == obj.end() || name_it == obj.end() ||
		    !id_it->isDouble() || !name_it->isString()) {
			continue;
		}

		const auto id = static_cast<uint64_t>(id_it->toDouble());
		const auto name = name_it->toString();

		add_middle_content(id, name);
		m_channels.emplace(id, name);

		// TODO: This should belong to some first-time setup.
		if (m_middle_content == nullptr) {
			set_middle_content(m_middle_contents.at(id));
		}
	}

	// Set the channels in the left sidebar.
	m_left_sidebar->set_channels(m_channels);
}

void MainWindow::add_middle_content(uint64_t id, const QString &name)
{
	if (m_middle_contents.find(id) != m_middle_contents.end()) {
		return;
	}

	auto *middle_content = new MiddleContent{ id, name, this };
	m_middle_contents.emplace(id, middle_content);

	connect(middle_content, &MiddleContent::message_sent, this,
		[this](const QString &message) { send_message(message); });
}

void MainWindow::set_middle_content(MiddleContent *content)
{
	if (content == m_middle_content) {
		return;
	}

	if (m_middle_content != nullptr) {
		m_central_layout->removeWidget(m_middle_content);
		m_middle_content->hide();
	}

	m_middle_content = content;

	if (m_middle_content != nullptr) {
		m_central_layout->insertWidget(2, m_middle_content);
		m_middle_content->show();
	}
}
} // namespace mozart