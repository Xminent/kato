#ifndef MOZART_COMPONENTS_SCROLL_BAR_HPP
#define MOZART_COMPONENTS_SCROLL_BAR_HPP

#include <QScrollBar>

namespace mozart
{
class ScrollBar : public QScrollBar {
	Q_OBJECT

    public:
	explicit ScrollBar(Qt::Orientation orientation,
			   QWidget *parent = nullptr);
};
} // namespace mozart

#endif // MOZART_COMPONENTS_SCROLL_BAR_HPP
