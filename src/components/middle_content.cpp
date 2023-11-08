#include <QDebug>
#include <QUrl>
#include <mozart/components/middle_content.hpp>

namespace mozart
{
MiddleContent::MiddleContent(QWidget *parent)
	: QWidget{ parent }
{
	setup_ui();
}

void MiddleContent::setup_ui()
{
	m_central_layout->addItem(m_main_layout);
	m_central_layout->setContentsMargins(0, 0, 0, 0);
	m_central_layout->setSpacing(4);
	m_central_layout->setStretch(0, 1);
	m_central_layout->setStretch(1, 3);

	m_main_layout->addWidget(m_top_half);
	m_main_layout->addWidget(m_scroll_area);
	m_main_layout->addWidget(m_bottom_half);
	m_main_layout->setContentsMargins(0, 0, 0, 0);
	m_main_layout->setSpacing(0);
	m_main_layout->setStretch(0, 2);
	m_main_layout->setStretch(1, 30);
	m_main_layout->setStretch(2, 1);

	m_scroll_area->setWidget(m_scroll_area_widget_contents);
	m_scroll_area->setWidgetResizable(true);
	m_scroll_area->setFrameShape(QFrame::NoFrame);
	m_scroll_area->setVerticalScrollBar(m_scroll_bar);

	m_scroll_area_layout->addSpacerItem(m_spacer);

	m_scroll_area_widget_contents->set_background_color(QColor(18, 18, 18));

	m_top_half->set_background_color(QColor(230, 175, 65));
	m_top_half->set_border_radius(15, 15, 0, 0);
	m_top_half->set_text("Results");
	m_top_half->set_text_color(Qt::white);

	auto font = m_top_half->font();
	font.setPointSize(16);
	font.setBold(true);
	m_top_half->setFont(font);

	m_bottom_half->set_background_color(QColor(230, 175, 65));
	m_bottom_half->set_border_radius(0, 0, 15, 15);
}
} // namespace mozart
