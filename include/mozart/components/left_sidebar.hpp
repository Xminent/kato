#ifndef MOZART_COMPONENTS_LEFT_SIDEBAR_HPP
#define MOZART_COMPONENTS_LEFT_SIDEBAR_HPP

#include "mozart/components/header.hpp"
#include <QScrollArea>
#include <QVBoxLayout>
#include <mozart/components/channel_item.hpp>
#include <mozart/components/smooth_scroll_bar.hpp>

namespace mozart
{
struct LeftSidebar : QScrollArea {
	Q_OBJECT

    public:
	explicit LeftSidebar(const QString &name, QWidget *parent = nullptr);

    signals:
	void channel_selected(ChannelItem *channel);

    private:
	void setup_ui();
	void add_channel(ChannelItem *channel);
	void wheelEvent(QWheelEvent *e) override;

	/// ScrollArea needs a content widget.
	QWidget *m_content{ new QWidget() };
	/// Used to store the items vertically.
	QVBoxLayout *m_layout{ new QVBoxLayout(m_content) };
	QString m_name;
	Header *m_header{};
	QVBoxLayout *m_channel_layout{ new QVBoxLayout() };
	/// Used to store the scroll bar.
	SmoothScrollBar *m_scroll_bar{ new SmoothScrollBar(this) };
};
} // namespace mozart

#endif // MOZART_COMPONENTS_LEFT_SIDEBAR_HPP
