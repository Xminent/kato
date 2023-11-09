#ifndef MOZART_COMPONENTS_DROPDOWN_GROUP_HPP
#define MOZART_COMPONENTS_DROPDOWN_GROUP_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <mozart/components/clickable_label.hpp>
#include <mozart/components/widget.hpp>

namespace mozart
{
struct DropdownGroup : Widget {
	Q_OBJECT

    public:
	explicit DropdownGroup(const QString &text, QWidget *parent = nullptr);

    signals:
	void toggle();
	void add();

    private:
	void setup_ui();
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;

	QHBoxLayout *m_layout{ new QHBoxLayout(this) };
	QLabel *m_arrow{ new QLabel(this) };
	Widget *m_text{};
	ClickableLabel *m_icon{};
};
} // namespace mozart

#endif // MOZART_COMPONENTS_DROPDOWN_GROUP_HPP
