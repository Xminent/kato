#include <QMouseEvent>
#include <mozart/components/hover_button.hpp>

namespace mozart
{
HoverButton::HoverButton(const QPixmap &pixmap, QWidget *parent)
	: Widget{ parent }
{
	{
		m_pixmap = pixmap;
		QPainter painter{ &m_pixmap };
		painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
		painter.fillRect(m_pixmap.rect(), QColor{ 181, 186, 193 });
	}

	// Creating the hovered pixmap.
	{
		m_pixmap_hovered = pixmap;
		QPainter painter(&m_pixmap_hovered);
		painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
		painter.fillRect(m_pixmap_hovered.rect(), Qt::white);
	}

	setup_ui();
}

void HoverButton::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		emit clicked();
	}
}

void HoverButton::enterEvent([[maybe_unused]] QEvent *event)
{
	set_pixmap(m_pixmap_hovered);
}

void HoverButton::leaveEvent([[maybe_unused]] QEvent *event)
{
	set_pixmap(m_pixmap);
}

void HoverButton::setup_ui()
{
	setFixedSize(32, 32);
	set_pixmap(m_pixmap);
}
} // namespace mozart
