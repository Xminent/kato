#ifndef MOZART_VIEWS_MAIN_WINDOW_HPP
#define MOZART_VIEWS_MAIN_WINDOW_HPP

#include <QAudioProbe>
#include <QAudioRecorder>
#include <QMainWindow>
#include <QPushButton>
#include <QUdpSocket>
#include <mozart/components/hover_button.hpp>
#include <mozart/components/titlebar.hpp>

namespace mozart
{
struct MainWindow : QMainWindow {
	Q_OBJECT

    public:
	explicit MainWindow(QWidget *parent = nullptr);
	MainWindow(const MainWindow &) = delete;
	MainWindow &operator=(const MainWindow &) = delete;
	MainWindow(MainWindow &&) = delete;
	MainWindow &operator=(MainWindow &&) = delete;
	~MainWindow() override;

    private:
	void setup_audio();
	void setup_signals();
	void setup_ui();

	QWidget *m_central_widget{ new QWidget(this) };
	QVBoxLayout *m_central_layout{ new QVBoxLayout(m_central_widget) };

	// AudioRecording setup.
	QAudioProbe *m_audio_probe{ new QAudioProbe(this) };
	QAudioRecorder *m_audio_input{ new QAudioRecorder(this) };
	HoverButton *m_mute_button{ new HoverButton(
		QPixmap{ ":/icons/mute.svg" }, m_central_widget) };

	// Udp/client setup.
	QUdpSocket *m_udp_socket{ new QUdpSocket(this) };
	QPushButton *m_connect_button{ new QPushButton("Connect",
						       m_central_widget) };
	bool m_connected{};

	// User settings.
};
} // namespace mozart

#endif // MOZART_VIEWS_MAIN_WINDOW_HPP
