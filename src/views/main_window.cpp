#include <QDebug>
#include <QFontDatabase>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QProcessEnvironment>
#include <QStackedWidget>
#include <kato/api/win_dark.hpp>
#include <kato/defer.hpp>
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

	m_left_sidebar_layout->setContentsMargins(0, 0, 0, 0);
	m_left_sidebar_layout->setSpacing(0);
	m_left_sidebar_layout->addWidget(m_left_sidebar);
	m_left_sidebar_layout->addWidget(m_user_area);

	m_central_layout->setContentsMargins(0, 0, 0, 0);
	m_central_layout->setSpacing(0);
	m_central_layout->addWidget(m_navbar);
	m_central_layout->addLayout(m_left_sidebar_layout);
	m_central_layout->addSpacerItem(m_spacer);

	setCentralWidget(m_central_widget);
}

void MainWindow::create_message(const QString &message,
				const std::function<void()> &on_sent)
{
	if (m_middle_content == nullptr) {
		return;
	}

	QJsonObject data{
		{ "content", message },
	};
	QJsonDocument doc{ data };

	// TODO: Make the endpoint an environment variable.
	auto url = QString{ "http://localhost:%1/api/channels/%2/messages" }
			   .arg(API_PORT)
			   .arg(m_middle_content->channel_id());

	qDebug() << "Sending: " << doc.toJson(QJsonDocument::Compact) << " to "
		 << url;

	auto req = QNetworkRequest{ QUrl{ url } };

	req.setRawHeader("Authorization",
			 QString{ "Bearer %1" }.arg(m_token).toUtf8());
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	auto *reply = m_network_manager->post(
		req, doc.toJson(QJsonDocument::Compact));

	connect(reply, &QNetworkReply::finished, reply, [reply, on_sent] {
		defer
		{
			reply->deleteLater();
		};

		if (reply->error() != QNetworkReply::NoError) {
			qDebug() << "Error: " << reply->errorString();
			return;
		}

		on_sent();
	});
}

void MainWindow::handle_gateway_event(const QJsonObject &json)
{
	const auto op_it = json.find("op");
	const auto d_it = json.find("d");

	if (op_it == json.end() || !op_it->isDouble() || d_it == json.end() ||
	    !d_it->isObject()) {
		qDebug() << "Invalid JSON: " << json;
		return;
	}

	const auto op = static_cast<GatewayOpcode>(op_it->toInt());
	const auto d = d_it->toObject();

	qDebug() << "received op: " << static_cast<int>(op) << " d: " << d;

	switch (op) {
	case GatewayOpcode::Ready: {
		const auto user_it = d.find("user");
		const auto channels_it = d.find("channels");
		const auto users = d.find("users");

		if (user_it == d.end() || !user_it->isObject() ||
		    channels_it == d.end() || !channels_it->isArray() ||
		    users == d.end() || !users->isArray()) {
			qDebug() << "Invalid JSON: " << d;
			return;
		}

		const auto user = user_it->toObject();
		const auto id_it = user.find("id");
		const auto avatar_it = user.find("avatar");
		const auto username_it = user.find("username");

		if (id_it == user.end() || !id_it->isDouble() ||
		    avatar_it == user.end() || !avatar_it->isString() ||
		    username_it == user.end() || !username_it->isString()) {
			qDebug() << "Invalid JSON: " << d;
		}

		m_id = static_cast<uint64_t>(id_it->toDouble());
		m_avatar = avatar_it->toString();
		m_name = username_it->toString();
		qDebug() << "Setting name to: " << m_name;
		handle_get_channels(channels_it->toArray());
		handle_get_users(users->toArray());
		break;
	}
	case GatewayOpcode::MessageCreate: {
		const auto author_it = d.find("author");
		const auto channel_id_it = d.find("channel_id");
		const auto content_it = d.find("content");
		const auto id_it = d.find("id");

		if (author_it == d.end() || !author_it->isObject() ||
		    channel_id_it == d.end() || !channel_id_it->isDouble() ||
		    content_it == d.end() || !content_it->isString() ||
		    id_it == d.end() || !id_it->isDouble()) {
			qDebug() << "Invalid JSON: " << d;
			return;
		}

		const auto channel_id =
			static_cast<uint64_t>(channel_id_it->toDouble());

		if (m_middle_contents.find(channel_id) ==
		    m_middle_contents.end()) {
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

		const auto id = static_cast<uint64_t>(id_it->toDouble());

		qDebug() << "adding message: " << id
			 << "to channel: " << channel_id;

		m_middle_contents.at(channel_id)
			->add_message(id, avatar, author_name_it->toString(),
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

void MainWindow::handle_get_channels(const QJsonArray &arr)
{
	for (const auto &value : arr) {
		if (!value.isObject()) {
			continue;
		}

		const auto obj = value.toObject();
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
	}

	qDebug() << "Got channels: " << m_channels;

	// Set the channels in the left sidebar.
	m_left_sidebar->set_channels(m_channels);
}

void MainWindow::handle_get_users(const QJsonArray &arr)
{
	for (const auto &value : arr) {
		if (!value.isObject()) {
			continue;
		}

		const auto obj = value.toObject();
		const auto id_it = obj.find("id");
		const auto avatar_it = obj.find("avatar");
		const auto name_it = obj.find("username");

		if (id_it == obj.end() || avatar_it == obj.end() ||
		    !id_it->isDouble() || name_it == obj.end() ||
		    !name_it->isString()) {
			continue;
		}

		QString avatar;

		if (avatar_it->isString()) {
			avatar = avatar_it->toString();
		}

		const auto id = static_cast<uint64_t>(id_it->toDouble());
		const auto name = name_it->toString();

		m_users.try_emplace(id, new User{ avatar, name });
	}

	if (m_right_sidebar == nullptr) {
		return;
	}

	for (const auto &[_, user] : m_users) {
		m_right_sidebar->add_user(user);
	}

	qDebug() << "Got " << m_users.size() << " users";
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
		[this](const QString &message, const auto &on_sent) {
			create_message(message, on_sent);
		});

	auto *right_sidebar = new RightSidebar{ this };

	m_right_sidebars.emplace(id, right_sidebar);
}

void MainWindow::set_channel(std::optional<uint64_t> id)
{
	defer
	{
		if (m_middle_content == nullptr && m_right_sidebar == nullptr) {
			m_central_layout->addSpacerItem(m_spacer);
		}
	};

	if (id && !m_channels_fetched.contains(*id)) {
		fetch_channel_messages(*id);
	}

	auto *content = id ? m_middle_contents.at(*id) : nullptr;

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

	auto *sidebar = id ? m_right_sidebars.at(*id) : nullptr;

	if (sidebar == m_right_sidebar) {
		return;
	}

	if (m_right_sidebar != nullptr) {
		m_central_layout->removeWidget(m_right_sidebar);
		m_right_sidebar->hide();
	}

	m_right_sidebar = sidebar;

	if (m_right_sidebar != nullptr) {
		for (const auto &[_, user] : m_users) {
			m_right_sidebar->add_user(user);
		}

		m_central_layout->insertWidget(3, m_right_sidebar);
		m_right_sidebar->show();
	}

	m_central_layout->removeItem(m_spacer);
}

void MainWindow::fetch_channel_messages(uint64_t channel_id)
{
	auto *middle_content = m_middle_contents[channel_id];

	if (middle_content == nullptr) {
		return;
	}

	// build a request to fetch the channel.
	QNetworkRequest req{ QUrl{
		QString{ "http://localhost:%1/api/channels/%2/messages" }
			.arg(API_PORT)
			.arg(channel_id) } };

	req.setRawHeader("Authorization",
			 QString{ "Bearer %1" }.arg(m_token).toUtf8());

	qDebug() << "Fetching messages for channel: " << channel_id << " from "
		 << req.url();

	auto *reply = m_network_manager->get(req);

	m_channels_fetched.insert(channel_id);

	connect(reply, &QNetworkReply::finished, reply,
		[this, middle_content, reply, channel_id] {
			defer
			{
				reply->deleteLater();
			};

			if (reply->error() != QNetworkReply::NoError) {
				qDebug() << "Error: " << reply->error();
				m_channels_fetched.remove(channel_id);
				return;
			}

			QJsonDocument doc{ QJsonDocument::fromJson(
				reply->readAll()) };

			if (!doc.isArray()) {
				qDebug() << "Not an object";
				return;
			}

			const auto arr = doc.array();

			for (const auto &value : arr) {
				if (!value.isObject()) {
					continue;
				}

				const auto message = value.toObject();
				const auto id_it = message.find("id");
				const auto author_it = message.find("author");
				const auto content_it = message.find("content");
				const auto created_at_it =
					message.find("created_at");

				if (id_it == message.end() ||
				    !id_it->isDouble() ||
				    author_it == message.end() ||
				    !author_it->isObject() ||
				    content_it == message.end() ||
				    !content_it->isString() ||
				    created_at_it == message.end() ||
				    !created_at_it->isDouble()) {
					continue;
				}

				const auto id = static_cast<uint64_t>(
					id_it->toDouble());
				const auto author = author_it->toObject();
				const auto content = content_it->toString();
				const auto created_at =
					QDateTime::fromSecsSinceEpoch(
						static_cast<qint64>(
							created_at_it
								->toDouble()));
				const auto avatar_it = author.find("avatar");
				const auto name_it = author.find("username");

				QString avatar;

				if (avatar_it != author.end() &&
				    avatar_it->isString()) {
					avatar = avatar_it->toString();
				}

				if (name_it == author.end() ||
				    !name_it->isString()) {
					continue;
				}

				const auto name = name_it->toString();

				middle_content->add_message(
					id, avatar, name, content, created_at);
			}

			qDebug() << "Got " << arr.size()
				 << " messages for channel: " << channel_id;
		});
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
