#ifndef KATO_COMPONENTS_NOW_PLAYING_BAR_HPP
#define KATO_COMPONENTS_NOW_PLAYING_BAR_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QWidget>

namespace kato
{
class NowPlayingBar : public QWidget {
	Q_OBJECT
    public:
	explicit NowPlayingBar(QWidget *parent = nullptr);

    signals:
	void go_back();

    public slots:
	void on_position_changed(qint64 position);

    private:
	void setup_ui();

	QVBoxLayout *m_vertical_layout{ new QVBoxLayout(this) };

	// Progress bar

	QHBoxLayout *m_progress_bar_layout{ new QHBoxLayout() };
	QLabel *m_playback_position_label{ new QLabel(this) };
	QProgressBar *m_progress_bar{ new QProgressBar(this) };

	// Actions

	QHBoxLayout *m_actions_layout{ new QHBoxLayout() };
	QSpacerItem *m_actions_left_spacer{ new QSpacerItem(
		20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding) };
	QPushButton *m_previous_button{ new QPushButton(this) };
	QPushButton *m_play_button{ new QPushButton(this) };
	QPushButton *m_next_button{ new QPushButton(this) };
	QSpacerItem *m_actions_right_spacer{ new QSpacerItem(
		20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding) };
};
} // namespace kato

#endif // KATO_COMPONENTS_NOW_PLAYING_BAR_HPP
