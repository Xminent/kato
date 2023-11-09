#ifndef MOZART_COMPONENTS_MESSAGE_ITEM_HPP
#define MOZART_COMPONENTS_MESSAGE_ITEM_HPP

#include "mozart/components/clickable_label.hpp"
#include <mozart/components/widget.hpp>
#include <qboxlayout.h>
#include <qdatetime.h>
#include <qlabel.h>

namespace mozart
{
struct MessageItem : Widget {
	Q_OBJECT

    public:
	explicit MessageItem(const QString &author, const QString &message,
			     QWidget *parent = nullptr);

    private:
	void setup_ui();
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;

	QHBoxLayout *m_layout{ new QHBoxLayout(this) };
	QVBoxLayout *m_vertical_layout{ new QVBoxLayout() };
	QHBoxLayout *m_horizontal_layout{ new QHBoxLayout() };

	QString m_avatar;
	QString m_author;
	QDateTime m_date;
	QString m_message;

	Widget *m_avatar_label{};
	ClickableLabel *m_author_label{};
	QLabel *m_date_label{};
	QLabel *m_message_label{};
};
} // namespace mozart

#endif // MOZART_COMPONENTS_MESSAGE_ITEM_HPP
