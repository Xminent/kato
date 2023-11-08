
#ifndef MOZART_API_WIN_KEYBOARD_HOOK_HPP
#define MOZART_API_WIN_KEYBOARD_HOOK_HPP

#include <QAbstractNativeEventFilter>
#include <QObject>

#ifndef Q_OS_WIN
#error "Only Windows supported"
#endif

#include <Windows.h>

namespace mozart
{
class WinKeyboardHook : public QObject, public QAbstractNativeEventFilter {
	Q_OBJECT

    public:
	~WinKeyboardHook() override;

	static WinKeyboardHook &instance()
	{
		static WinKeyboardHook instance;
		return instance;
	}

	bool nativeEventFilter(const QByteArray &event_type, void *message,
			       long *result) override;

    signals:
	void play_pause_pressed();

    private:
	explicit WinKeyboardHook();

	// HHOOK m_hook {};
};
} // namespace mozart

#endif // MOZART_API_WIN_KEYBOARD_HOOK_HPP
