#ifndef MOZART_COMPONENTS_FORM_HPP
#define MOZART_COMPONENTS_FORM_HPP

#include <mozart/components/widget.hpp>
#include <qboxlayout.h>
#include <qplaintextedit.h>

namespace mozart
{
struct Form : Widget {
	Q_OBJECT

    public:
	explicit Form(const QString &name, QWidget *parent = nullptr);

    signals:
	void message_sent(const QString &message);

    private:
	void setup_ui();
	bool eventFilter(QObject *obj, QEvent *event) override;

	QString m_name;
	QString m_message;
	QHBoxLayout *m_layout{ new QHBoxLayout(this) };
	QVBoxLayout *m_vertical_layout{ new QVBoxLayout() };
	// Input field.
	QPlainTextEdit *m_input{ new QPlainTextEdit(this) };
};
} // namespace mozart

#endif // MOZART_COMPONENTS_FORM_HPP
