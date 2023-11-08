#ifndef MOZART_COMPONENTS_CLICKABLE_LABEL_HPP
#define MOZART_COMPONENTS_CLICKABLE_LABEL_HPP

#include <QLabel>
#include <QMouseEvent>

namespace mozart
{
class ClickableLabel : public QLabel {
	Q_OBJECT
    public:
	explicit ClickableLabel(QWidget *parent = nullptr);
	explicit ClickableLabel(const QString &text, QWidget *parent = nullptr);

    signals:
	void clicked();
	void left_clicked();
	void right_clicked();

    protected:
	void mousePressEvent(QMouseEvent *event) override;
};
} // namespace mozart

#endif // MOZART_COMPONENTS_CLICKABLE_LABEL_HPP
