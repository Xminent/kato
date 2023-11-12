#include <QDebug>
#include <QWheelEvent>
#include <kato/components/left_sidebar.hpp>

namespace kato
{
LeftSidebar::LeftSidebar(const QString &name, QWidget *parent)
	: Widget{ parent }
	, m_name{ name }
	, m_header{ new Header{ m_name, std::nullopt, this } }
{
	setup_ui();

	connect(m_text_channels, &DropdownGroup::toggle, this, [this] {
		// Hide all other channels, but the currently focused one.

		for (int i{}; i < m_channel_layout->count(); i++) {
			auto *item = qobject_cast<ChannelItem *>(
				m_channel_layout->itemAt(i)->widget());

			if (item == nullptr ||
			    m_channel_idx && i == m_channel_idx) {
				continue;
			}

			// toggle the show/hide state.
			item->setVisible(!item->isVisible());
		}
	});

	connect(m_text_channels, &DropdownGroup::add, this, [this] {
		qDebug() << "show add channel modal";
		emit create_channel();
	});

	connect(m_scroll_bar, &QScrollBar::rangeChanged, this, [this] {
		bool is_overflowed = m_scroll_area->widget()->height() >
				     m_scroll_area->height();

		m_content_layout->removeItem(m_spacer);

		if (is_overflowed) {
			m_spacer->changeSize(4, 0, QSizePolicy::Fixed,
					     QSizePolicy::Expanding);
		} else {
			m_spacer->changeSize(8, 0, QSizePolicy::Fixed,
					     QSizePolicy::Expanding);
		}

		m_content_layout->addSpacerItem(m_spacer);
	});
}

void LeftSidebar::set_channels(const std::map<uint64_t, QString> &channels)
{
	m_text_channels->set_disabled(true);

	// NOTE: Not as efficient as it could be.
	while (m_channel_layout->count() > 0) {
		auto *item = m_channel_layout->itemAt(0)->widget();

		if (item == nullptr) {
			continue;
		}

		disconnect(item, nullptr, nullptr, nullptr);
		m_channel_layout->removeWidget(item);
		delete item;
	}

	for (const auto &[id, name] : channels) {
		add_channel(new ChannelItem{ id, name, this });
	}

	m_text_channels->set_disabled(false);
}

void LeftSidebar::setup_ui()
{
	setFixedWidth(240);

	m_item_layout->setContentsMargins(0, 0, 0, 0);
	m_item_layout->setSpacing(0);
	m_item_layout->addWidget(m_text_channels);
	m_item_layout->addLayout(m_channel_layout);
	m_item_layout->addSpacerItem(new QSpacerItem(
		0, 16, QSizePolicy::Minimum, QSizePolicy::Fixed));
	m_item_layout->addWidget(m_voice_channels);
	m_item_layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum,
						     QSizePolicy::Expanding));

	m_content_layout->setContentsMargins(0, 0, 0, 0);
	m_content_layout->setSpacing(0);
	m_content_layout->addLayout(m_item_layout);
	m_content_layout->addSpacerItem(m_spacer);

	m_scroll_area->setFrameShape(QScrollArea::Shape::NoFrame);
	m_scroll_area->setWidget(m_content);
	m_scroll_area->setWidgetResizable(true);
	m_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_scroll_area->setVerticalScrollBar(m_scroll_bar);

	m_scroll_bar->setSizePolicy([this] {
		auto sp = m_scroll_bar->sizePolicy();
		sp.setRetainSizeWhenHidden(true);
		return sp;
	}());

	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);
	m_layout->addWidget(m_header);
	m_layout->addWidget(m_scroll_area);
}

void LeftSidebar::add_channel(ChannelItem *channel)
{
	connect(channel, &ChannelItem::left_clicked, this,
		[this](ChannelItem *c) {
			for (int i{}; i < m_channel_layout->count(); i++) {
				if (auto *item = qobject_cast<ChannelItem *>(
					    m_channel_layout->itemAt(i)
						    ->widget());
				    item != nullptr) {
					if (item == c) {
						m_channel_idx = i;
						continue;
					}

					item->set_clicked(false);
				}
			}

			emit channel_selected(c);
		});

	m_channel_layout->addWidget(channel);
}

void LeftSidebar::wheelEvent(QWheelEvent *e)
{
	// When an event is captured , Call the slot function of relative scrolling
	// m_scroll_bar->scroll(e->angleDelta().y());
}

void LeftSidebar::enterEvent(QEvent *e)
{
	m_scroll_bar->setVisible(true);
}

void LeftSidebar::leaveEvent(QEvent *e)
{
	m_scroll_bar->setVisible(false);
}
} // namespace kato
