#ifndef KATO_VIEWS_MAIN_WINDOW_HPP
#define KATO_VIEWS_MAIN_WINDOW_HPP

#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioProbe>
#include <QAudioRecorder>
#include <QBuffer>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QUdpSocket>
#include <QtWebSockets/QWebSocket>
#include <kato/api/audio_probe.hpp>
#include <kato/api/user.hpp>
#include <kato/components/left_sidebar.hpp>
#include <kato/components/middle_content.hpp>
#include <kato/components/modal.hpp>
#include <kato/components/navbar.hpp>
#include <kato/components/right_sidebar.hpp>
#include <kato/components/titlebar.hpp>
#include <kato/components/user.hpp>
#include <memory>

namespace kato
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
	void create_message(const QString &message,
			    const std::function<void()> &on_sent);
	void handle_gateway_event(const QJsonObject &json);
	void handle_get_channels(const QJsonArray &arr);
	void handle_get_users(const QJsonArray &arr);
	void add_channel(uint64_t id, const QString &name);
	void set_channel(std::optional<uint64_t> id);
	void fetch_channel_messages(uint64_t id);
	void connect_to_gateway();
	void identify();
	void resizeEvent(QResizeEvent *event) override;

	QWidget *m_central_widget{ new QWidget(this) };
	QHBoxLayout *m_central_layout{ new QHBoxLayout(m_central_widget) };
	NavBar *m_navbar{ new NavBar(m_central_widget) };

	QVBoxLayout *m_left_sidebar_layout{ new QVBoxLayout() };
	LeftSidebar *m_left_sidebar{ new LeftSidebar("Guild",
						     m_central_widget) };
	UserArea *m_user_area{ new UserArea(
		"https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSTUzFFTIH8Au-FWER6Jyna-LB1oBEC5bcuhw&usqp=CAU",
		"GOAT", m_central_widget) };

	/// Modal that will be shown for whatever currently focused task.
	std::unique_ptr<Modal> m_modal;

	// Represents the chat content for each individual channel.
	std::map<uint64_t, MiddleContent *> m_middle_contents;
	MiddleContent *m_middle_content{};
	// Represents the chat contents we've fetched from the API at startup.
	QSet<uint64_t> m_channels_fetched;

	// Represents the right sidebar for each individual channel.
	std::map<uint64_t, RightSidebar *> m_right_sidebars;
	RightSidebar *m_right_sidebar{};

	QSpacerItem *m_spacer{ new QSpacerItem(
		0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum) };

	std::map<uint64_t, QString> m_channels;

	// Users for each guild. (Currently only have one guild).
	std::map<uint64_t, User *> m_users;

	// AudioRecording/Playback setup.
	// QAudioProbe *m_audio_probe{ new QAudioProbe(this) };
	AudioProbeDevice *m_audio_probe{ new AudioProbeDevice(this) };
	QAudioRecorder *m_audio_recorder{ new QAudioRecorder(this) };
	QAudioFormat m_format;
	QAudioInput *m_audio_input{};
	QAudioOutput *m_audio_output{};
	QBuffer *m_buffer{ new QBuffer(this) };

	// Udp/client setup. (AUDIO)
	QUdpSocket *m_udp_socket{ new QUdpSocket(this) };
	bool m_udp_connected{};

	// Websocket setup.
	QWebSocket m_ws;
	// ID acquired from authenticating.
	uint64_t m_id{};
	// Avatar acquired from authenticating.
	QString m_avatar;
	// Name acquired from authenticating.
	QString m_name;
	// Token provided at runtime/startup.
	QString m_token;

	// Http setup.
	QNetworkAccessManager *m_network_manager{ new QNetworkAccessManager(
		this) };

	// User settings.
};
} // namespace kato

#endif // KATO_VIEWS_MAIN_WINDOW_HPP
