#include "mozart/components/message_item.hpp"
#include "mozart/components/widget.hpp"
#include <QDebug>
#include <QUrl>
#include <mozart/components/middle_content.hpp>
#include <qnamespace.h>

namespace mozart
{
MiddleContent::MiddleContent(const QString &name, QWidget *parent)
	: Widget{ parent }
	, m_name{ name }
{
	setup_ui();

	connect(m_form, &Form::message_sent, this,
		&MiddleContent::message_sent);
}

void MiddleContent::add_message(const QString &message)
{
	static int count{};

	auto *label = new MessageItem{ "Xminent", message, this };

	m_messages_layout->addWidget(label);
}

void MiddleContent::setup_ui()
{
	setMinimumWidth(390);
	set_background_color(Qt::transparent);

	m_central_layout->addItem(m_main_layout);
	m_central_layout->setContentsMargins(0, 0, 0, 0);

	m_central_layout->setSpacing(4);
	m_central_layout->setStretch(0, 1);
	m_central_layout->setStretch(1, 3);

	m_main_layout->addWidget(m_header);
	m_main_layout->addWidget(m_scroll_area);
	m_main_layout->addWidget(m_form);
	m_main_layout->setContentsMargins(0, 0, 0, 0);
	m_main_layout->setSpacing(0);
	m_main_layout->setStretch(0, 2);
	m_main_layout->setStretch(1, 30);
	m_main_layout->setStretch(2, 1);

	m_scroll_area->setWidget(m_scroll_area_widget_contents);
	m_scroll_area->setWidgetResizable(true);
	m_scroll_area->setFrameShape(QFrame::NoFrame);
	m_scroll_area->setVerticalScrollBar(m_scroll_bar);
	m_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_scroll_area_layout->addLayout(m_messages_layout);
	m_scroll_area_layout->addSpacerItem(m_spacer);
	m_scroll_area_widget_contents->set_background_color(Qt::transparent);
}
} // namespace mozart
