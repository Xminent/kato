#include <mozart/components/smooth_scroll_bar.hpp>

namespace mozart
{
SmoothScrollBar::SmoothScrollBar(QWidget *parent)
	: QScrollBar(parent)
{
	m_scroll_animation->setParent(parent);
	// Set animation curves , stay Qt There is a detailed introduction in the document
	m_scroll_animation->setEasingCurve(QEasingCurve::OutCubic);
	// Set animation time , The smaller the value, the faster the playback
	m_scroll_animation->setDuration(0);
}

void SmoothScrollBar::set_value(int value) const
{
	// Stop the current animation , Prevent conflict
	m_scroll_animation->stop();
	// Sets the initial value of the animated scroll to the current position
	m_scroll_animation->setStartValue(this->value());
	// Set the end position of the animation to the target value
	m_scroll_animation->setEndValue(value);
	// Start the animation
	m_scroll_animation->start();
}

void SmoothScrollBar::scroll(int value)
{
	// Calculate the target value and relative position
	m_target_value -= value;
	// Start the animation
	set_value(m_target_value);
}

void SmoothScrollBar::mousePressEvent(QMouseEvent *e)
{
	// When using the mouse to operate the scroll bar , Not refresh m_target_value Value , So you need to rewrite the event , Refresh it .
	m_scroll_animation->stop();
	QScrollBar::mousePressEvent(e);
	m_target_value = value();
}

void SmoothScrollBar::mouseReleaseEvent(QMouseEvent *e)
{
	m_scroll_animation->stop();
	QScrollBar::mouseReleaseEvent(e);
	m_target_value = value();
}

void SmoothScrollBar::mouseMoveEvent(QMouseEvent *e)
{
	m_scroll_animation->stop();
	QScrollBar::mouseMoveEvent(e);
	m_target_value = value();
}
} // namespace mozart
