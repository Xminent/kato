#ifndef KATO_COMPONENTS_LEFT_SIDEBAR_HPP
#define KATO_COMPONENTS_LEFT_SIDEBAR_HPP

#include <QScrollArea>
#include <kato/components/channel_item.hpp>
#include <kato/components/dropdown_group.hpp>
#include <kato/components/header.hpp>
#include <kato/components/smooth_scroll_bar.hpp>

namespace kato
{
struct MiddleContent;

struct LeftSidebar : Widget {
	Q_OBJECT

    public:
	explicit LeftSidebar(const QString &name, QWidget *parent = nullptr);

	void set_channels(const std::map<uint64_t, QString> &channels);

    signals:
	void channel_selected(ChannelItem *channel);
	void create_channel();

    private:
	void setup_ui();
	void add_channel(ChannelItem *channel);
	void wheelEvent(QWheelEvent *e) override;

	QString m_name;
	QVBoxLayout *m_layout{ new QVBoxLayout(this) };
	Header *m_header{};
	QScrollArea *m_scroll_area{ new QScrollArea(this) };
	/// ScrollArea needs a content widget.
	QWidget *m_content{ new QWidget(this) };
	/// Used to store the items vertically.
	QVBoxLayout *m_item_layout{ new QVBoxLayout(m_content) };

	/// The currently focused channel.
	std::optional<int> m_channel_idx;
	QVBoxLayout *m_channel_layout{ new QVBoxLayout() };
	DropdownGroup *m_text_channels{ new DropdownGroup("TEXT CHANNELS",
							  this) };
	DropdownGroup *m_voice_channels{ new DropdownGroup("VOICE CHANNELS",
							   this) };

	/// Used to store the scroll bar.
	SmoothScrollBar *m_scroll_bar{ new SmoothScrollBar(Qt::Vertical,
							   this) };
};
} // namespace kato

#endif // KATO_COMPONENTS_LEFT_SIDEBAR_HPP
