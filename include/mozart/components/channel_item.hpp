#ifndef MOZART_COMPONENTS_CHANNEL_ITEM_HPP
#define MOZART_COMPONENTS_CHANNEL_ITEM_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <mozart/components/widget.hpp>

namespace mozart
{
struct ChannelItem : Widget {
	Q_OBJECT

    public:
	explicit ChannelItem(uint64_t id, const QString &name,
			     QWidget *parent = nullptr);

	[[nodiscard]] uint64_t id() const
	{
		return m_id;
	}

	void set_clicked(bool clicked);

    signals:
	void left_clicked(ChannelItem *channel);

    private:
	void setup_ui();
	void enterEvent(QEvent *e) override;
	void leaveEvent(QEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;

	// Widgets

	/// Generic horizontal layout.
	QHBoxLayout *layout{ new QHBoxLayout(this) };
	/// Widget for holding the channel type icon.
	Widget *icon{ new Widget(this) };
	/// Label for holding the channel name.
	QLabel *label{ new QLabel(this) };
	/// Spacer for keeping items aligned.
	QSpacerItem *spacer{ new QSpacerItem(0, 0, QSizePolicy::Expanding,
					     QSizePolicy::Minimum) };

	// Metadata

	uint64_t m_id{};
	QString m_name;
	bool m_is_private{};
	bool m_clicked{};
	bool m_change_when_clicked{ true };
};
} // namespace mozart

#endif // MOZART_COMPONENTS_CHANNEL_ITEM_HPP
