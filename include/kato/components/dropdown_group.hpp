#ifndef KATO_COMPONENTS_DROPDOWN_GROUP_HPP
#define KATO_COMPONENTS_DROPDOWN_GROUP_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <kato/components/clickable_label.hpp>
#include <kato/components/widget.hpp>

namespace kato
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
} // namespace kato

#endif // KATO_COMPONENTS_DROPDOWN_GROUP_HPP
