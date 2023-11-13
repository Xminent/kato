#ifndef KATO_COMPONENTS_MIDDLE_CONTENT_HPP
#define KATO_COMPONENTS_MIDDLE_CONTENT_HPP

#include "kato/components/smooth_scroll_bar.hpp"
#include <QDateTime>
#include <QScrollArea>
#include <kato/components/form.hpp>
#include <kato/components/header.hpp>
#include <kato/components/scroll_bar.hpp>

namespace kato
{
struct MiddleContent : Widget {
	Q_OBJECT

    public:
	explicit MiddleContent(uint64_t channel_id, const QString &channel_name,
			       QWidget *parent = nullptr);

	[[nodiscard]] uint64_t channel_id() const
	{
		return m_channel_id;
	}

	void add_message(uint64_t id, const QString &avatar,
			 const QString &author, const QString &message,
			 const QDateTime &date = QDateTime::currentDateTime());

    signals:
	void message_sent(const QString &message,
			  const std::function<void()> &on_sent);

    private:
	void setup_ui();
	void wheelEvent(QWheelEvent *event) override;

	uint64_t m_channel_id{};
	QString m_channel_name;
	QHBoxLayout *m_central_layout{ new QHBoxLayout(this) };
	QVBoxLayout *m_main_layout{ new QVBoxLayout() };
	Header *m_header{};
	bool m_scroll_to_bottom{};
	QScrollArea *m_scroll_area{ new QScrollArea(this) };
	SmoothScrollBar *m_scroll_bar{ new SmoothScrollBar(Qt::Vertical,
							   this) };
	Widget *m_scroll_area_widget_contents{ new Widget() };
	QVBoxLayout *m_scroll_area_layout{ new QVBoxLayout(
		m_scroll_area_widget_contents) };
	QVBoxLayout *m_messages_layout{ new QVBoxLayout() };
	Form *m_form{ new Form("test", this) };
	QSpacerItem *m_spacer{ new QSpacerItem(0, 0, QSizePolicy::Minimum,
					       QSizePolicy::Expanding) };
};
} // namespace kato

#endif // KATO_COMPONENTS_MIDDLE_CONTENT_HPP
