#include <QGuiApplication>
#include <QLabel>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>
#include <mozart/components/modal.hpp>

namespace mozart
{
Modal::Modal(const QString &title, QWidget *child, QWidget *parent)
	: Widget{ parent }
	, m_child{ child }
	, m_title{ new QLabel(title, this) }
{
	if (m_child != nullptr) {
		m_child->setParent(this);
	}

	setup_ui();
	connect(m_close_button, &ClickableLabel::clicked, this, &Modal::close);
	move_to_center();

	connect(m_close_button, &ClickableLabel::on_enter, this, [this] {
		auto pm = *m_close_button->pixmap();

		QPainter p{ &pm };
		p.setCompositionMode(QPainter::CompositionMode_SourceIn);
		p.fillRect(pm.rect(), Qt::white);

		m_close_button->setPixmap(pm);
	});
	connect(m_close_button, &ClickableLabel::on_leave, this, [this] {
		auto pm = *m_close_button->pixmap();

		QPainter p{ &pm };
		p.setCompositionMode(QPainter::CompositionMode_SourceIn);
		p.fillRect(pm.rect(), QColor{ 142, 146, 151 });

		m_close_button->setPixmap(pm);
	});
}

void Modal::move_to_center()
{
	if (parentWidget() != nullptr) {
		resize(parentWidget()->size());
		const auto center = parentWidget()->rect().center();
		const auto widget_pos = center - rect().center();
		return move(widget_pos);
	}

	resize(QGuiApplication::primaryScreen()->geometry().size());
	const auto center =
		QGuiApplication::primaryScreen()->geometry().center();
	const auto widget_pos = center - rect().center();
	move(widget_pos);
}

void Modal::setup_ui()
{
	set_background_color(QColor{ 0, 0, 0, 200 });

	m_title->setFont([this] {
		auto f = font();
		f.setBold(true);
		f.setPointSize(14);
		return f;
	}());

	m_close_button->setPixmap([] {
		auto pm = QPixmap(":/icons/close.svg");

		QPainter p{ &pm };
		p.setCompositionMode(QPainter::CompositionMode_SourceIn);
		p.fillRect(pm.rect(), QColor{ 142, 146, 151 });

		return pm;
	}());

	m_close_button->setCursor(Qt::PointingHandCursor);

	m_header->set_fixed_size(460, 80);
	m_header->set_background_color(Qt::transparent);

	m_header_layout_h->setContentsMargins(0, 0, 0, 0);
	m_header_layout_h->setSpacing(0);
	m_header_layout_h->addSpacerItem(new QSpacerItem(
		16, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	m_header_layout_h->addWidget(m_title);
	m_header_layout_h->addSpacerItem(new QSpacerItem(
		0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	m_header_layout_h->addWidget(m_close_button);
	m_header_layout_h->addSpacerItem(new QSpacerItem(
		16, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));

	m_header_layout_v->setContentsMargins(0, 0, 0, 0);
	m_header_layout_v->setSpacing(0);
	m_header_layout_v->addLayout(m_header_layout_h);

	m_header_layout_v->addSpacerItem(new QSpacerItem(
		0, 16, QSizePolicy::Minimum, QSizePolicy::Fixed));
	m_header_layout_v->addWidget(m_header);
	m_header_layout_v->addSpacerItem(new QSpacerItem(
		0, 16, QSizePolicy::Minimum, QSizePolicy::Fixed));

	m_content->setMinimumSize(460, 360);
	m_content->set_background_color(QColor{ 6, 6, 7 });
	m_content->set_border_color(QColor{ 255, 255, 255, 25 });
	m_content->set_border_radius(12);
	m_content->set_border_size(1);

	m_content_layout->setContentsMargins(0, 0, 0, 0);
	m_content_layout->setSpacing(0);
	m_content_layout->addWidget(m_header, 0, Qt::AlignTop);

	if (m_child != nullptr) {
		m_content_layout->addWidget(m_child);
	}

	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);
	m_layout->addWidget(m_content, 0, Qt::AlignCenter);
}

void Modal::resizeEvent(QResizeEvent *event)
{
	move_to_center();
}
} // namespace mozart
