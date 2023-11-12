#include <kato/components/dropdown_group.hpp>
#include <qnamespace.h>

namespace kato
{
DropdownGroup::DropdownGroup(const QString &text, QWidget *parent)
	: Widget{ parent }
	, m_text{ new ClickableLabel(text, this) }
	, m_icon{ new ClickableLabel{ {}, this } }
{
	setup_ui();

	connect(m_text, &ClickableLabel::clicked, this,
		[this, collapsed = false]() mutable {
			collapsed = !collapsed;

			m_arrow->set_rotation(collapsed ? 0 : -90, true);
		});

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

	m_arrow->set_pixmap(QPixmap(":/icons/arrow.svg"));
	m_arrow->setFixedSize(12, 12);

	m_text->setPalette([this] {
		auto p = palette();
		p.setColor(QPalette::WindowText, QColor(142, 146, 151));
		return p;
	}());
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
	auto pixmap = m_arrow->pixmap();

	QPainter painter{ &pixmap };
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(pixmap.rect(), Qt::white);

	m_arrow->set_pixmap(pixmap);
	m_text->setPalette([this] {
		auto p = palette();
		p.setColor(QPalette::WindowText, Qt::white);
		return p;
	}());
}

void DropdownGroup::leaveEvent(QEvent *event)
{
	auto pixmap = m_arrow->pixmap();

	QPainter painter{ &pixmap };
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(pixmap.rect(), QColor(142, 146, 151));

	m_arrow->set_pixmap(pixmap);
	m_text->setPalette([this] {
		auto p = palette();
		p.setColor(QPalette::WindowText, QColor(142, 146, 151));
		return p;
	}());
}

void DropdownGroup::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		emit toggle();
	}
}
} // namespace kato
