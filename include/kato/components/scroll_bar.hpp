#ifndef KATO_COMPONENTS_SCROLL_BAR_HPP
#define KATO_COMPONENTS_SCROLL_BAR_HPP

#include <QScrollBar>

namespace kato
{
class ScrollBar : public QScrollBar {
	Q_OBJECT

    public:
	ScrollBar(Qt::Orientation orientation, QWidget *parent = nullptr);
};
} // namespace kato

#endif // KATO_COMPONENTS_SCROLL_BAR_HPP
