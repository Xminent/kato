#ifndef MOZART_COMPONENTS_NAVBAR_ITEM_HPP
#define MOZART_COMPONENTS_NAVBAR_ITEM_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <mozart/components/guild_icon.hpp>
#include <mozart/components/widget.hpp>

namespace mozart
{
struct NavBarItem : Widget {
	Q_OBJECT

    public:
	explicit NavBarItem(const std::optional<QString> &name,
			    QWidget *parent = nullptr);

	void unclick();

    signals:
	void left_clicked(NavBarItem *channel);

    private:
	void setup_ui();
	void enterEvent(QEvent *e) override;
	void leaveEvent(QEvent *e) override;

	/// Generic horizontal layout.
	QHBoxLayout *m_layout{ new QHBoxLayout(this) };
	/// Widget which contains the pill.
	QWidget *m_pill_container{ new Widget(this) };
	QHBoxLayout *m_pill_layout{ new QHBoxLayout(m_pill_container) };
	/// Widget for holding the channel type icon.
	Widget *m_pill{ new Widget(this) };
	GuildIcon *m_icon{};
	bool m_clicked{};
};
} // namespace mozart

#endif // MOZART_COMPONENTS_NAVBAR_ITEM_HPP
