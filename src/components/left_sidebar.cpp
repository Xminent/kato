#include <QDebug>
#include <QWheelEvent>
#include <kato/components/left_sidebar.hpp>

namespace kato
{
LeftSidebar::LeftSidebar(const QString &name, QWidget *parent)
	: QScrollArea{ parent }
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
}

void LeftSidebar::set_channels(const std::map<uint64_t, QString> &channels)
{
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
}

void LeftSidebar::setup_ui()
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
	m_layout->addWidget(m_text_channels);
	m_layout->addLayout(m_channel_layout);
	m_layout->addSpacerItem(new QSpacerItem(0, 16, QSizePolicy::Minimum,
						QSizePolicy::Fixed));
	m_layout->addWidget(m_voice_channels);
	m_layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum,
						QSizePolicy::Expanding));
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
	m_scroll_bar->scroll(e->angleDelta().y());
}
} // namespace kato
