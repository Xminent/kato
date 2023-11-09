#ifndef MOZART_VIEWS_MAIN_WINDOW_HPP
#define MOZART_VIEWS_MAIN_WINDOW_HPP

#include <QAudioProbe>
#include <QAudioRecorder>
#include <QMainWindow>
#include <QPushButton>
#include <QUdpSocket>
#include <QtWebSockets/QWebSocket>
#include <mozart/components/left_sidebar.hpp>
#include <mozart/components/middle_content.hpp>
#include <mozart/components/navbar.hpp>
#include <mozart/components/titlebar.hpp>
#include <qnetworkaccessmanager.h>

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
	void send_message(const QString &message);
	void handle_gateway_event(const QJsonObject &json);
	void handle_get_rooms(QNetworkReply *reply);

	QWidget *m_central_widget{ new QWidget(this) };
	QHBoxLayout *m_central_layout{ new QHBoxLayout(m_central_widget) };
	NavBar *m_navbar{ new NavBar(m_central_widget) };
	LeftSidebar *m_left_sidebar{ new LeftSidebar("Guild",
						     m_central_widget) };

	// Represents the chat content for each individual channel.
	std::unordered_map<uint64_t, MiddleContent *> m_middle_contents;
	MiddleContent *m_middle_content{};

	// AudioRecording setup.
	QAudioProbe *m_audio_probe{ new QAudioProbe(this) };
	QAudioRecorder *m_audio_input{ new QAudioRecorder(this) };

	// Udp/client setup. (AUDIO)
	QUdpSocket *m_udp_socket{ new QUdpSocket(this) };
	bool m_udp_connected{};

	// Websocket setup.
	QWebSocket m_ws;

	// Http setup.
	QNetworkAccessManager *m_network_manager{ new QNetworkAccessManager(
		this) };

	// User settings.
};
} // namespace mozart

#endif // MOZART_VIEWS_MAIN_WINDOW_HPP
