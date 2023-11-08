#include <QApplication>
#include <QPalette>
#include <QSettings>
#include <QStyleFactory>
#include <mozart/api/win_dark.hpp>

namespace winDark
{
bool isDarkTheme()
{
#ifndef Q_OS_WIN
	return false;
#endif

	return QSettings(
		       "(HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize)",
		       QSettings::NativeFormat)
		       .value("AppsUseLightTheme") == 0;
}

void setDark_qApp()
{
#ifdef Q_OS_WIN
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	QPalette darkPalette;
	auto darkColor = QColor(25, 25, 25);
	auto disabledColor = QColor(127, 127, 127);
	darkPalette.setColor(QPalette::Window, darkColor);
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(18, 18, 18));
	darkPalette.setColor(QPalette::AlternateBase, darkColor);
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
	darkPalette.setColor(QPalette::Button, darkColor);
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText,
			     disabledColor);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);
	darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText,
			     disabledColor);
	QApplication::setPalette(darkPalette);
	qApp->setStyleSheet(
		"QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
#endif
}

void setDark_Titlebar(HWND hwnd)
{
#ifdef Q_OS_WIN
	auto *hUxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr,
					LOAD_LIBRARY_SEARCH_SYSTEM32);
	auto *hUser32 = GetModuleHandleW(L"user32.dll");
	auto AllowDarkModeForWindow =
		reinterpret_cast<fnAllowDarkModeForWindow>(
			GetProcAddress(hUxtheme, MAKEINTRESOURCEA(133)));
	auto SetPreferredAppMode = reinterpret_cast<fnSetPreferredAppMode>(
		GetProcAddress(hUxtheme, MAKEINTRESOURCEA(135)));
	auto SetWindowCompositionAttribute =
		reinterpret_cast<fnSetWindowCompositionAttribute>(
			GetProcAddress(hUser32,
				       "SetWindowCompositionAttribute"));

	SetPreferredAppMode(AllowDark);
	BOOL dark = TRUE;
	AllowDarkModeForWindow(hwnd, dark);
	WINDOWCOMPOSITIONATTRIBDATA data = { WCA_USEDARKMODECOLORS, &dark,
					     sizeof(dark) };
	SetWindowCompositionAttribute(hwnd, &data);
#endif
}
} // namespace winDark