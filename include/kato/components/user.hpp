#ifndef KATO_COMPONENTS_USER_HPP
#define KATO_COMPONENTS_USER_HPP

#include <QHBoxLayout>
#include <kato/components/widget.hpp>

namespace kato
{
struct User : Widget {
	Q_OBJECT

    public:
	explicit User(const QString &avatar, const QString &name,
		      QWidget *parent = nullptr);

    signals:
	void left_clicked(User *user);

    private:
	void setup_ui();
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;

	QHBoxLayout *m_layout{ new QHBoxLayout(this) };
	Widget *m_avatar{ new Widget(this) };
	Widget *m_name{ new Widget(this) };
};
} // namespace kato

#endif // KATO_COMPONENTS_USER_HPP
