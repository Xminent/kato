#ifndef KATO_COMPONENTS_HEADER_HPP
#define KATO_COMPONENTS_HEADER_HPP

#include "kato/components/widget.hpp"
#include <qboxlayout.h>
#include <qlabel.h>

namespace kato
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
} // namespace kato

#endif // KATO_COMPONENTS_HEADER_HPP
