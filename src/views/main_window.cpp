#include <QDebug>
#include <QFontDatabase>
#include <QJsonDocument>
#include <mozart/api/win_dark.hpp>
#include <mozart/views/main_window.hpp>

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
			qDebug() << "client received: " << message;
			m_middle_content->add_message(message);
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
	m_central_layout->addWidget(m_middle_content);
	// spacer to keep everything.
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

	qDebug() << "sending: " << doc.toJson();

	m_ws.sendTextMessage(doc.toJson());
}
} // namespace mozart