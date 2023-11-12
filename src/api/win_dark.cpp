#include <QApplication>
#include <QPalette>
#include <QSettings>
#include <QStyleFactory>
#include <kato/api/win_dark.hpp>

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