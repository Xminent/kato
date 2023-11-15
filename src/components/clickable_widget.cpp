#include <kato/components/clickable_widget.hpp>

namespace kato
{
ClickableWidget::ClickableWidget(QWidget *parent)
	: Widget{ parent }
{
	// set_text_color(palette().color(QPalette::Text));
	// set_text(text);
}

void ClickableWidget::enterEvent(QEvent *event)
{
	emit on_enter();
}

void ClickableWidget::leaveEvent(QEvent *event)
{
	emit on_leave();
}

void ClickableWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		emit left_clicked();
	} else if (event->button() == Qt::RightButton) {
		emit right_clicked();
	}

	emit clicked();
	QWidget::mousePressEvent(event);
}
} // namespace kato
