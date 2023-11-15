#ifndef KATO_COMPONENTS_SMOOTH_SCROLL_AREA_HPP
#define KATO_COMPONENTS_SMOOTH_SCROLL_AREA_HPP

#include <QList>
#include <QPair>
#include <QScrollArea>
#include <QTimer>

namespace kato
{
class SmoothScrollArea : public QScrollArea {
	Q_OBJECT
    public:
	explicit SmoothScrollArea(QWidget *parent = nullptr);

	enum SmoothMode { NO_SMOOTH, CONSTANT, LINEAR, QUADRATIC, COSINE };

    private:
	[[nodiscard]] double sub_delta(double delta, int steps_left) const;
	void wheelEvent(QWheelEvent *e) override;

	QTimer *smooth_move_timer{ new QTimer(this) };
	QWheelEvent *lastWheelEvent{};

	int m_fps{ 240 };
	int m_duration{ 200 };
	SmoothMode m_smooth_mode{ COSINE };

	double m_acceleration{};
	double m_small_step_ratio;
	double m_big_step_ratio;
	Qt::Modifier m_smallStepModifier;
	Qt::Modifier m_bigStepModifier;

	int stepsTotal{};
	QList<QPair<double, int> > steps_left_queue;

    public slots:
	void slotSmoothMove();
};
} // namespace kato

#endif // KATO_COMPONENTS_SMOOTH_SCROLL_AREA_HPP
