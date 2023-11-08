#include <QDebug>
#include <mozart/api/win_keyboard_hook.hpp>

// namespace {
// LRESULT CALLBACK keyboard_hook(int code, WPARAM wparam, LPARAM lparam)
// {
//     if (code == HC_ACTION && wparam == WM_KEYDOWN) {
//         const auto* key = reinterpret_cast<KBDLLHOOKSTRUCT*>(lparam);

//         if (key->vkCode == VK_MEDIA_PLAY_PAUSE) {
//             emit mozart::WinKeyboardHook::instance().play_pause_pressed();
//         }
//     }
//     return CallNextHookEx(nullptr, code, wparam, lparam);
// }
// } // namespace

namespace mozart
{
WinKeyboardHook::WinKeyboardHook()
{
}

WinKeyboardHook::~WinKeyboardHook()
{
}

bool WinKeyboardHook::nativeEventFilter(
	[[maybe_unused]] const QByteArray &event_type, void *message,
	[[maybe_unused]] long *result)
{
	if (const auto *msg = reinterpret_cast<MSG *>(message);
	    msg->message == WM_APPCOMMAND) {
		const auto *key = reinterpret_cast<APPBARDATA *>(msg->lParam);

		if (key != nullptr &&
		    key->lParam == APPCOMMAND_MEDIA_PLAY_PAUSE) {
			qDebug() << "Play/Pause pressed";
			emit play_pause_pressed();
		}
	}

	return false;
}
} // namespace mozart
