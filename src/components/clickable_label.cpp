#include <mozart/components/clickable_label.hpp>

namespace mozart
{
ClickableLabel::ClickableLabel(const Args &args, QWidget *parent)
	: QLabel(args.text, parent)
	, m_on_enter{ args.on_enter }
	, m_on_leave{ args.on_leave }
{
}

void ClickableLabel::enterEvent(QEvent *event)
{
	if (m_on_enter) {
		m_on_enter();
	}
}

void ClickableLabel::leaveEvent(QEvent *event)
{
	if (m_on_leave) {
		m_on_leave();
	}
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
