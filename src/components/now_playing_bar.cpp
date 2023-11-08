#include <QTime>
#include <mozart/components/now_playing_bar.hpp>

namespace mozart
{
NowPlayingBar::NowPlayingBar(QWidget *parent)
	: QWidget{ parent }
{
	// When the prev button is clicked emit a go_back signal.
	connect(m_previous_button, &QPushButton::clicked,
		[this] { emit go_back(); });
	// When the play button is clicked emit a play signal.

	setup_ui();
}

void NowPlayingBar::on_position_changed(qint64 position)
{
	m_playback_position_label->setText(
		QTime(0, 0, 0).addMSecs(position).toString("m:ss"));
}

void NowPlayingBar::setup_ui()
{
	setFixedHeight(115);

	QPalette pal = palette();
	pal.setColor(QPalette::Window, QColor(9, 9, 9));
	setAutoFillBackground(true);
	setPalette(pal);

	// TODO: Remove this.
	m_progress_bar->setValue(50);

	m_progress_bar->setStyleSheet(
		QString::fromUtf8("QProgressBar {\n"
				  "    background: #535353;\n"
				  "    border-style: outset;\n"
				  "    border-radius: 2px;\n"
				  "}\n"
				  "\n"
				  "QProgressBar::chunk {\n"
				  "    background-color: #ffffff;\n"
				  "    border-radius: 7px;\n"
				  "}\n"
				  "\n"
				  "QProgressBar::chunk::hover {\n"
				  "    background-color: #1db954;\n"
				  "    border-radius: 7px;\n"
				  "}"));
	m_progress_bar->setTextVisible(false);

	m_playback_position_label->setText("0:00");
	m_playback_position_label->setToolTip("Playback position");

	m_progress_bar_layout->addWidget(m_playback_position_label);
	m_progress_bar_layout->addWidget(m_progress_bar);

	m_previous_button->setText("Previous");
	m_play_button->setText("Play");
	m_next_button->setText("Next");

	m_actions_layout->addItem(m_actions_left_spacer);
	m_actions_layout->addWidget(m_previous_button);
	m_actions_layout->addWidget(m_play_button);
	m_actions_layout->addWidget(m_next_button);
	m_actions_layout->addItem(m_actions_right_spacer);

	m_vertical_layout->addLayout(m_progress_bar_layout);
	m_vertical_layout->addLayout(m_actions_layout);
}
} // namespace mozart
