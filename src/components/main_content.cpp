#include <mozart/components/main_content.hpp>

namespace mozart
{
MainContent::MainContent(QWidget *parent)
	: Widget{ parent }
{
	setup_ui();
}

void MainContent::setup_ui()
{
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
