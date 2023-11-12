#ifndef COMPONENTS_NAVBAR_HPP
#define COMPONENTS_NAVBAR_HPP

#include <QScrollArea>
#include <kato/components/navbar_item.hpp>
#include <kato/components/smooth_scroll_bar.hpp>

namespace kato
{
struct NavBar : QScrollArea {
	Q_OBJECT

    public:
	explicit NavBar(QWidget *parent = nullptr);

	void add_item(NavBarItem *nav);

    signals:
	void home_clicked();

    private:
	void setup_ui();
	void wheelEvent(QWheelEvent *e) override;

	/// ScrollArea needs a content widget.
	QWidget *m_content{ new QWidget(this) };
	/// Used to store the items vertically.
	QVBoxLayout *m_layout{ new QVBoxLayout(m_content) };
	QVBoxLayout *m_icon_layout{ new QVBoxLayout() };
	NavBarItem *m_home_button{ new NavBarItem(std::nullopt, this) };
	/// Used to store the scroll bar.
	SmoothScrollBar *m_scroll_bar{ new SmoothScrollBar(Qt::Vertical,
							   m_content) };
};
} // namespace kato

#endif // COMPONENTS_NAVBAR_HPP
