#ifndef KATO_COMPONENTS_CLICKABLE_WIDGET_HPP
#define KATO_COMPONENTS_CLICKABLE_WIDGET_HPP

#include <QMouseEvent>
#include <kato/components/widget.hpp>

namespace kato
{
struct ClickableWidget : Widget {
	Q_OBJECT

    public:
	explicit ClickableWidget(QWidget *parent = nullptr);

    signals:
	void clicked();
	void left_clicked();
	void right_clicked();
	void on_enter();
	void on_leave();

    private:
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
};
} // namespace kato

#endif // KATO_COMPONENTS_CLICKABLE_WIDGET_HPP
