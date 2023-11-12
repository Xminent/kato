#include <kato/components/header.hpp>

namespace kato
{
Header::Header(const QString &name, const std::optional<QPixmap> &icon,
	       QWidget *parent)
	: Widget{ parent }
	, m_name{ name }
	, m_icon{ icon }
{
	setup_ui();
}

void Header::setup_ui()
{
	setFixedHeight(48);
	set_border_size(0, 0, 2, 0);
	set_border_color(QColor(6, 6, 7));
	set_background_color(Qt::transparent);

	m_text = new QLabel{ m_name, this };
	m_text->setAlignment(Qt::AlignLeft);
	m_text->setFont([this] {
		auto f = font();
		f.setBold(true);
		f.setPointSize(11);
		return f;
	}());

	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->addSpacerItem(new QSpacerItem(16, 0, QSizePolicy::Fixed,
						QSizePolicy::Minimum));

	m_vertical_layout->addSpacerItem(new QSpacerItem(
		0, 12, QSizePolicy::Minimum, QSizePolicy::Fixed));

	if (m_icon) {
		auto *label = new QLabel(this);
		label->setPixmap(*m_icon);
		label->setScaledContents(true);
		label->setFixedSize(20, 20);

		m_horizontal_layout->addWidget(label, 0, Qt::AlignVCenter);
		m_horizontal_layout->addSpacerItem(new QSpacerItem(
			8, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	}

	m_horizontal_layout->addWidget(m_text, 0, Qt::AlignVCenter);
	m_vertical_layout->addLayout(m_horizontal_layout);
	m_vertical_layout->addSpacerItem(new QSpacerItem(
		0, 12, QSizePolicy::Minimum, QSizePolicy::Fixed));

	m_layout->addLayout(m_vertical_layout);

	m_layout->addSpacerItem(new QSpacerItem(16, 0, QSizePolicy::Fixed,
						QSizePolicy::Minimum));
}

void Header::enterEvent(QEvent *e)
{
	set_background_color(QColor(6, 6, 7));
}

void Header::leaveEvent(QEvent *e)
{
	set_background_color(Qt::transparent);
}
} // namespace kato
