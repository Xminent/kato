#include <mozart/components/navbar.hpp>

namespace mozart
{
NavBar::NavBar(QWidget *parent)
	: QScrollArea{ parent }
{
	setup_ui();
}

void NavBar::add_item(NavBarItem *nav)
{
	connect(nav, &NavBarItem::left_clicked, this,
		[this](const NavBarItem *g) {
			for (int j{}; j < m_icon_layout->count(); j++) {
				if (auto *item = qobject_cast<NavBarItem *>(
					    m_icon_layout->itemAt(j)->widget());
				    item != nullptr && item != g) {
					item->unclick();
				}
			}

			if (g == m_home_button) {
				emit home_clicked();
			}
		});

	m_icon_layout->addWidget(nav, 0, Qt::AlignHCenter);
}

void NavBar::setup_ui()
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFixedWidth(70);
	setFrameShape(QFrame::NoFrame);

	QPalette palette;
	QBrush brush(QColor{ 6, 6, 7 });
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Window, brush);
	setPalette(palette);

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_scroll_bar->setOrientation(Qt::Vertical);

	setVerticalScrollBar(m_scroll_bar);
	setWidgetResizable(true);
	setWidget(m_content);

	m_layout->setContentsMargins(0, 0, 0, 0);
	m_icon_layout->setContentsMargins(0, 0, 0, 0);

	// TODO: Consider, Add spacer thats 12px tall.
	// m_layout->addSpacerItem(new QSpacerItem(0, 12, QSizePolicy::Minimum,
	// 					QSizePolicy::Fixed));
	m_layout->addLayout(m_icon_layout);
	// Add a spacer to keep the elements near the top of the layout.
	m_layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum,
						QSizePolicy::Expanding));

	add_item(m_home_button);

	// TODO: Remove debug. Add 10 GuildIcons.
	for (int i{}; i < 5; i++) {
		auto *item = new NavBarItem(QString("%1").arg(i), this);

		add_item(item);
	}
}

void NavBar::wheelEvent(QWheelEvent *e)
{
	m_scroll_bar->scroll(e->angleDelta().y());
}
} // namespace mozart
