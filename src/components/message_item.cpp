#include <QDebug>
#include <QDesktopServices>
#include <kato/api/file_downloader.hpp>
#include <kato/api/image_cache.hpp>
#include <kato/components/message_item.hpp>

namespace kato
{
MessageItem::MessageItem(uint64_t id, const QString &avatar,
			 const QString &author, const QString &message,
			 const QDateTime &date, QWidget *parent)
	: Widget{ parent }
	, m_id{ id }
	, m_author{ author }
	, m_date{ date }
	, m_message{ message }
	, m_avatar_label{ new Widget{ this } }
	, m_author_label{ new ClickableLabel{ author, this } }
	, m_date_label{ new QLabel{
		  m_date.toLocalTime().toString(
			  Qt::DateFormat::SystemLocaleShortDate),
		  this } }
	, m_message_label{ new QLabel{ message, this } }
{
	setup_ui();

	connect(m_author_label, &ClickableLabel::on_enter, this, [this] {
		auto f = m_author_label->font();
		f.setUnderline(true);
		m_author_label->setFont(f);
	});

	connect(m_author_label, &ClickableLabel::on_leave, this, [this] {
		auto f = m_author_label->font();
		f.setUnderline(false);
		m_author_label->setFont(f);
	});

	ImageCache::download(QUrl{ avatar }, [this](const auto &pixmap) {
		m_avatar_label->set_pixmap(pixmap);
	});
}

void MessageItem::setup_ui()
{
	setMinimumHeight(48);

	m_author_date_layout->setFixedHeight(22);
	m_author_date_layout->set_background_color(Qt::transparent);

	m_avatar_label->set_pixmap(QPixmap{ ":/images/logo.png" });
	m_avatar_label->setFixedSize(40, 40);
	m_avatar_label->set_border_radius(20);
	m_avatar_label->set_background_color(QColor(49, 51, 56));
	m_avatar_label->setCursor(Qt::PointingHandCursor);

	m_author_label->setCursor(Qt::PointingHandCursor);
	m_author_label->setFont([this] {
		auto f = font();
		f.setBold(true);
		f.setPointSize(11);
		return f;
	}());
	m_author_label->setTextInteractionFlags(Qt::TextSelectableByMouse);

	m_date_label->setFont([this] {
		auto f = font();
		f.setPointSize(8);
		return f;
	}());
	m_date_label->setTextInteractionFlags(Qt::TextSelectableByMouse);

	m_date_label->setPalette([this] {
		auto p = m_date_label->palette();
		p.setColor(QPalette::WindowText, Qt::gray);
		return p;
	}());

	m_message_label->setCursor(Qt::IBeamCursor);
	m_message_label->setFont([this] {
		auto f = font();
		f.setPointSize(11);
		return f;
	}());
	m_message_label->setTextFormat(Qt::TextFormat::PlainText);
	m_message_label->setTextInteractionFlags(Qt::TextSelectableByMouse);
	m_message_label->setWordWrap(true);

	{
		QString new_text;

		for (const QChar &character : m_message) {
			new_text.append(character);

			if (character.isPrint()) {
				new_text.append(QChar(0x200B));
			}
		}

		m_message_label->setText(new_text);
	}

	// If the message is a link, we set the color to blue and make it clickable, so it opens external links.
	if (m_message.startsWith("http://") ||
	    m_message.startsWith("https://")) {
		m_message_label->setPalette([this] {
			auto p = m_message_label->palette();
			// p.setColor(QPalette::Link, QColor{ 0, 168, 252 });
			p.setColor(QPalette::Link, Qt::green);
			return p;
		}());
		m_message_label->setStyleSheet("text-decoration: none;");
		const auto new_text =
			QString("<a href='%1'>%2</a>").arg(m_message, m_message);

		qDebug()
			<< "Editing from: " << m_message << " to: " << new_text;

		m_message_label->setText(new_text);
		m_message_label->setOpenExternalLinks(true);
		m_message_label->setCursor(Qt::PointingHandCursor);
		m_message_label->setTextFormat(Qt::TextFormat::RichText);
	}

	m_horizontal_layout->setContentsMargins(0, 0, 0, 0);
	m_horizontal_layout->setSpacing(0);
	m_horizontal_layout->addWidget(m_author_label);
	m_horizontal_layout->addSpacerItem(new QSpacerItem(
		8, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	m_horizontal_layout->addWidget(m_date_label);
	m_horizontal_layout->addStretch(1);

	m_vertical_layout->setContentsMargins(0, 0, 0, 0);
	m_vertical_layout->setSpacing(0);
	m_vertical_layout->addWidget(m_author_date_layout);
	m_vertical_layout->addWidget(m_message_label);

	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);
	m_layout->addSpacerItem(new QSpacerItem(16, 0, QSizePolicy::Fixed,
						QSizePolicy::Minimum));
	m_layout->addWidget(m_avatar_label, 0, Qt::AlignTop);
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

void MessageItem::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
}
} // namespace kato
