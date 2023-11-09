#include <QDesktopServices>
#include <QGraphicsDropShadowEffect>
#include <mozart/components/titlebar.hpp>

namespace mozart
{
Titlebar::Titlebar(QWidget *parent)
	: Widget{ parent }
{
	setup_ui();

	connect(m_minimize_button, &WinButton::left_clicked, this,
		[this] { emit window_minimized(); });

	connect(m_maximize_button, &WinButton::left_clicked, this, [this] {
		m_is_maximized = !m_is_maximized;

		m_maximize_button->set_pixmap(
			m_is_maximized ? QPixmap{ ":/icons/restore.svg" } :
					 QPixmap{ ":/icons/maximize.svg" });

		m_is_maximized ? emit window_maximized() :
				 emit window_restored();
	});

	connect(m_close_button, &WinButton::left_clicked, this,
		[this] { emit window_closed(); });
}

void Titlebar::setup_ui()
{
	set_background_color(QColor::fromRgba(qRgba(255, 255, 255, 10)));
	set_border_radius(8);
	setFixedHeight(64);

	m_central_layout->addWidget(m_logo);
	m_central_layout->addItem(m_spacer3);
	m_central_layout->addWidget(m_title_label);
	m_central_layout->addItem(m_spacer);
	m_central_layout->addWidget(m_profile_picture);
	m_central_layout->addItem(m_spacer2);
	m_central_layout->addWidget(m_minimize_button, 0, Qt::AlignTop);
	m_central_layout->addWidget(m_maximize_button, 0, Qt::AlignTop);
	m_central_layout->addWidget(m_close_button, 0, Qt::AlignTop);
	m_central_layout->setContentsMargins(0, 0, 0, 0);
	m_central_layout->setSpacing(0);

	m_logo->setFixedSize(40, 40);
	m_logo->setPixmap(QPixmap{ ":/images/logo.png" });
	m_logo->setScaledContents(true);

	m_title_label->setText("Voice Chat");
	auto font = m_title_label->font();
	font.setPointSize(10);
	font.setBold(true);
	m_title_label->setFont(font);

	m_profile_picture->setFixedSize(40, 40);
	m_profile_picture->set_border_radius(20);
	m_profile_picture->set_pixmap(QPixmap{ ":/images/mozart.png" });

	auto *effect = new QGraphicsDropShadowEffect();
	effect->setBlurRadius(20);
	effect->setColor(Qt::red);
	effect->setOffset(0, 0);
	m_profile_picture->setGraphicsEffect(effect);
}
} // namespace mozart
