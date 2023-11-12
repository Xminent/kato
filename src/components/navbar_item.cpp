#include <QMouseEvent>
#include <kato/components/navbar_item.hpp>

namespace kato
{
NavBarItem::NavBarItem(const std::optional<QString> &name, QWidget *parent)
	: Widget{ parent }
	, m_icon{ new GuildIcon(name, this) }
{
	setup_ui();

	connect(m_icon, &GuildIcon::left_clicked, this, [this] {
		m_clicked = true;
		m_pill->set_background_color(Qt::white);
		m_pill_container->setFixedSize(4, 40);
		m_pill->set_fixed_size(4, 40, 150);
		m_pill->set_border_radius(0, 4, 4, 0);
		emit left_clicked(this);
	});
}

void NavBarItem::unclick()
{
	m_clicked = false;
	m_icon->unclick();
	m_pill_container->setFixedSize(4, 20);
	m_pill->set_fixed_size(4, 20, 150);
	m_pill->set_background_color(Qt::transparent);
	m_pill->set_border_radius(0, 4, 4, 0);
}

void NavBarItem::setup_ui()
{
	setCursor(Qt::PointingHandCursor);
	setFixedSize(72, 48);
	set_background_color(Qt::transparent);

	m_pill->setFixedSize(4, 20);
	m_pill->set_background_color(Qt::transparent);
	m_pill->set_border_radius(0, 4, 4, 0);

	m_pill_container->setFixedSize(4, 20);
	m_pill_layout->setContentsMargins(0, 0, 0, 0);
	m_pill_layout->addWidget(m_pill);

	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->addWidget(m_pill_container);
	m_layout->addWidget(m_icon);
	m_layout->addSpacerItem(new QSpacerItem(16, 0, QSizePolicy::Fixed,
						QSizePolicy::Minimum));
}

void NavBarItem::enterEvent(QEvent *e)
{
	if (m_clicked) {
		return;
	}

	m_pill_container->setFixedSize(4, 20);
	m_pill->set_fixed_size(4, 20, 150);
	m_pill->set_background_color(Qt::white);
}

void NavBarItem::leaveEvent(QEvent *e)
{
	if (m_clicked) {
		return;
	}

	// m_pill->set_fixed_size(4, 20, true);
	m_pill->set_fixed_size(0, 0, true);
	m_pill->set_background_color(Qt::transparent);
	m_pill->set_border_radius(0, 4, 4, 0);
}
} // namespace kato
