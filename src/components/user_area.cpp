#include <kato/api/image_cache.hpp>
#include <kato/components/user_area.hpp>

namespace kato
{
UserArea::UserArea(const QString &avatar, const QString &name, QWidget *parent)
	: Widget{ parent }
	, m_name_label{ new ClickableLabel{ name, this } }
{
	setup_ui();

	ImageCache::download(QUrl{ avatar }, [this](const QPixmap &pixmap) {
		m_avatar->set_pixmap(pixmap);
	});

	connect(m_avatar_wrapper, &ClickableWidget::on_enter, [this] {
		m_avatar_wrapper->set_background_color(QColor{ 13, 13, 15 });
	});

	connect(m_avatar_wrapper, &ClickableWidget::on_leave, [this] {
		m_avatar_wrapper->set_background_color(Qt::transparent);
	});

	connect(m_mic_mute_wrapper, &ClickableWidget::on_enter, [this] {
		m_mic_mute_wrapper->set_background_color(QColor{ 13, 13, 15 });

		auto pm = m_mic_mute->pixmap();
		QPainter p{ &pm };
		p.setCompositionMode(QPainter::CompositionMode_SourceIn);
		p.fillRect(pm.rect(), Qt::white);
		m_mic_mute->set_pixmap(pm);
	});

	connect(m_mic_mute_wrapper, &ClickableWidget::on_leave, [this] {
		m_mic_mute_wrapper->set_background_color(Qt::transparent);

		auto pm = m_mic_mute->pixmap();
		QPainter p{ &pm };
		p.setCompositionMode(QPainter::CompositionMode_SourceIn);
		p.fillRect(pm.rect(), QColor{ 142, 146, 151 });
		m_mic_mute->set_pixmap(pm);
	});

	connect(m_mic_mute_wrapper, &ClickableWidget::clicked, [this] {
		m_is_muted = !m_is_muted;

		m_mic_mute->set_pixmap(
			m_is_muted ? QPixmap{ ":/icons/muted.svg" } :
				     QPixmap{ ":/icons/unmuted.svg" });
	});

	connect(m_speaker_mute_wrapper, &ClickableWidget::on_enter, [this] {
		m_speaker_mute_wrapper->set_background_color(
			QColor{ 13, 13, 15 });

		auto pm = m_speaker_mute->pixmap();
		QPainter p{ &pm };
		p.setCompositionMode(QPainter::CompositionMode_SourceIn);
		p.fillRect(pm.rect(), Qt::white);
		m_speaker_mute->set_pixmap(pm);
	});

	connect(m_speaker_mute_wrapper, &ClickableWidget::on_leave, [this] {
		m_speaker_mute_wrapper->set_background_color(Qt::transparent);

		auto pm = m_speaker_mute->pixmap();
		QPainter p{ &pm };
		p.setCompositionMode(QPainter::CompositionMode_SourceIn);
		p.fillRect(pm.rect(), QColor{ 142, 146, 151 });
		m_speaker_mute->set_pixmap(pm);
	});

	connect(m_speaker_mute_wrapper, &ClickableWidget::clicked, [this] {
		m_is_deafened = !m_is_deafened;

		m_speaker_mute->set_pixmap(
			m_is_deafened ? QPixmap{ ":/icons/deafened.svg" } :
					QPixmap{ ":/icons/undeafened.svg" });
	});
}

void UserArea::setup_ui()
{
	setFixedSize(240, 52);

	m_avatar_wrapper->setFixedHeight(42);
	m_avatar_wrapper->set_border_radius(4);
	m_avatar->setFixedSize(32, 32);
	m_avatar->set_border_radius(16);
	m_avatar_layout->setContentsMargins(8, 0, 0, 0);
	m_avatar_layout->setSpacing(8);
	m_avatar_layout->addWidget(m_avatar);
	m_avatar_layout->addWidget(m_name_label);

	m_mic_mute_wrapper->setFixedSize(32, 32);
	m_mic_mute_wrapper->set_border_radius(4);
	m_mic_mute->setFixedSize(20, 20);
	m_mic_mute->set_background_color(Qt::transparent);
	m_mic_mute->set_pixmap([] {
		auto pm = QPixmap{ ":/icons/unmuted.svg" };
		QPainter p{ &pm };

		p.setCompositionMode(QPainter::CompositionMode_SourceIn);
		p.fillRect(pm.rect(), QColor{ 181, 186, 193 });

		return pm;
	}());

	m_speaker_mute_wrapper->setFixedSize(32, 32);
	m_speaker_mute_wrapper->set_border_radius(4);
	m_speaker_mute->setFixedSize(20, 20);
	m_speaker_mute->set_background_color(Qt::transparent);
	m_speaker_mute->set_pixmap([] {
		auto pm = QPixmap{ ":/icons/undeafened.svg" };
		QPainter p{ &pm };

		p.setCompositionMode(QPainter::CompositionMode_SourceIn);
		p.fillRect(pm.rect(), QColor{ 181, 186, 193 });

		return pm;
	}());

	m_mic_mute_layout->setContentsMargins(0, 0, 0, 0);
	m_mic_mute_layout->setSpacing(0);
	m_mic_mute_layout->addWidget(m_mic_mute);

	m_speaker_mute_layout->setContentsMargins(0, 0, 0, 0);
	m_speaker_mute_layout->setSpacing(0);
	m_speaker_mute_layout->addWidget(m_speaker_mute);

	m_layout->setContentsMargins(8, 0, 8, 0);
	m_layout->setSpacing(8);
	m_layout->addWidget(m_avatar_wrapper);
	m_layout->addWidget(m_mic_mute_wrapper);
	m_layout->addWidget(m_speaker_mute_wrapper);
}
} // namespace kato
