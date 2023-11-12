#include <QDebug>
#include <QFontDatabase>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QProcessEnvironment>
#include <QStackedWidget>
#include <kato/api/win_dark.hpp>
#include <kato/views/main_window.hpp>

namespace
{
constexpr QSize WINDOW_SIZE{ 800, 600 };
constexpr QSize MIN_WINDOW_SIZE{ 940, 500 };
constexpr uint16_t API_PORT{ 8080 };
constexpr uint16_t AUDIO_PORT{ 9090 };

#ifdef Q_OS_WIN
// NOLINTNEXTLINE
kato::MainWindow *main_window_instance{};
#endif

enum class GatewayOpcode : uint8_t {
	Ready = 1,
	MessageCreate = 2,
	ChannelCreate = 3,
};
} // namespace

namespace kato
{
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow{ parent }
{
	m_token = QProcessEnvironment::systemEnvironment().value("SHIKI_TOKEN");

	Q_ASSERT(!m_token.isEmpty());

	m_udp_socket->bind(QHostAddress::LocalHost);

	connect_to_gateway();
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
	settings.setSampleRate(48'000);
	settings.setChannelCount(2);
	settings.setQuality(QMultimedia::HighQuality);

	m_audio_recorder->setEncodingSettings(settings);
	m_buffer->open(QIODevice::ReadWrite);

	m_format.setSampleRate(48'000);
	m_format.setChannelCount(2);
	m_format.setSampleSize(16);
	m_format.setCodec("audio/pcm");
	m_format.setByteOrder(QAudioFormat::LittleEndian);
	m_format.setSampleType(QAudioFormat::UnSignedInt);


    if (QAudioDeviceInfo info{QAudioDeviceInfo::defaultOutputDevice()}; !info.isFormatSupported(m_format)) {
		qWarning()
			<< "Default format not supported, trying to use nearest.";
		m_format = info.nearestFormat(m_format);
    }

	m_audio_input = new QAudioInput(m_format, this);
	m_audio_output = new QAudioOutput(m_format, this);
	// m_audio_output->start();
	m_audio_probe->start(m_audio_input);
	m_audio_input->start(m_audio_probe);
	m_audio_output->start(m_buffer);
}

void MainWindow::setup_signals()
{
	// connect(m_audio_probe, &QAudioProbe::audioBufferProbed, this,
	// 	[this](QAudioBuffer buf) {
	// 		// Send audio to the server.
	// 		m_udp_socket->writeDatagram(
	// 			reinterpret_cast<const char *>(buf.data()),
	// 			buf.byteCount(), QHostAddress::LocalHost,
	// 			AUDIO_PORT);
	// 	});

	connect(m_audio_probe, &AudioProbeDevice::audio_available, this,
		[this](const QAudioBuffer &buf) {
			m_udp_socket->writeDatagram(
				reinterpret_cast<const char *>(buf.data()),
				buf.byteCount(), QHostAddress::LocalHost,
				AUDIO_PORT);
		});

	connect(m_udp_socket, &QUdpSocket::readyRead, this, [this] {
		while (m_udp_socket->hasPendingDatagrams()) {
			QByteArray datagram;
			datagram.resize(static_cast<int>(
				m_udp_socket->pendingDatagramSize()));
			m_udp_socket->readDatagram(datagram.data(),
						   datagram.size());

			if (datagram.isEmpty()) {
				continue;
			}

			QAudioBuffer buf{ datagram, m_format };

			qDebug() << "Received datagram of size: "
				 << datagram.size();

			m_buffer->write(
				reinterpret_cast<const char *>(buf.data()),
				buf.byteCount());
		}
	});

	connect(m_navbar, &NavBar::home_clicked, this,
		[this] { qDebug() << "home clicked"; });

	connect(m_left_sidebar, &LeftSidebar::channel_selected, this,
		[this](const ChannelItem *channel) {
			set_channel(channel->id());
		});

	connect(m_left_sidebar, &LeftSidebar::create_channel, this, [this] {
		static int count{};

		auto *label = new QLabel{ QString("Channel %1").arg(++count) };
		m_modal = std::make_unique<Modal>(
			QString("Create channel %1").arg(++count), label, this);
		m_modal->move_to_center();
		m_modal->show();
	});

	connect(&m_ws, &QWebSocket::connected, this, [this] {
		qDebug() << "connected";
		identify();
	});

	connect(&m_ws, &QWebSocket::disconnected, this, [this] {
		qDebug() << "disconnected: close code: " << m_ws.closeCode();
		connect_to_gateway();
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
	setWindowTitle("Kato");

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

	setFont(QFont{ "Inter", 10 });

	m_central_layout->setContentsMargins(0, 0, 0, 0);
	m_central_layout->setSpacing(0);
	m_central_layout->addWidget(m_navbar);
	m_central_layout->addWidget(m_left_sidebar);
	m_central_layout->addSpacerItem(new QSpacerItem(
		0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));

	setCentralWidget(m_central_widget);
}

void MainWindow::create_message(const QString &message)
{
	if (m_middle_content == nullptr) {
		return;
	}

	QJsonObject data{
		{ "content", message },
	};
	QJsonDocument doc{ data };

	auto url = QString{ "http://localhost:%1/api/channels/%2/messages" }
			   .arg(API_PORT)
			   .arg(m_middle_content->id());

	qDebug() << "Sending: " << doc.toJson(QJsonDocument::Compact) << " to "
		 << url;

	auto req = QNetworkRequest{ QUrl{ url } };

	req.setRawHeader("Authorization",
			 QString{ "Bearer %1" }.arg(m_token).toUtf8());
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

	switch (static_cast<GatewayOpcode>(op)) {
	case GatewayOpcode::Ready: {
		const auto user_it = d.find("user");
		const auto channels_it = d.find("channels");

		if (user_it == d.end() || !user_it->isObject() ||
		    channels_it == d.end() || !channels_it->isArray()) {
			qDebug() << "Invalid JSON: " << d;
			return;
		}

		const auto user = user_it->toObject();
		const auto username_it = user.find("username");

		if (username_it == user.end() || !username_it->isString()) {
			qDebug() << "Invalid JSON: " << d;
		}

		m_name = username_it->toString();
		qDebug() << "Setting name to: " << m_name;
		handle_get_channels(channels_it->toArray());
		create_message("Hello world");
		break;
	}
	case GatewayOpcode::MessageCreate: {
		const auto author_it = d.find("author");
		const auto content_it = d.find("content");

		if (author_it == d.end() || !author_it->isObject() ||
		    content_it == d.end() || !content_it->isString()) {
			qDebug() << "Invalid JSON: " << d;
			return;
		}

		const auto author = author_it->toObject();
		const auto author_name_it = author.find("username");
		const auto avatar_it = author.find("avatar");

		if (author_name_it == author.end() ||
		    !author_name_it->isString() || avatar_it == author.end()) {
			qDebug() << "Invalid JSON: " << d;
			return;
		}

		QString avatar;

		if (avatar_it->isString()) {
			avatar = avatar_it->toString();
		}

		m_middle_content->add_message(avatar,
					      author_name_it->toString(),
					      content_it->toString());
		break;
	}
	case GatewayOpcode::ChannelCreate: {
		const auto id_it = d.find("id");
		const auto name_it = d.find("name");

		if (id_it == json.end() || name_it == json.end() ||
		    !id_it->isDouble() || !name_it->isString()) {
			qDebug() << "Invalid JSON: " << json;
			return;
		}

		const auto id = static_cast<uint64_t>(id_it->toDouble());
		const auto name = name_it->toString();

		add_channel(id, name);
		m_channels.emplace(id, name);
		m_left_sidebar->set_channels(m_channels);
		break;
	}
	default:
		qDebug() << "Unknown event: " << json;
	}
}

void MainWindow::handle_get_channels(const QJsonArray &channels)
{
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

		add_channel(id, name);
		m_channels.emplace(id, name);

		// TODO: This should belong to some first-time setup.
		if (m_middle_content == nullptr) {
			set_channel(id);
		}
	}

	qDebug() << "Got channels: " << m_channels;

	// Set the channels in the left sidebar.
	m_left_sidebar->set_channels(m_channels);
}

void MainWindow::add_channel(uint64_t id, const QString &name)
{
	if (m_middle_contents.find(id) != m_middle_contents.end() ||
	    m_right_sidebars.find(id) != m_right_sidebars.end()) {
		return;
	}

	auto *middle_content = new MiddleContent{ id, name, this };
	m_middle_contents.emplace(id, middle_content);

	connect(middle_content, &MiddleContent::message_sent, this,
		[this](const QString &message) { create_message(message); });

	auto *right_sidebar = new RightSidebar{ this };

	m_right_sidebars.emplace(id, right_sidebar);
}

void MainWindow::set_channel(uint64_t id)
{
	auto *content = m_middle_contents.at(id);

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

	auto *sidebar = m_right_sidebars.at(id);

	if (sidebar == m_right_sidebar) {
		return;
	}

	if (m_right_sidebar != nullptr) {
		m_central_layout->removeWidget(m_right_sidebar);
		m_right_sidebar->hide();
	}

	m_right_sidebar = sidebar;

	if (m_right_sidebar != nullptr) {
		m_central_layout->insertWidget(3, m_right_sidebar);
		m_right_sidebar->show();
	}
}

void MainWindow::connect_to_gateway()
{
	m_ws.open(QUrl{ QString{ "ws://localhost:%1/gateway" }.arg(API_PORT) });
}

void MainWindow::identify()
{
	if (m_token.isEmpty()) {
		return;
	}

	QJsonObject data{ { "token", m_token } };
	QJsonObject obj{ { "op", 0 }, { "d", data } };
	QJsonDocument doc{ obj };

	m_ws.sendTextMessage(doc.toJson(QJsonDocument::Compact));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	if (m_modal) {
		m_modal->move_to_center();
	}
}
} // namespace kato