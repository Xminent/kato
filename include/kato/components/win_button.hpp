#ifndef KATO_WIN_BUTTON_HPP
#define KATO_WIN_BUTTON_HPP

#include <kato/components/widget.hpp>

namespace kato
{
struct WinButton : Widget {
	Q_OBJECT

    public:
	WinButton(const QPixmap &pixmap, QWidget *parent = nullptr);

    signals:
	void left_clicked();

    private:
	void mousePressEvent(QMouseEvent *event) override;
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;

	void setup_ui();

	QPixmap m_pixmap;
};
} // namespace kato

#endif // KATO_WIN_BUTTON_HPP
