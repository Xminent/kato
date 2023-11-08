#ifndef NOW_PLAYING_BAR_BUTTON_HPP
#define NOW_PLAYING_BAR_BUTTON_HPP

#include <mozart/components/widget.hpp>

namespace mozart
{
struct HoverButton : Widget {
	Q_OBJECT

    public:
	explicit HoverButton(const QPixmap &pixmap, QWidget *parent = nullptr);

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
} // namespace mozart

#endif // NOW_PLAYING_BAR_BUTTON_HPP
