#ifndef MOZART_COMPONENTS_MIDDLE_CONTENT_HPP
#define MOZART_COMPONENTS_MIDDLE_CONTENT_HPP

#include <QScrollArea>
#include <mozart/components/form.hpp>
#include <mozart/components/header.hpp>
#include <mozart/components/scroll_bar.hpp>

namespace mozart
{
class MiddleContent : public Widget {
	Q_OBJECT

    public:
	explicit MiddleContent(uint64_t id, const QString &name,
			       QWidget *parent = nullptr);

	[[nodiscard]] uint64_t id() const
	{
		return m_id;
	}

	void add_message(const QString &message);

    signals:
	void message_sent(const QString &message);

    private:
	void setup_ui();

	uint64_t m_id{};
	QString m_name;
	QHBoxLayout *m_central_layout{ new QHBoxLayout(this) };
	QVBoxLayout *m_main_layout{ new QVBoxLayout() };
	Header *m_header{};
	QScrollArea *m_scroll_area{ new QScrollArea(this) };
	ScrollBar *m_scroll_bar{ new ScrollBar(Qt::Vertical, this) };
	Widget *m_scroll_area_widget_contents{ new Widget() };
	QVBoxLayout *m_scroll_area_layout{ new QVBoxLayout(
		m_scroll_area_widget_contents) };
	QVBoxLayout *m_messages_layout{ new QVBoxLayout() };
	Form *m_form{ new Form("test", this) };
	QSpacerItem *m_spacer{ new QSpacerItem(0, 0, QSizePolicy::Minimum,
					       QSizePolicy::Expanding) };
};
} // namespace mozart

#endif // MOZART_COMPONENTS_MIDDLE_CONTENT_HPP
