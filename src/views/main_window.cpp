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

	// Look for rooms.
	QNetworkRequest req{ QUrl{ "http://localhost:8080/api/rooms" } };
	auto *reply = m_network_manager->get(req);

	connect(reply, &QNetworkReply::finished,
		[this, reply] { handle_get_rooms(reply); });

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

	connect(m_middle_content, &MiddleContent::message_sent, this,
		[this](const QString &message) { send_message(message); });
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
	QJsonObject obj{ { "op", 2 } };
	QJsonObject d{
		{ "content", message },
	};
	obj.insert("d", d);

	QJsonDocument doc{ obj };

	qDebug() << "sending: " << doc;

	m_ws.sendTextMessage(doc.toJson());
}

void MainWindow::handle_gateway_event(const QJsonObject &json)
{
	qDebug() << "received JSON: " << json;

	const auto op = json.find("op");
	const auto content = json.find("content");

	if (op == json.end() || content == json.end() || !op->isDouble() ||
	    !content->isString()) {
		qDebug() << "Invalid JSON: " << json;
		return;
	}

	switch (op->toInt()) {
	case 2:
		m_middle_content->add_message(content->toString());
		break;
	default:
		qDebug() << "Unknown event: " << json;
	}
}

void MainWindow::handle_get_rooms(QNetworkReply *reply)
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

	const auto rooms = json.array();

	for (const auto &room : rooms) {
		const auto obj = room.toObject();
		const auto id = obj.value("id").toInt();
		const auto name = obj.value("name").toString();

		m_middle_contents.emplace(id, new MiddleContent{ name, this });

		if (m_middle_content == nullptr) {
			m_middle_content = m_middle_contents.at(id);
			// insert into layout into idx 2
			m_central_layout->insertWidget(2, m_middle_content);
		}
	}
}
} // namespace mozart