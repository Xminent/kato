#ifndef MOZART_COMPONENTS_SEARCH_BAR_HPP
#define MOZART_COMPONENTS_SEARCH_BAR_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <mozart/components/widget.hpp>

namespace mozart
{
struct SearchBar : Widget {
	Q_OBJECT

    public:
	explicit SearchBar(QWidget *parent = nullptr);

	[[nodiscard]] QString text() const
	{
		return m_search_input->text();
	}

    signals:
	void return_pressed();

    private:
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void focusInEvent(QFocusEvent *event) override;
	void focusOutEvent(QFocusEvent *event) override;
	void setup_ui();

	QHBoxLayout *m_central_layout{ new QHBoxLayout(this) };
	QLabel *m_label{ new QLabel(this) };
	QLineEdit *m_search_input{ new QLineEdit(this) };
};
} // namespace mozart

#endif // MOZART_COMPONENTS_SEARCH_BAR_HPP
