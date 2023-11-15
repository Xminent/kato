#ifndef KATO_COMPONENTS_HOVER_BUTTON_HPP
#define KATO_COMPONENTS_HOVER_BUTTON_HPP

#include <kato/components/widget.hpp>

namespace kato
{
struct HoverButton : Widget {
	Q_OBJECT

    public:
	HoverButton(const QPixmap &pixmap, QWidget *parent = nullptr);

    signals:
	void clicked();

    private:
	void mousePressEvent(QMouseEvent *event) override;
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;

	void setup_ui();

	QPixmap m_pixmap;
	QPixmap m_pixmap_hovered;
};
} // namespace kato

#endif // KATO_COMPONENTS_HOVER_BUTTON_HPP
