#include <QMouseEvent>
#include <kato/components/guild_icon.hpp>

namespace
{
constexpr uint8_t GUILD_ICON_DIMENSION = 48;
constexpr uint8_t CIRCULAR_RADIUS = GUILD_ICON_DIMENSION / 2;
constexpr uint8_t ROUNDED_RECT_RADIUS = GUILD_ICON_DIMENSION / 3;
} // namespace

namespace kato
{
GuildIcon::GuildIcon(const std::optional<QString> &name, QWidget *parent)
	: Widget{ parent }
	, m_name{ name }
{
	setup_ui();
}

void GuildIcon::unclick()
{
	m_clicked = false;
	set_background_color(QColor{ 49, 51, 56 });
	set_border_radius(CIRCULAR_RADIUS, true);
}

void GuildIcon::setup_ui()
{
	setFixedSize(QSize(GUILD_ICON_DIMENSION, GUILD_ICON_DIMENSION));
	set_background_color(QColor{ 49, 51, 56 });
	set_border_radius(CIRCULAR_RADIUS);
	set_text_color(Qt::white);

	if (!m_name) {
		set_pixmap(QPixmap{ ":/icons/home-icon.svg" });
		return;
	}

	const auto &name = *m_name;

	if (!name.isEmpty() && name.isSimpleText()) {
		set_text(QString{ name.isRightToLeft() ?
					  name[name.length() - 1] :
					  name[0] });
	} else {
		set_pixmap(QPixmap{ ":/images/logo.png" });
	}
}

void GuildIcon::enterEvent([[maybe_unused]] QEvent *event)
{
	if (m_clicked) {
		return;
	}

	set_background_color(QColor{ 68, 80, 242 });
	set_border_radius(ROUNDED_RECT_RADIUS, true);
}

void GuildIcon::leaveEvent([[maybe_unused]] QEvent *e)
{
	if (m_clicked) {
		return;
	}

	set_background_color(QColor{ 49, 51, 56 });
	set_border_radius(CIRCULAR_RADIUS, true);
}

void GuildIcon::mousePressEvent([[maybe_unused]] QMouseEvent *e)
{
	if (!m_clicked) {
		m_clicked = true;
	}
}

void GuildIcon::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		set_border_radius(ROUNDED_RECT_RADIUS, true);
	}

	emit left_clicked(this);
}
} // namespace kato
