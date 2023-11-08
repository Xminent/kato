#include <QToolButton>
#include <mozart/components/search_bar.hpp>

namespace mozart
{
SearchBar::SearchBar(QWidget *parent)
	: Widget{ parent }
{
	setup_ui();

	connect(m_search_input, &QLineEdit::returnPressed, this,
		&SearchBar::return_pressed);
}

void SearchBar::enterEvent([[maybe_unused]] QEvent *event)
{
	set_background_color(QColor(52, 52, 52));

	QPalette input_palette;
	QBrush input_background_brush{ QColor(52, 52, 52) };
	QBrush input_text_brush{ Qt::white };

	input_palette.setBrush(QPalette::Base, input_background_brush);
	input_palette.setBrush(QPalette::Text, input_text_brush);

	m_search_input->setPalette(input_palette);
}

void SearchBar::leaveEvent([[maybe_unused]] QEvent *event)
{
	set_background_color(QColor(42, 42, 42));

	QPalette input_palette;
	QBrush input_background_brush{ QColor(42, 42, 42) };
	QBrush input_text_brush{ Qt::white };

	input_palette.setBrush(QPalette::Base, input_background_brush);
	input_palette.setBrush(QPalette::Text, input_text_brush);

	m_search_input->setPalette(input_palette);
}

void SearchBar::focusInEvent(QFocusEvent *event)
{
	set_border_size(0);
	Widget::focusInEvent(event);
}

void SearchBar::focusOutEvent(QFocusEvent *event)
{
	set_border_size(1);
	Widget::focusOutEvent(event);
}

void SearchBar::setup_ui()
{
	set_background_color(QColor(42, 42, 42));
	set_border_color(Qt::white);
	set_border_radius(20);
	setFocusPolicy(Qt::StrongFocus);
	setFixedSize(364, 40);

	QPalette input_palette;
	QBrush input_background_brush{ QColor(42, 42, 42) };
	QBrush input_text_brush{ Qt::white };

	input_palette.setBrush(QPalette::Base, input_background_brush);
	input_palette.setBrush(QPalette::Text, input_text_brush);

	m_central_layout->setSpacing(0);
	m_central_layout->addWidget(m_label);
	m_central_layout->addWidget(m_search_input);

	m_search_input->setClearButtonEnabled(true);
	m_search_input->setFrame(false);
	m_search_input->setMaxLength(800);
	m_search_input->setPalette(input_palette);
	m_search_input->setPlaceholderText("What do you want to listen to?");
	auto font = m_search_input->font();
	font.setPointSize(12);
	m_search_input->setFont(font);

	auto *clear_button = m_search_input->findChild<QToolButton *>();
	clear_button->setFixedSize(24, 24);
	clear_button->setIconSize(QSize{ 24, 24 });
	clear_button->setIcon(QIcon{ ":/icons/close.svg" });

	m_label->setFixedSize(24, 24);
	m_label->setPixmap(QPixmap(":/icons/search.svg"));
	m_label->setScaledContents(true);
}
} // namespace mozart
