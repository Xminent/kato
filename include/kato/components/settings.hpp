#ifndef KATO_COMPONENTS_SETTINGS_HPP
#define KATO_COMPONENTS_SETTINGS_HPP

#include <QColor>
#include <map>

namespace kato
{
struct Settings {
	enum class ColorEnum {
		None,
		White,
		HeaderPrimary,
		HeaderSecondary,
		TextNormal,
		TextMuted,
		InteractiveNormal,
		InteractiveHover,
		InteractiveActive,
		InteractiveMuted,
		BackgroundPrimary,
		BackgroundSecondary,
		BackgroundSecondaryAlt,
		BackgroundTertiary,
		BackgroundFloating,
		BackgroundModifierSelected,
		BackgroundModifierActive,
		BackgroundModifierHover,
		BackgroundModifierAccent,
		BackgroundMessageHover,
		ChannelsDefault,
		ChannelIcon,
		ChanneltextareaBackground,
		ChanneltextareaPlaceholder,
		DeprecatedCardBg,
		ButtonSecondaryBackground,
		ButtonSecondaryBackgroundHover,
		ButtonSecondaryBackgroundActive,
		ButtonDangerBackground,
		ButtonDangerBackgroundHover,
		ButtonDangerBackgroundActive,
		ButtonOutlineDangerBackground,
		ButtonOutlineDangerBackgroundHover,
		ButtonOutlineDangerBackgroundActive,
		ButtonOutlineDangerText,
		ButtonOutlineDangerTextHover,
		ButtonOutlineDangerTextActive,
		ButtonOutlineDangerBorder,
		ButtonOutlineDangerBorderHover,
		ButtonOutlineDangerBorderActive,
		InfoDangerForeground,
		BrandExperiment,
		BrandExperiment560,
		BrandExperiment600,
		StatusOffline,
		StatusOnline,
		StatusIdle,
		StatusDND,
		StatusBackgroundHover,
		StatusBackgroundActive,
		TextPositive,
		UnsavedBackground,
		Black40,
		Black85,
		Black,
		Link,
		Error,
		SwitchActive,
		SwitchInactive,
		RadioBarGreen,
		RadioBarOrange,
		RadioBarRed,
		SettingsButtonTextColor,
		FileSizeColor,
	};

	enum class Theme : uint8_t { Dark, Light, Sync };

	// Has to be called before using settings
	static void init();
	static void save();
	static void setTheme(Theme theme);
	static void setSaturation(int saturation);
	static void applySaturationToCustom(bool applyToCustom);

	static std::map<ColorEnum, QColor> colors;

    private:
	static const std::map<ColorEnum, QColor> dark_colors;
	static const std::map<ColorEnum, QColor> light_colors;
};
} // namespace kato

#endif // KATO_COMPONENTS_SETTINGS_HPP
