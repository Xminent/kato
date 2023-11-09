#include <mozart/components/clickable_label.hpp>

namespace mozart
{
ClickableLabel::ClickableLabel(const QString &text, QWidget *parent)
	: QLabel(text, parent)
{
}

void ClickableLabel::enterEvent(QEvent *event)
{
	emit on_enter();
}

void ClickableLabel::leaveEvent(QEvent *event)
{
	emit on_leave();
}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		emit left_clicked();
	} else if (event->button() == Qt::RightButton) {
		emit right_clicked();
	}

	emit clicked();
	QLabel::mousePressEvent(event);
}
} // namespace mozart
