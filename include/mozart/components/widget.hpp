#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QMovie>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QWidget>
#include <optional>

namespace mozart
{
struct Widget : QWidget {
	explicit Widget(QWidget *parent = nullptr);

	void set_background_color(QColor color);
	void set_border_color(QColor color);
	void set_border_size(int size);
	void set_border_size(int top, int right, int bottom, int left);
	void set_border_radius(int radius, bool animate = false);
	void set_border_radius(int top_left, int top_right, int bottom_right,
			       int bottom_left, bool animate = false);
	void set_fixed_size(int w, int h,
			    std::optional<int> animation_ms = std::nullopt);
	void set_image(const QString &file, bool centered = true,
		       QRect coords = QRect());
	void set_movie(QMovie *movie, bool centered = true,
		       QRect coords = QRect());
	void set_pixmap(const QPixmap &pixmap, bool centered = true,
			QRect coords = QRect());
	void set_text(const QString &text);
	void set_text_alignment(int alignment);
	void set_text_color(QColor color);
	void set_font(QFont font);

    private:
	void resizeEvent(QResizeEvent *e) override;
	void paintEvent(QPaintEvent *event) override;

	QColor m_background_color;
	QColor m_border_color;
	int m_border_radius_top_left{};
	int m_border_radius_top_right{};
	int m_border_radius_bottom_right{};
	int m_border_radius_bottom_left{};
	int m_border_top{};
	int m_border_left{};
	int m_border_bottom{};
	int m_border_right{};
	QFont m_font;
	QImage m_img;
	bool m_img_centered{};
	QRect m_img_coords;
	QMovie *m_mov{};
	QPixmap m_pixmap;
	QString m_text;
	int m_text_alignment{ Qt::AlignCenter };
	QColor m_text_color;
};
} // namespace mozart

#endif // WIDGET_HPP
