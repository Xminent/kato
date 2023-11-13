#include <QDebug>
#include <QUrl>
#include <kato/components/message_item.hpp>
#include <kato/components/middle_content.hpp>

namespace kato
{
MiddleContent::MiddleContent(uint64_t channel_id, const QString &channel_name,
			     QWidget *parent)
	: Widget{ parent }
	, m_channel_id{ channel_id }
	, m_channel_name{ channel_name }
	, m_header{ new Header(channel_name, QPixmap{ ":/icons/text.svg" },
			       this) }
{
	setup_ui();

	connect(m_form, &Form::message_sent, this,
		[this](const QString &message) {
			emit message_sent(message, [this] {
				m_scroll_to_bottom = true;
				m_form->clear();
			});
		});

	connect(m_scroll_bar, &ScrollBar::rangeChanged, this,
		[this]([[maybe_unused]] int min, int max) {
			if (m_scroll_to_bottom) {
				m_scroll_bar->setValue(max);
				m_scroll_to_bottom = false;
			}
		});
}

void MiddleContent::add_message(uint64_t id, const QString &avatar,
				const QString &author, const QString &message)
{
	m_messages_layout->addWidget(
		new MessageItem{ id, avatar, author, message, this });
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
	m_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	m_scroll_area_layout->addLayout(m_messages_layout);
	m_scroll_area_layout->addSpacerItem(m_spacer);
	m_scroll_area_widget_contents->set_background_color(Qt::transparent);
}
} // namespace kato
