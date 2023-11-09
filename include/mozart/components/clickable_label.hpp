#ifndef MOZART_COMPONENTS_CLICKABLE_LABEL_HPP
#define MOZART_COMPONENTS_CLICKABLE_LABEL_HPP

#include <QLabel>
#include <QMouseEvent>

namespace mozart
{
struct ClickableLabel : QLabel {
	Q_OBJECT

    public:
	struct Args {
		QString text;
		std::function<void()> on_enter;
		std::function<void()> on_leave;
	};

	explicit ClickableLabel(const Args &args = {},
				QWidget *parent = nullptr);

    signals:
	void clicked();
	void left_clicked();
	void right_clicked();

    private:
	std::function<void()> m_on_enter;
	std::function<void()> m_on_leave;

	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
};
} // namespace mozart

#endif // MOZART_COMPONENTS_CLICKABLE_LABEL_HPP
