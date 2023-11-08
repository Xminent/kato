#ifndef MOZART_TITLEBAR_HPP
#define MOZART_TITLEBAR_HPP

#include <mozart/components/clickable_label.hpp>
#include <mozart/components/search_bar.hpp>
#include <mozart/components/win_button.hpp>

namespace mozart
{
struct Titlebar : Widget {
	Q_OBJECT

    public:
	explicit Titlebar(QWidget *parent = nullptr);

    signals:
	void window_minimized();
	void window_maximized();
	void window_restored();
	void window_closed();

    private:
	void setup_ui();

	QHBoxLayout *m_central_layout{ new QHBoxLayout(this) };
	ClickableLabel *m_logo{ new ClickableLabel(this) };
	QLabel *m_title_label{ new QLabel(this) };
	QSpacerItem *m_spacer{ new QSpacerItem(40, 20, QSizePolicy::Expanding,
					       QSizePolicy::Minimum) };
	Widget *m_profile_picture{ new Widget(this) };
	QSpacerItem *m_spacer2{ new QSpacerItem(40, 20, QSizePolicy::Fixed,
						QSizePolicy::Minimum) };
	QSpacerItem *m_spacer3{ new QSpacerItem(20, 20, QSizePolicy::Fixed,
						QSizePolicy::Minimum) };

	// Window buttons
	// TODO: Move this to a separate widget
	bool m_is_maximized{};
	WinButton *m_minimize_button{ new WinButton(
		QPixmap{ ":/icons/minimize.svg" }, this) };
	WinButton *m_maximize_button{ new WinButton(
		QPixmap{ ":/icons/maximize.svg" }, this) };
	WinButton *m_close_button{ new WinButton(QPixmap{ ":/icons/close.svg" },
						 this) };
};
} // namespace mozart

#endif // MOZART_TITLEBAR_HPP
