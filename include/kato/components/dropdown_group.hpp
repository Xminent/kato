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
	DropdownGroup(const QString &text, QWidget *parent = nullptr);

	void set_disabled(bool disabled)
	{
		m_disabled = disabled;
	}

    signals:
	void toggle();
	void add();

    private:
	void setup_ui();
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;

	/// Disabled on start, there are no channels.
	bool m_disabled{ true };
	bool m_collapsed{};
	QHBoxLayout *m_layout{ new QHBoxLayout(this) };
	Widget *m_arrow{ new Widget(this) };
	QLabel *m_text{};
	ClickableLabel *m_icon{};
};
} // namespace kato

#endif // KATO_COMPONENTS_DROPDOWN_GROUP_HPP
