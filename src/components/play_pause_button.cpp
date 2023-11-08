#include <QMouseEvent>
#include <mozart/components/play_pause_button.hpp>

namespace mozart
{
PlayPauseButton::PlayPauseButton(QWidget *parent)
	: Widget{ parent }
{
	QPixmap pixmap_play{ ":/icons/play.svg" };
	// QPainter painter { &pixmap_play };
	// painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	// painter.fillRect(pixmap_play.rect(), Qt::black);

	QPixmap pixmap_pause{ ":/icons/pause.svg" };
	// painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	// painter.fillRect(pixmap_pause.rect(), Qt::black);

	m_pixmap_play = pixmap_play;
	m_pixmap_pause = pixmap_pause;

	setup_ui();
}

void PlayPauseButton::set_playback_state(bool is_playing)
{
	m_is_playing = is_playing;
	set_pixmap(m_is_playing ? m_pixmap_pause : m_pixmap_play);
}

void PlayPauseButton::setup_ui()
{
	set_background_color(Qt::white);
	set_border_radius(16);
	setFixedSize(32, 32);
	set_pixmap(m_is_playing ? m_pixmap_pause : m_pixmap_play);
}

void PlayPauseButton::mousePressEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton) {
		return;
	}

	m_is_playing = !m_is_playing;

	if (m_is_playing) {
		set_pixmap(m_pixmap_play);
		emit pause();
		return;
	}

	set_pixmap(m_pixmap_pause);
	emit play();
}

void PlayPauseButton::enterEvent([[maybe_unused]] QEvent *event)
{
	setFixedSize(36, 36);
	set_border_radius(18);
}

void PlayPauseButton::leaveEvent([[maybe_unused]] QEvent *event)
{
	setFixedSize(32, 32);
	set_border_radius(16);
}
} // namespace mozart
