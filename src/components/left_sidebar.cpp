#include <QWheelEvent>
#include <mozart/components/left_sidebar.hpp>
#include <qnamespace.h>

namespace mozart
{
LeftSidebar::LeftSidebar(const QString &name, QWidget *parent)
	: QScrollArea{ parent }
	, m_name{ name }
	, m_header{ new Header{ m_name, std::nullopt, this } }
{
	setup_ui();
}

void LeftSidebar::setup_ui()
{
	setFrameShape(Shape::NoFrame);
	setFixedWidth(240);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);
	setWidget(m_content);
	setWidgetResizable(true);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_scroll_bar->setOrientation(Qt::Vertical);
	setVerticalScrollBar(m_scroll_bar);
	m_scroll_bar->setStyleSheet(
		"QScrollBar:vertical {"
		"    width: 8px;"
		"    background: #121212;"
		"    margin: 0px 0px 0px 0px;"
		"    border: 1px transparent #2A2929;"
		"    border-radius: 4px;"
		"}"
		"QScrollBar::handle:vertical {"
		"    background: #2A2929;"
		"    min-height: 0px;"
		"    border-radius: 4px;"
		"}"
		"QScrollBar::add-line:vertical {"
		"    height: 0px;"
		"    subcontrol-position: bottom;"
		"    subcontrol-origin: margin;"
		"}"
		"QScrollBar::sub-line:vertical {"
		"    height: 0 px;"
		"    subcontrol-position: top;"
		"    subcontrol-origin: margin;"
		"}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
		"    background: none;"
		"}"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
		"    background: none;"
		"}"
		"QScrollBar:horizontal {"
		"    height: 8px;"
		"    background: #121212;"
		"    margin: 0px 0px 0px 0px;"
		"    border: 1px transparent #2A2929;"
		"    border-radius: 4px;"
		"}"
		"QScrollBar::handle:horizontal {"
		"    background: #2A2929;"
		"    min-height: 0px;"
		"    border-radius: 4px;"
		"}"
		"QScrollBar::add-line:horizontal {"
		"    height: 0px;"
		"    subcontrol-position: bottom;"
		"    subcontrol-origin: margin;"
		"}"
		"QScrollBar::sub-line:horizontal {"
		"    height: 0 px;"
		"    subcontrol-position: top;"
		"    subcontrol-origin: margin;"
		"}"
		"QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal {"
		"    background: none;"
		"}"
		"QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {"
		"    background: none;"
		"}"
		"}");

	m_layout->addWidget(m_header);
	m_layout->addLayout(m_channel_layout);
	// spacer to keep things at the top.
	m_layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum,
						QSizePolicy::Expanding));

	// TODO: Remove debug
	for (int i{}; i < 10; i++) {
		auto *c = new ChannelItem(QString::number(i), this);
		add_channel(c);
	}
}

void LeftSidebar::add_channel(ChannelItem *channel)
{
	connect(channel, &ChannelItem::left_clicked, this,
		[this](const ChannelItem *c) {
			for (int i{}; i < m_channel_layout->count(); i++) {
				if (auto *item = qobject_cast<ChannelItem *>(
					    m_channel_layout->itemAt(i)
						    ->widget());
				    item != nullptr && item != c) {
					item->set_clicked(false);
				}
			}
		});

	m_channel_layout->addWidget(channel);
}

void LeftSidebar::wheelEvent(QWheelEvent *e)
{
	// When an event is captured , Call the slot function of relative scrolling
	m_scroll_bar->scroll(e->angleDelta().y());
}
} // namespace mozart
