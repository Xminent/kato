#ifndef MOZART_COMPONENTS_SMOOTH_SCROLL_BAR_HPP
#define MOZART_COMPONENTS_SMOOTH_SCROLL_BAR_HPP

#include <QPropertyAnimation>
#include <QScrollBar>

namespace mozart
{
struct SmoothScrollBar : QScrollBar {
	Q_OBJECT

    public:
	explicit SmoothScrollBar(QWidget *parent = nullptr);

    private:
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;

	QPropertyAnimation *m_scroll_animation{ new QPropertyAnimation(
		this, "value") };
	int m_target_value{ value() };

    public slots:
	void set_value(int value) const;
	void scroll(int value);
};
} // namespace mozart

#endif // MOZART_COMPONENTS_SMOOTH_SCROLL_BAR_HPP
