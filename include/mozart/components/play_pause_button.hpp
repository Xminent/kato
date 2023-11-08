#ifndef MOZART_COMPONENTS_PLAY_PAUSE_BUTTON_HPP
#define MOZART_COMPONENTS_PLAY_PAUSE_BUTTON_HPP

#include <mozart/components/widget.hpp>

namespace mozart
{
struct PlayPauseButton : Widget {
	Q_OBJECT

    public:
	explicit PlayPauseButton(QWidget *parent = nullptr);

	void set_playback_state(bool is_playing);

    signals:
	void play();
	void pause();

    private:
	void mousePressEvent(QMouseEvent *event) override;
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;

	void setup_ui();

	QPixmap m_pixmap_play;
	QPixmap m_pixmap_pause;
	bool m_is_playing{};
};
} // namespace mozart

#endif // MOZART_COMPONENTS_PLAY_PAUSE_BUTTON_HPP
