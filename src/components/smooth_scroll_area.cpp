#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QQueue>
#include <QScrollBar>
#include <QWheelEvent>
#include <QtMath>
#include <kato/components/smooth_scroll_area.hpp>

namespace kato
{
SmoothScrollArea::SmoothScrollArea(QWidget *parent)
	: QScrollArea(parent)
{
	connect(smooth_move_timer, SIGNAL(timeout()), this,
		SLOT(slotSmoothMove()));

	m_acceleration = 5;
	m_smallStepModifier = Qt::SHIFT;
	m_small_step_ratio = 1.0 / 5.0;
	m_bigStepModifier = Qt::ALT;
	m_big_step_ratio = 5.0;
}

void SmoothScrollArea::wheelEvent(QWheelEvent *e)
{
	if (m_smooth_mode == NO_SMOOTH) {
		QScrollArea::wheelEvent(e);
		return;
	}

	// According to my experiment, a normal person is able to scroll his wheel
	// at the speed about 36 times per second in average.  Here we use a
	// conservative value 30: a user can achieve the maximum acceration when he
	// scrools his wheel at 30 times / second.
	static QQueue<qint64> scrollStamps;
	qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
	scrollStamps.enqueue(now);

	qDebug() << "ScrollStamps: " << scrollStamps;

	while (now - scrollStamps.front() > 500) {
		scrollStamps.dequeue();
	}

	double acceration_ratio = qMin(scrollStamps.size() / 15.0, 1.0);

	if (lastWheelEvent == nullptr) {
		lastWheelEvent = new QWheelEvent(*e);
	} else {
		*lastWheelEvent = *e;
	}

	stepsTotal = m_fps * m_duration / 1000;
	double multiplier = 1.0;

	if ((QApplication::keyboardModifiers() & m_smallStepModifier) != 0) {
		multiplier *= m_small_step_ratio;
	}

	if ((QApplication::keyboardModifiers() & m_bigStepModifier) != 0) {
		multiplier *= m_big_step_ratio;
	}

	double delta = e->delta() * multiplier;

	if (m_acceleration > 0) {
		delta += delta * m_acceleration * acceration_ratio;
	}

	steps_left_queue.push_back(qMakePair(delta, stepsTotal));
	smooth_move_timer->start(1000 / m_fps);
}

void SmoothScrollArea::slotSmoothMove()
{
	double totalDelta = 0;

	for (auto &it : steps_left_queue) {
		totalDelta += sub_delta(it.first, it.second);
		--(it.second);
	}

	while (!steps_left_queue.empty() &&
	       steps_left_queue.begin()->second == 0) {
		steps_left_queue.pop_front();
	}

	Qt::Orientation orientation = lastWheelEvent->orientation();
	// By default, when you press ALT, QT will scroll horizontally.  But if we
	// have defined the use of ALT key, we ignore this setting since horizontal
	// scroll is not so useful in okular
	if (((m_bigStepModifier & Qt::ALT) != 0) ||
	    ((m_smallStepModifier & Qt::ALT) != 0)) {
		orientation = Qt::Vertical;
	}

	QWheelEvent e(lastWheelEvent->pos(), lastWheelEvent->globalPos(),
		      qRound(totalDelta), lastWheelEvent->buttons(), nullptr,
		      orientation);

	if (e.orientation() == Qt::Horizontal) {
		QApplication::sendEvent(horizontalScrollBar(), &e);
	} else {
		QApplication::sendEvent(verticalScrollBar(), &e);
	}

	if (steps_left_queue.empty()) {
		smooth_move_timer->stop();
	}
}

double SmoothScrollArea::sub_delta(double delta, int steps_left) const
{
	Q_ASSERT(m_smooth_mode != NO_SMOOTH);

	double m = stepsTotal / 2.0;
	double x = abs(stepsTotal - steps_left - m);

	// some mathmatical integral result.
	switch (m_smooth_mode) {
	case NO_SMOOTH:
		return 0;
		break;

	case CONSTANT:
		return double(delta) / stepsTotal;
		break;

	case LINEAR:
		return 2.0 * delta / stepsTotal * (m - x) / m;
		break;

	case QUADRATIC:
		return 3.0 / 4.0 / m * (1.0 - x * x / m / m) * delta;
		break;

	case COSINE:
		return (cos(x * M_PI / m) + 1.0) / (2.0 * m) * delta;
		break;
	}

	return 0;
}
} // namespace kato
