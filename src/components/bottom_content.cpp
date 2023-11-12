// #include <QBitmap>
// #include <kato/bottom_content.hpp>

// namespace kato
// {
// BottomContent::BottomContent(QWidget *parent)
// 	: Widget{parent}
// {
// 	setup_ui();

// 	connect(m_shuffle_button, &HoverButton::clicked, this,
// 		&BottomContent::shuffle);
// 	connect(m_previous_button, &HoverButton::clicked, this,
// 		&BottomContent::previous);
// 	connect(m_play_button, &PlayPauseButton::play, this,
// 		&BottomContent::play);
// 	connect(m_play_button, &PlayPauseButton::pause, this,
// 		&BottomContent::pause);
// 	connect(m_next_button, &HoverButton::clicked, this,
// 		&BottomContent::next);
// 	connect(m_repeat_button, &HoverButton::clicked, this,
// 		&BottomContent::repeat);

// 	connect(m_volume_button, &HoverButton::clicked, this, [this] {
// 		if (m_volume_slider->value() == 0) {
// 			m_volume_slider->setValue(m_volume_value);
// 		} else {
// 			m_volume_value = m_volume_slider->value();
// 			m_volume_slider->setValue(0);
// 		}
// 	});

// 	connect(m_volume_slider, &QSlider::valueChanged, this,
// 		[this](int value) {
// 			if (value == 0) {
// 				m_volume_button->set_pixmap(m_volume_off_icon);
// 			} else if (value < 75) {
// 				m_volume_button->set_pixmap(m_volume_low_icon);
// 			} else {
// 				m_volume_button->set_pixmap(m_volume_icon);
// 			}

// 			emit volume(value);
// 		});
// }

// void BottomContent::set_playback_state(bool is_playing)
// {
// 	m_play_button->set_playback_state(is_playing);
// }

// void BottomContent::setup_ui()
// {
// 	// set_background_color(QColor(18, 18, 18));
// 	// set_border_radius(0, 0, 10, 10);
// 	setFixedHeight(90);

// 	m_central_layout->addSpacerItem(m_spacer);
// 	m_central_layout->addWidget(m_shuffle_button);
// 	m_central_layout->addWidget(m_previous_button);
// 	m_central_layout->addWidget(m_play_button);
// 	m_central_layout->addWidget(m_next_button);
// 	m_central_layout->addWidget(m_repeat_button);
// 	m_central_layout->addWidget(m_volume_slider);
// 	m_central_layout->addSpacerItem(m_spacer2);
// 	m_central_layout->addWidget(m_volume_button);
// 	m_central_layout->addWidget(m_volume_slider);
// 	m_central_layout->addSpacerItem(m_spacer3);
// 	m_central_layout->setContentsMargins(0, 0, 0, 0);

// 	// Each one of our buttons needs to have a tooltip.
// 	m_shuffle_button->setToolTip("Shuffle");
// 	m_previous_button->setToolTip("Previous");
// 	m_play_button->setToolTip("Play");
// 	m_next_button->setToolTip("Next");
// 	m_repeat_button->setToolTip("Repeat");

// 	m_volume_slider->setFixedWidth(100);
// 	m_volume_slider->setValue(50);
// 	m_volume_slider->setStyleSheet(R"(
//         QSlider::groove:horizontal {
//             background: #ffffff;
//             border-radius: 4px;
//             height: 8px;
//         }

//         QSlider::groove:horizontal:hover {
//             background: #be1d1c;
//         }

//         QSlider::handle:horizontal {
//             background: transparent;
//             border-radius: 8px;
//             height: 16px;
//             margin: -4px 0;
//             width: 16px;
//         }

//         QSlider::handle:horizontal:hover {
//             background: #ffffff;
//         }

//         QSlider::add-page:horizontal {
//             background: #535353;
//             border-radius: 4px;
//             height: 8px;
//         }
//     )");
// }
// } // namespace kato
