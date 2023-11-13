#ifndef KATO_COMPONENTS_MESSAGE_ITEM_HPP
#define KATO_COMPONENTS_MESSAGE_ITEM_HPP

#include <QBoxLayout>
#include <QDateTime>
#include <kato/components/clickable_label.hpp>
#include <kato/components/widget.hpp>

namespace kato
{
struct MessageItem : Widget {
	Q_OBJECT

    public:
	explicit MessageItem(uint64_t id, const QString &avatar,
			     const QString &author, const QString &message,
			     QWidget *parent = nullptr);

	[[nodiscard]] uint64_t id() const
	{
		return m_id;
	}

    private:
	void setup_ui();
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;

	QHBoxLayout *m_layout{ new QHBoxLayout(this) };
	QVBoxLayout *m_vertical_layout{ new QVBoxLayout() };
	QHBoxLayout *m_horizontal_layout{ new QHBoxLayout() };

	uint64_t m_id{};
	QString m_author;
	QDateTime m_date;
	QString m_message;

	Widget *m_avatar_label{};
	ClickableLabel *m_author_label{};
	QLabel *m_date_label{};
	QLabel *m_message_label{};
};
} // namespace kato

#endif // KATO_COMPONENTS_MESSAGE_ITEM_HPP
