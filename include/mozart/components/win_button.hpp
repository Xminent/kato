#ifndef MOZART_WIN_BUTTON_HPP
#define MOZART_WIN_BUTTON_HPP

#include <mozart/components/widget.hpp>

namespace mozart
{
struct WinButton : Widget {
	Q_OBJECT

    public:
	explicit WinButton(const QPixmap &pixmap, QWidget *parent = nullptr);

    signals:
	void left_clicked();

    private:
	void mousePressEvent(QMouseEvent *event) override;
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;

	void setup_ui();

	QPixmap m_pixmap;
};
} // namespace mozart

#endif // MOZART_WIN_BUTTON_HPP
