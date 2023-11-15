#include <kato/components/smooth_scroll_bar.hpp>

namespace kato
{
SmoothScrollBar::SmoothScrollBar(Qt::Orientation orientation, QWidget *parent)
	: QScrollBar{ orientation, parent }
{
	m_scroll_animation->setParent(parent);
	// Set animation curves , stay Qt There is a detailed introduction in the document
	m_scroll_animation->setEasingCurve(QEasingCurve::OutCubic);
	// Set animation time , The smaller the value, the faster the playback
	m_scroll_animation->setDuration(5000);

	setStyleSheet(
		"QScrollBar:vertical {"
		"    width: 4px;"
		"    background: #121212;"
		"    margin: 0px 0px 0px 0px;"
		"    border: 1px transparent #2A2929;"
		"    border-radius: 2px;"
		"}"
		"QScrollBar::handle:vertical {"
		"    background: #2A2929;"
		"    min-height: 0px;"
		"    border-radius: 2px;"
		"}"
		"QScrollBar::add-line:vertical {"
		"    height: 0px;"
		"    subcontrol-position: bottom;"
		"    subcontrol-origin: margin;"
		"}"
		"QScrollBar::sub-line:vertical {"
		"    height: 0 px;"
		"    subcontrol-position: top;"
		"    subcontrol-origin: margin;"
		"}"
		"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
		"    background: none;"
		"}"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
		"    background: none;"
		"}"
		"QScrollBar:horizontal {"
		"    height: 4px;"
		"    background: #121212;"
		"    margin: 0px 0px 0px 0px;"
		"    border: 1px transparent #2A2929;"
		"    border-radius: 2px;"
		"}"
		"QScrollBar::handle:horizontal {"
		"    background: #2A2929;"
		"    min-height: 0px;"
		"    border-radius: 2px;"
		"}"
		"QScrollBar::add-line:horizontal {"
		"    height: 0px;"
		"    subcontrol-position: bottom;"
		"    subcontrol-origin: margin;"
		"}"
		"QScrollBar::sub-line:horizontal {"
		"    height: 0 px;"
		"    subcontrol-position: top;"
		"    subcontrol-origin: margin;"
		"}"
		"QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal {"
		"    background: none;"
		"}"
		"QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {"
		"    background: none;"
		"}");
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
} // namespace kato
