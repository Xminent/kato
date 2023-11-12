#include <QMouseEvent>
#include <kato/api/image_cache.hpp>
#include <kato/components/user.hpp>

namespace
{
constexpr int ICON_RADIUS{ 16 };
constexpr int ROUNDED_RECT_RADIUS{ 4 };
} // namespace

namespace kato
{
User::User(const QString &avatar, const QString &name, QWidget *parent)
	: Widget{ parent }
{
	setup_ui();

	m_name->set_text(name);

	ImageCache::download(QUrl{ avatar }, [this](const auto &pixmap) {
		m_avatar->set_pixmap(pixmap);
	});
}

void User::setup_ui()
{
	setFixedSize(224, 44);
	set_background_color(Qt::transparent);

	m_avatar->set_fixed_size(32, 32);
	m_avatar->set_border_radius(ICON_RADIUS);
	m_avatar->set_pixmap(QPixmap{ ":/images/logo.png" });

	m_name->setMaximumSize(124, 20);
	m_name->set_background_color(Qt::transparent);
	m_name->set_text_alignment(Qt::AlignLeft);
	m_name->set_text_color(QColor(142, 146, 151));

	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);
	m_layout->addWidget(m_avatar);
	m_layout->addWidget(m_name);
}

void User::enterEvent(QEvent *event)
{
	set_background_color(QColor(47, 49, 54));
	set_border_radius(ROUNDED_RECT_RADIUS);

	m_name->set_text_color(Qt::white);
}

void User::leaveEvent(QEvent *event)
{
	set_background_color(Qt::transparent);
	set_border_radius(0);

	m_name->set_text_color(QColor(142, 146, 151));
}

void User::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		emit left_clicked(this);
	}
}
} // namespace kato
