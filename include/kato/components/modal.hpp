#ifndef KATO_COMPONENTS_MODAL_HPP
#define KATO_COMPONENTS_MODAL_HPP

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <kato/components/clickable_label.hpp>
#include <kato/components/widget.hpp>

namespace kato
{
struct Modal : Widget {
	Q_OBJECT

    public:
	Modal(const QString &title, QWidget *child = nullptr,
	      QWidget *parent = nullptr);

	void move_to_center();

    private:
	void setup_ui();
	void resizeEvent(QResizeEvent *event) override;

	QWidget *m_child;
	QVBoxLayout *m_layout{ new QVBoxLayout(this) };
	Widget *m_content{ new Widget(this) };
	QVBoxLayout *m_content_layout{ new QVBoxLayout(m_content) };
	Widget *m_header{ new Widget(this) };
	QVBoxLayout *m_header_layout_v{ new QVBoxLayout(m_header) };
	QHBoxLayout *m_header_layout_h{ new QHBoxLayout() };
	QLabel *m_title{};
	ClickableLabel *m_close_button{ new ClickableLabel({}, this) };
};
} // namespace kato

#endif // KATO_COMPONENTS_MODAL_HPP
