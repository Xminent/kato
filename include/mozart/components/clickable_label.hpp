#ifndef MOZART_COMPONENTS_CLICKABLE_LABEL_HPP
#define MOZART_COMPONENTS_CLICKABLE_LABEL_HPP

#include <QLabel>
#include <QMouseEvent>

namespace mozart
{
struct ClickableLabel : QLabel {
	Q_OBJECT

    public:
	explicit ClickableLabel(const QString &text = {},
				QWidget *parent = nullptr);

    signals:
	void clicked();
	void left_clicked();
	void right_clicked();
	void on_enter();
	void on_leave();

    private:
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
};
} // namespace mozart

#endif // MOZART_COMPONENTS_CLICKABLE_LABEL_HPP
