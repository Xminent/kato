#ifndef KATO_COMPONENTS_SEARCH_BAR_HPP
#define KATO_COMPONENTS_SEARCH_BAR_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <kato/components/widget.hpp>

namespace kato
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
} // namespace kato

#endif // KATO_COMPONENTS_SEARCH_BAR_HPP
