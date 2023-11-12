#include <kato/components/dropdown_group.hpp>
#include <qnamespace.h>

namespace kato
{
DropdownGroup::DropdownGroup(const QString &text, QWidget *parent)
	: Widget{ parent }
	, m_text{ new Widget(this) }
	, m_icon{ new ClickableLabel{ {}, this } }
{
	setup_ui();
	m_text->set_text(text);

	connect(m_icon, &ClickableLabel::clicked, this, &DropdownGroup::add);
	connect(m_icon, &ClickableLabel::on_enter, this, [this] {
		auto pixmap = *m_icon->pixmap();

		QPainter painter{ &pixmap };
		painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
		painter.fillRect(pixmap.rect(), Qt::white);

		m_icon->setPixmap(pixmap);
	});
	connect(m_icon, &ClickableLabel::on_leave, this, [this] {
		auto pixmap = *m_icon->pixmap();

		QPainter painter{ &pixmap };
		painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
		painter.fillRect(pixmap.rect(), QColor(142, 146, 151));

		m_icon->setPixmap(pixmap);
	});
}

void DropdownGroup::setup_ui()
{
	setCursor(Qt::PointingHandCursor);
	setFixedHeight(24);

	m_arrow->setPixmap(QPixmap(":/icons/arrow.svg"));
	m_arrow->setScaledContents(true);
	m_arrow->setFixedSize(12, 12);

	m_text->set_text_alignment(Qt::AlignLeft);
	m_text->set_text_color(QColor(142, 146, 151));
	m_text->setFont([this] {
		auto f = font();
		f.setBold(true);
		f.setPointSize(9);
		return f;
	}());

	m_icon->setPixmap(QPixmap(":/icons/add.svg"));
	m_icon->setScaledContents(true);
	m_icon->setFixedSize(18, 18);

	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);
	m_layout->addWidget(m_arrow);
	m_layout->addSpacerItem(new QSpacerItem(8, 0, QSizePolicy::Fixed,
						QSizePolicy::Minimum));
	m_layout->addWidget(m_text);
	m_layout->addSpacerItem(new QSpacerItem(8, 0, QSizePolicy::Fixed,
						QSizePolicy::Minimum));
	m_layout->addWidget(m_icon);
}

void DropdownGroup::enterEvent(QEvent *event)
{
	auto pixmap = *m_arrow->pixmap();

	QPainter painter{ &pixmap };
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(pixmap.rect(), Qt::white);

	m_arrow->setPixmap(pixmap);
	m_text->set_text_color(Qt::white);
}

void DropdownGroup::leaveEvent(QEvent *event)
{
	auto pixmap = *m_arrow->pixmap();

	QPainter painter{ &pixmap };
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(pixmap.rect(), QColor(142, 146, 151));

	m_arrow->setPixmap(pixmap);
	m_text->set_text_color(QColor(142, 146, 151));
}

void DropdownGroup::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		emit toggle();
	}
}
} // namespace kato
