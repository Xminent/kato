#include <QMouseEvent>
#include <mozart/components/win_button.hpp>

namespace mozart
{
WinButton::WinButton(const QPixmap &pixmap, QWidget *parent)
	: Widget{ parent }
	, m_pixmap{ pixmap }
{
	setup_ui();
}

void WinButton::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		emit left_clicked();
	}
}

void WinButton::enterEvent([[maybe_unused]] QEvent *event)
{
	set_background_color(QColor{ 70, 70, 70 });
}

void WinButton::leaveEvent([[maybe_unused]] QEvent *event)
{
	set_background_color(Qt::black);
}

void WinButton::setup_ui()
{
	setFixedSize(46, 32);
	set_background_color(Qt::black);
	set_pixmap(m_pixmap);
}
} // namespace mozart
