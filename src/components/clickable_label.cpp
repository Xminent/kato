#include <mozart/components/clickable_label.hpp>

namespace mozart
{
ClickableLabel::ClickableLabel(QWidget *parent)
	: QLabel(parent)
{
}

ClickableLabel::ClickableLabel(const QString &text, QWidget *parent)
	: QLabel(text, parent)
{
}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		emit left_clicked();
	} else if (event->button() == Qt::RightButton) {
		emit right_clicked();
	}

	emit clicked();
}
} // namespace mozart
