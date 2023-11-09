#include <mozart/components/message_item.hpp>

namespace mozart
{
MessageItem::MessageItem(const QString &author, const QString &message,
			 QWidget *parent)
	: Widget{ parent }
	, m_author{ author }
	, m_date{ QDateTime::currentDateTime() }
	, m_message{ message }
	, m_avatar_label{ new Widget{ this } }
	, m_author_label{ new ClickableLabel{
		  { author,
		    [this] {
			    auto f = m_author_label->font();
			    f.setUnderline(true);
			    m_author_label->setFont(f);
		    },
		    [this] {
			    auto f = m_author_label->font();
			    f.setUnderline(false);
			    m_author_label->setFont(f);
		    } },
		  this } }
	, m_date_label{ new QLabel{ m_date.toString("dd/MM/yyyy hh:mm"),
				    this } }
	, m_message_label{ new QLabel{ message, this } }
{
	setup_ui();
}

void MessageItem::setup_ui()
{
	setMinimumHeight(48);

	m_avatar_label->set_pixmap(QPixmap{ ":/images/logo.png" });
	m_avatar_label->setFixedSize(40, 40);
	m_avatar_label->set_border_radius(20);
	m_avatar_label->set_background_color(QColor(49, 51, 56));
	m_avatar_label->setCursor(Qt::PointingHandCursor);

	for (auto *label : { dynamic_cast<QLabel *>(m_author_label),
			     m_date_label, m_message_label }) {
		label->setTextInteractionFlags(Qt::TextSelectableByMouse);
		label->setWordWrap(true);
		label->setAlignment(Qt::AlignLeft);
		label->setFont(QFont{ "Inter", 10 });
	}

	for (auto *layout : { m_layout, m_horizontal_layout }) {
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);
	}

	m_author_label->setFont(QFont{ "Inter", 10, QFont::Bold });
	m_author_label->setCursor(Qt::PointingHandCursor);
	m_date_label->setFont(QFont{ "Inter", 8 });
	m_date_label->setStyleSheet("color: gray;");
	m_message_label->setCursor(Qt::IBeamCursor);

	m_horizontal_layout->addWidget(m_author_label);
	m_horizontal_layout->addSpacerItem(new QSpacerItem(
		8, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	m_horizontal_layout->addWidget(m_date_label);
	m_horizontal_layout->addSpacerItem(new QSpacerItem(
		0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

	m_vertical_layout->setContentsMargins(0, 0, 0, 0);
	m_vertical_layout->setSpacing(0);
	m_vertical_layout->addLayout(m_horizontal_layout);
	m_vertical_layout->addSpacerItem(new QSpacerItem(
		0, 8, QSizePolicy::Minimum, QSizePolicy::Fixed));
	m_vertical_layout->addWidget(m_message_label);

	m_layout->addWidget(m_avatar_label);
	m_layout->addSpacerItem(new QSpacerItem(16, 0, QSizePolicy::Fixed,
						QSizePolicy::Minimum));
	m_layout->addLayout(m_vertical_layout);
}

void MessageItem::enterEvent(QEvent *event)
{
	set_background_color(QColor(4, 4, 4));
}

void MessageItem::leaveEvent(QEvent *event)
{
	set_background_color(Qt::transparent);
}
} // namespace mozart
