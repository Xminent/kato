#ifndef KATO_COMPONENTS_RIGHT_SIDEBAR_HPP
#define KATO_COMPONENTS_RIGHT_SIDEBAR_HPP

#include <QScrollArea>
#include <kato/components/channel_item.hpp>
#include <kato/components/dropdown_group.hpp>
#include <kato/components/header.hpp>
#include <kato/components/smooth_scroll_bar.hpp>
#include <kato/components/user.hpp>

namespace kato
{
struct MiddleContent;

struct RightSidebar : QScrollArea {
	Q_OBJECT

    public:
	explicit RightSidebar(QWidget *parent = nullptr);

	void add_user(User *user);

    private:
	void setup_ui();

	/// ScrollArea needs a content widget.
	QWidget *m_content{ new QWidget() };
	/// Used to store the items vertically.
	QVBoxLayout *m_layout{ new QVBoxLayout(m_content) };
	Header *m_header{};

	QVBoxLayout *m_user_layout{ new QVBoxLayout() };

	/// Used to store the scroll bar.
	SmoothScrollBar *m_scroll_bar{ new SmoothScrollBar(Qt::Vertical,
							   this) };
};
} // namespace kato

#endif // KATO_COMPONENTS_RIGHT_SIDEBAR_HPP
