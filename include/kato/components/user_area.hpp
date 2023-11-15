#ifndef KATO_COMPONENTS_USER_AREA_HPP
#define KATO_COMPONENTS_USER_AREA_HPP

#include <QHBoxLayout>
#include <kato/components/clickable_label.hpp>
#include <kato/components/clickable_widget.hpp>

namespace kato
{
struct UserArea : Widget {
	Q_OBJECT

    public:
	explicit UserArea(const QString &avatar, const QString &name,
			  QWidget *parent = nullptr);

    private:
	void setup_ui();

	QHBoxLayout *m_layout{ new QHBoxLayout(this) };
	ClickableWidget *m_avatar_wrapper{ new ClickableWidget(this) };
	QHBoxLayout *m_avatar_layout{ new QHBoxLayout(m_avatar_wrapper) };
	ClickableWidget *m_avatar{ new ClickableWidget(this) };
	ClickableLabel *m_name_label{};

	bool m_is_muted{};
	ClickableWidget *m_mic_mute_wrapper{ new ClickableWidget(this) };
	QHBoxLayout *m_mic_mute_layout{ new QHBoxLayout(m_mic_mute_wrapper) };
	ClickableWidget *m_mic_mute{ new ClickableWidget(m_mic_mute_wrapper) };

	bool m_is_deafened{};
	ClickableWidget *m_speaker_mute_wrapper{ new ClickableWidget(this) };
	QHBoxLayout *m_speaker_mute_layout{ new QHBoxLayout(
		m_speaker_mute_wrapper) };
	ClickableWidget *m_speaker_mute{ new ClickableWidget(
		m_speaker_mute_wrapper) };
};
} // namespace kato

#endif // KATO_COMPONENTS_USER_AREA_HPP
