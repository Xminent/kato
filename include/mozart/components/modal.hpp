#ifndef MOZART_COMPONENTS_MODAL_HPP
#define MOZART_COMPONENTS_MODAL_HPP

#include "mozart/components/clickable_label.hpp"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <mozart/components/widget.hpp>

namespace mozart
{
struct Modal : Widget {
	Q_OBJECT

    public:
	explicit Modal(const QString &title, QWidget *parent = nullptr);

	void move_to_center();

    private:
	void setup_ui();
	void resizeEvent(QResizeEvent *event) override;

	QVBoxLayout *m_layout{ new QVBoxLayout(this) };
	Widget *m_content{ new Widget(this) };
	QVBoxLayout *m_content_layout{ new QVBoxLayout(m_content) };
	Widget *m_header{ new Widget(this) };
	QVBoxLayout *m_header_layout_v{ new QVBoxLayout(m_header) };
	QHBoxLayout *m_header_layout_h{ new QHBoxLayout() };
	QLabel *m_title{};
	ClickableLabel *m_close_button{ new ClickableLabel({}, this) };
};
} // namespace mozart

#endif // MOZART_COMPONENTS_MODAL_HPP
