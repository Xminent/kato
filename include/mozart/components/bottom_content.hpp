// #pragma once
// #include <QSlider>
// #include <QVBoxLayout>
// #include <mozart/now_playing_bar_button.hpp>
// #include <mozart/play_pause_button.hpp>

// namespace mozart
// {
// class BottomContent : public Widget {
// 	Q_OBJECT

//     public:
// 	explicit BottomContent(QWidget *parent = nullptr);

// 	void set_playback_state(bool is_playing);

//     signals:
// 	void play();
// 	void pause();
// 	void next();
// 	void previous();
// 	void shuffle();
// 	void repeat();
// 	void volume(int value);

//     private:
// 	void setup_ui();

// 	QHBoxLayout *m_central_layout{ new QHBoxLayout(this) };
// 	QSpacerItem *m_spacer{ new QSpacerItem(16, 20, QSizePolicy::Expanding,
// 					       QSizePolicy::Minimum) };
// 	HoverButton *m_shuffle_button{ new HoverButton(
// 		QPixmap(":/icons/shuffle.svg"), this) };
// 	HoverButton *m_previous_button{ new HoverButton(
// 		QPixmap(":/icons/previous.svg"), this) };
// 	PlayPauseButton *m_play_button{ new PlayPauseButton(this) };
// 	HoverButton *m_next_button{ new HoverButton(QPixmap(":/icons/next.svg"),
// 						    this) };
// 	HoverButton *m_repeat_button{ new HoverButton(
// 		QPixmap(":/icons/repeat.svg"), this) };

// 	QSpacerItem *m_spacer2{ new QSpacerItem(32, 20, QSizePolicy::Expanding,
// 						QSizePolicy::Minimum) };

// 	QPixmap m_volume_icon{ ":/icons/volume.svg" };
// 	QPixmap m_volume_low_icon{ ":/icons/volume_low.svg" };
// 	QPixmap m_volume_off_icon{ ":/icons/volume_off.svg" };
// 	int m_volume_value{};

// 	HoverButton *m_volume_button{ new HoverButton(m_volume_icon, this) };
// 	QSlider *m_volume_slider{ new QSlider(Qt::Horizontal, this) };
// 	QSpacerItem *m_spacer3{ new QSpacerItem(16, 20, QSizePolicy::Fixed,
// 						QSizePolicy::Minimum) };
// };
// } // namespace mozart

#ifndef MOZART_COMPONENTS_BOTTOM_CONTENT_HPP
#define MOZART_COMPONENTS_BOTTOM_CONTENT_HPP

#endif // MOZART_COMPONENTS_BOTTOM_CONTENT_HPP
