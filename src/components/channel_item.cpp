#include <mozart/components/channel_item.hpp>
#include <qnamespace.h>

namespace
{
constexpr uint8_t GUILD_ICON_DIMENSION = 48;
constexpr uint8_t CIRCULAR_RADIUS = GUILD_ICON_DIMENSION / 2;
constexpr uint8_t ROUNDED_RECT_RADIUS = 7;
} // namespace

namespace mozart
{
ChannelItem::ChannelItem(const QString &name, QWidget *parent)
	: Widget{ parent }
	, m_name{ name }
{
	setup_ui();
}

void ChannelItem::setup_ui()
{
	QFont font;
	font.setFamily("Inter");
	font.setPointSize(12);
	setFont(font);

	setFixedSize(224, 34);
	set_background_color(Qt::transparent);
	set_text_color(QColor(142, 146, 151));
	icon->set_background_color(Qt::transparent);
	icon->setFixedSize(QSize(20, 20));
	icon->set_image(":/icons/text.svg");
	layout->addWidget(icon, 0, Qt::AlignVCenter);
	label->setText(m_name);
	layout->addWidget(label, 0, Qt::AlignVCenter);
	layout->addSpacerItem(spacer);
}

void ChannelItem::set_clicked(bool clicked)
{
	m_clicked = clicked;

	if (!m_change_when_clicked) {
		// Because our cursor will be over it when we click, setting clicked to true will lock the element with its "clicked" background color. We must reset it.
		set_background_color(QColor(47, 49, 54));
		set_border_radius(0);
		set_text_color(QColor(142, 146, 151));
		return;
	}

	if (clicked) {
		set_background_color(QColor(13, 13, 15));
		set_border_radius(ROUNDED_RECT_RADIUS);
		set_text_color(Qt::white);
		return;
	}

	set_text_color(QColor(142, 146, 151));
	set_background_color(Qt::transparent);
	set_border_radius(0);
}

void ChannelItem::enterEvent([[maybe_unused]] QEvent *e)
{
	if (m_clicked) {
		return;
	}

	set_text_color(Qt::white);
	set_background_color(QColor(8, 9, 10));
	set_border_radius(ROUNDED_RECT_RADIUS);
}

void ChannelItem::leaveEvent([[maybe_unused]] QEvent *e)
{
	if (m_clicked) {
		return;
	}

	set_text_color(QColor(142, 146, 151));
	set_background_color(Qt::transparent);
	set_border_radius(0);
}

void ChannelItem::mousePressEvent([[maybe_unused]] QMouseEvent *e)
{
	if (!m_clicked) {
		set_clicked(true);
	}
}

void ChannelItem::mouseReleaseEvent([[maybe_unused]] QMouseEvent *e)
{
	emit left_clicked(this);
}
} // namespace mozart