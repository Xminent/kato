#include <QDebug>
#include <QFontDatabase>
#include <mozart/api/win_dark.hpp>
#include <mozart/views/main_window.hpp>

namespace
{
constexpr QSize MIN_WINDOW_SIZE{ 800, 600 };

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

	connect(m_mute_button, &HoverButton::clicked, this,
		[this, muted = true]() mutable {
			muted = !muted;

			// Can't send audio if not connected.
			if (!m_connected) {
				return;
			}

			// Toggle sending audio to the server.
			// muted ? m_audio_input->pause() :
			// 	m_audio_input->record();
		});

	connect(m_connect_button, &QPushButton::clicked, this, [this] {
		m_connected = !m_connected;

		m_connected ? m_connect_button->setText("Disconnect") :
			      m_connect_button->setText("Connect");
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
}

void MainWindow::setup_ui()
{
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

	auto *label = new QLabel{ "Welcome to PornHub! 🟧⬛  😩💦",
				  m_central_widget };
	label->setTextInteractionFlags(Qt::TextSelectableByMouse);

	m_mute_button->set_background_color(Qt::transparent);
	m_mute_button->set_border_color(QColor{ 42, 42, 42 });
	m_mute_button->setFixedSize(20, 20);

	m_central_layout->addWidget(label, 0, Qt::AlignCenter);
	m_central_layout->addWidget(m_connect_button);
	m_central_layout->addWidget(m_mute_button);
	m_central_layout->addStretch();

	setCentralWidget(m_central_widget);
}
} // namespace mozart