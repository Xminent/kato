#include "kato/components/user.hpp"
#include <QWheelEvent>
#include <kato/components/right_sidebar.hpp>

namespace kato
{
RightSidebar::RightSidebar(QWidget *parent)
	: QScrollArea{ parent }
	, m_header{ new Header{ "Members", std::nullopt, this } }
{
	setup_ui();
}

void RightSidebar::add_user(const QString &avatar, const QString &name)
{
	auto *user = new User{ avatar, name, this };
	m_user_layout->addWidget(user);
}

void RightSidebar::setup_ui()
{
	setFrameShape(Shape::NoFrame);
	setFixedWidth(240);
	setWidget(m_content);
	setWidgetResizable(true);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBar(m_scroll_bar);

	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);
	m_layout->addWidget(m_header);
	m_layout->addLayout(m_user_layout);
	m_layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum,
						QSizePolicy::Expanding));

	for (int i{}; i < 10; ++i) {
		add_user("", "kato");
	}
}
} // namespace kato
