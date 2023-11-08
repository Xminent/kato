#ifndef MOZART_COMPONENTS_MAIN_CONTENT_HPP
#define MOZART_COMPONENTS_MAIN_CONTENT_HPP

#include <QScrollArea>
#include <QVBoxLayout>
#include <mozart/components/widget.hpp>

namespace mozart
{
class MainContent : public Widget {
	Q_OBJECT

    public:
	explicit MainContent(QWidget *parent = nullptr);

    private:
	void setup_ui();

	QVBoxLayout *m_main_layout{ new QVBoxLayout() };
	Widget *m_top_half{ new Widget(this) };
	QScrollArea *m_scroll_area{ new QScrollArea(this) };
	Widget *m_scroll_area_widget_contents{ new Widget() };
	QVBoxLayout *m_scroll_area_layout{ new QVBoxLayout(
		m_scroll_area_widget_contents) };
	Widget *m_bottom_half{ new Widget(this) };
	QSpacerItem *m_spacer{ new QSpacerItem(0, 0, QSizePolicy::Minimum,
					       QSizePolicy::Expanding) };
};
} // namespace mozart

#endif // MOZART_COMPONENTS_MAIN_CONTENT_HPP
