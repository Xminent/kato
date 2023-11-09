#ifndef MOZART_COMPONENTS_HEADER_HPP
#define MOZART_COMPONENTS_HEADER_HPP

#include "mozart/components/widget.hpp"
#include <qboxlayout.h>
#include <qlabel.h>

namespace mozart
{
struct Header : Widget {
	Q_OBJECT

    public:
	explicit Header(const QString &name,
			const std::optional<QPixmap> &icon = std::nullopt,
			QWidget *parent = nullptr);

    private:
	void setup_ui();
	void enterEvent(QEvent *e) override;
	void leaveEvent(QEvent *e) override;

	QString m_name;
	std::optional<QPixmap> m_icon;
	QHBoxLayout *m_layout{ new QHBoxLayout(this) };
	QVBoxLayout *m_vertical_layout{ new QVBoxLayout() };
	QHBoxLayout *m_horizontal_layout{ new QHBoxLayout() };
	QLabel *m_text{};
};
} // namespace mozart

#endif // MOZART_COMPONENTS_HEADER_HPP
