#include <kato/components/form.hpp>
#include <qnamespace.h>

namespace kato
{
Form::Form(const QString &name, QWidget *parent)
	: Widget{ parent }
	, m_name{ name }
{
	setup_ui();

	connect(m_input, &QPlainTextEdit::textChanged, this,
		[this] { m_message = m_input->toPlainText().trimmed(); });

	m_input->installEventFilter(this);
}

void Form::clear()
{
	m_input->clear();
}

void Form::setup_ui()
{
	setMinimumHeight(68);
	set_background_color(Qt::transparent);

	{
		QPalette palette;
		QBrush bg_brush{ QColor(42, 42, 42) };
		QBrush text_brush{ Qt::white };

		palette.setBrush(QPalette::Base, bg_brush);
		palette.setBrush(QPalette::Text, text_brush);

		m_input->setPalette(palette);
	}

	m_input->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	m_input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_input->setPlaceholderText(QString{ "Message #%1" }.arg(m_name));
	m_input->setFrameShape(QFrame::NoFrame);
	m_input->setMinimumHeight(44);
	m_input->setStyleSheet(
		"border-radius: 8px; background-color: rgb(12, 12, 13); color: white;");

	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->addSpacerItem(new QSpacerItem(16, 0, QSizePolicy::Fixed,
						QSizePolicy::Minimum));

	m_vertical_layout->addWidget(m_input);
	m_vertical_layout->addSpacerItem(new QSpacerItem(
		0, 24, QSizePolicy::Minimum, QSizePolicy::Fixed));

	m_layout->addLayout(m_vertical_layout);

	m_layout->addSpacerItem(new QSpacerItem(16, 0, QSizePolicy::Fixed,
						QSizePolicy::Minimum));
}

bool Form::eventFilter(QObject *obj, QEvent *event)
{
	if (obj != m_input || event->type() != QEvent::KeyPress) {
		return QWidget::eventFilter(obj, event);
	}

	const auto *key_event = dynamic_cast<QKeyEvent *>(event);

	if (const auto key = key_event->key();
	    key != Qt::Key_Return && key != Qt::Key_Enter) {
		return QWidget::eventFilter(obj, event);
	}

	if ((key_event->modifiers() & Qt::ShiftModifier) != 0) {
		m_input->insertPlainText("\n");
		return true;
	}

	if (!m_message.isEmpty()) {
		emit message_sent(m_message);
	}

	return true;
}
} // namespace kato