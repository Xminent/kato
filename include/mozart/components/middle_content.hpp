#ifndef MOZART_MIDDLE_CONTENT_HPP
#define MOZART_MIDDLE_CONTENT_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <mozart/components/scroll_bar.hpp>
#include <mozart/components/widget.hpp>

namespace mozart
{
class MiddleContent : public QWidget {
	Q_OBJECT

    public:
	explicit MiddleContent(QWidget *parent = nullptr);

    private:
	void setup_ui();

	QHBoxLayout *m_central_layout{ new QHBoxLayout(this) };

	QVBoxLayout *m_main_layout{ new QVBoxLayout() };
	Widget *m_top_half{ new Widget(this) };
	QScrollArea *m_scroll_area{ new QScrollArea(this) };
	ScrollBar *m_scroll_bar{ new ScrollBar(Qt::Vertical, this) };
	Widget *m_scroll_area_widget_contents{ new Widget() };
	QVBoxLayout *m_scroll_area_layout{ new QVBoxLayout(
		m_scroll_area_widget_contents) };
	Widget *m_bottom_half{ new Widget(this) };
	QSpacerItem *m_spacer{ new QSpacerItem(0, 0, QSizePolicy::Minimum,
					       QSizePolicy::Expanding) };
};
} // namespace mozart

#endif // MOZART_MIDDLE_CONTENT_HPP
