#include <QPainterPath>
#include <QVariantAnimation>
#include <QtMath>
#include <array>
#include <kato/components/widget.hpp>
#include <utility>

namespace
{
struct Point {
	double angle;
	double a;
	double b;
};
} // namespace

namespace kato
{
Widget::Widget(QWidget *parent)
	: QWidget{ parent }
{
}

void Widget::set_background_color(QColor color)
{
	m_background_color = std::move(color);
	update();
}

void Widget::set_border_color(QColor color)
{
	m_border_color = std::move(color);
	update();
}

void Widget::set_border_size(int size)
{
	set_border_size(size, size, size, size);
}

void Widget::set_border_size(int top, int right, int bottom, int left)
{
	const auto [width, height] =
		std::make_pair(size().width(), size().height());

	m_border_top = std::clamp(top, 0, height);
	m_border_left = std::clamp(left, 0, width);
	m_border_bottom = std::clamp(bottom, 0, height);
	m_border_right = std::clamp(right, 0, width);

	update();
}

void Widget::set_border_radius(int radius, bool animate)
{
	set_border_radius(radius, radius, radius, radius, animate);
}

void Widget::set_border_radius(int top_left, int top_right, int bottom_right,
			       int bottom_left, bool animate)
{
	const auto [width, height] =
		std::make_pair(size().width(), size().height());

	top_left = std::max(top_left, 0);
	top_right = std::max(top_right, 0);
	bottom_right = std::max(bottom_right, 0);
	bottom_left = std::max(bottom_left, 0);

	// Normalize all four values to the size of the widget.
	if (top_left + top_right + bottom_right + bottom_left >
	    width + height) {
		const auto ratio =
			static_cast<double>(width + height) /
			static_cast<double>(top_left + top_right +
					    bottom_right + bottom_left);

		top_left = static_cast<int>(top_left * ratio);
		top_right = static_cast<int>(top_right * ratio);
		bottom_right = static_cast<int>(bottom_right * ratio);
		bottom_left = static_cast<int>(bottom_left * ratio);
	}

	// Now we normalize them according to their respective sides.
	if (top_left + top_right > width) {
		const auto ratio = static_cast<double>(width) /
				   static_cast<double>(top_left + top_right);

		top_left = static_cast<int>(top_left * ratio);
		top_right = static_cast<int>(top_right * ratio);
	}

	if (bottom_left + bottom_right > width) {
		const auto ratio =
			static_cast<double>(width) /
			static_cast<double>(bottom_left + bottom_right);

		bottom_left = static_cast<int>(bottom_left * ratio);
		bottom_right = static_cast<int>(bottom_right * ratio);
	}

	if (top_left + bottom_left > height) {
		const auto ratio = static_cast<double>(height) /
				   static_cast<double>(top_left + bottom_left);

		top_left = static_cast<int>(top_left * ratio);
		bottom_left = static_cast<int>(bottom_left * ratio);
	}

	if (top_right + bottom_right > height) {
		const auto ratio =
			static_cast<double>(height) /
			static_cast<double>(top_right + bottom_right);

		top_right = static_cast<int>(top_right * ratio);
		bottom_right = static_cast<int>(bottom_right * ratio);
	}

	if (!animate) {
		m_border_radius_top_left = top_left;
		m_border_radius_top_right = top_right;
		m_border_radius_bottom_right = bottom_right;
		m_border_radius_bottom_left = bottom_left;
		return update();
	}

	const auto top_left_delta = top_left - m_border_radius_top_left;
	const auto top_right_delta = top_right - m_border_radius_top_right;
	const auto bottom_right_delta =
		bottom_right - m_border_radius_bottom_right;
	const auto bottom_left_delta =
		bottom_left - m_border_radius_bottom_left;

	auto *animation = new QVariantAnimation(this);

	connect(animation, &QVariantAnimation::valueChanged,
		[this, og_top_left = m_border_radius_top_left,
		 og_top_right = m_border_radius_top_right,
		 og_bottom_right = m_border_radius_bottom_right,
		 og_bottom_left = m_border_radius_bottom_left, top_left_delta,
		 top_right_delta, bottom_right_delta,
		 bottom_left_delta](const QVariant &value) {
			const double progress =
				static_cast<double>(value.toInt()) / 100.0;

			const auto tl = top_left_delta * progress;
			const auto tr = top_right_delta * progress;
			const auto br = bottom_right_delta * progress;
			const auto bl = bottom_left_delta * progress;

			m_border_radius_top_left = og_top_left + tl;
			m_border_radius_top_right = og_top_right + tr;
			m_border_radius_bottom_right = og_bottom_right + br;
			m_border_radius_bottom_left = og_bottom_left + bl;

			update();
		});

	animation->setDuration(250);
	animation->setStartValue(0);
	animation->setEndValue(100);
	animation->setEasingCurve(QEasingCurve::InOutCubic);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Widget::set_fixed_size(int w, int h, std::optional<int> animation_ms)
{
	if (!animation_ms) {
		return setFixedSize(w, h);
	}

	const auto [width, height] =
		std::make_pair(size().width(), size().height());
	auto *animation = new QVariantAnimation(this);

	connect(animation, &QVariantAnimation::valueChanged,
		[this, og_width = width, og_height = height,
		 width_delta = w - width,
		 height_delta = h - height](const QVariant &value) {
			const double progress =
				static_cast<double>(value.toInt()) / 100.0;

			const auto wd = width_delta * progress;
			const auto hd = height_delta * progress;
			const auto new_width = og_width + wd;
			const auto new_height = og_height + hd;

			//  qDebug() << "Old width: " << og_width
			// 	  << "Old height: " << og_height;

			//  qDebug() << "New width: " << new_width
			// 	  << "New height: " << new_height;

			setFixedSize(new_width, new_height);
		});

	animation->setDuration(*animation_ms);
	animation->setStartValue(0);
	animation->setEndValue(100);
	animation->setEasingCurve(QEasingCurve::InOutCubic);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Widget::set_image(const QString &file, bool centered, QRect coords)
{
	m_img.load(file);
	m_img_centered = centered;
	m_img_coords = coords;
	update();
}

void Widget::set_movie(QMovie *movie, bool centered, QRect coords)
{
	m_img = {};
	m_img_centered = centered;
	m_img_coords = coords;
	m_mov = movie;
	update();
}

void Widget::set_pixmap(const QPixmap &pixmap, bool centered, QRect coords)
{
	m_img = {};
	m_img_centered = centered;
	m_img_coords = coords;
	m_pixmap = pixmap;
	update();
}

void Widget::set_text(const QString &text)
{
	m_text = text;
	update();
}

void Widget::set_text_alignment(int alignment)
{
	m_text_alignment = alignment;
	update();
}

void Widget::set_text_color(QColor color)
{
	m_text_color = std::move(color);
	update();
}

void Widget::set_font(QFont font)
{
	m_font = std::move(font);
	update();
}

void Widget::set_rotation(qreal degrees, bool animate)
{
	if (!animate) {
		m_rotation = degrees;
		return update();
	}

	auto *animation = new QVariantAnimation(this);

	connect(animation, &QVariantAnimation::valueChanged,
		[this, og_rotation = m_rotation,
		 rotation_delta = degrees - m_rotation](const QVariant &value) {
			const double progress =
				static_cast<double>(value.toInt()) / 100.0;

			m_rotation = og_rotation + rotation_delta * progress;
			update();
		});

	animation->setDuration(250);
	animation->setStartValue(0);
	animation->setEndValue(100);
	animation->setEasingCurve(QEasingCurve::InOutCubic);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}
void Widget::resizeEvent([[maybe_unused]] QResizeEvent *e)
{
	update();
}

void Widget::paintEvent([[maybe_unused]] QPaintEvent *event)
{
	QPainter painter{ this };

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush{ m_background_color, Qt::SolidPattern });

	const auto [width, height] =
		std::make_pair(size().width(), size().height());

	painter.setBrush(QBrush{ m_background_color, Qt::SolidPattern });

	QPainterPath bordered_path;
	QPainterPath inner_path;

	// If all border radii are equal, we can use a rounded rectangle.
	if (m_border_radius_top_left == m_border_radius_top_right &&
	    m_border_radius_top_left == m_border_radius_bottom_left &&
	    m_border_radius_top_left == m_border_radius_bottom_right) {
		inner_path.addRoundedRect(
			m_border_left, m_border_top,
			width - m_border_left - m_border_right,
			height - m_border_top - m_border_bottom,
			m_border_radius_top_left, m_border_radius_top_left);

		bordered_path.addRoundedRect(0, 0, width, height,
					     m_border_radius_top_left,
					     m_border_radius_top_left);

	} else {
		inner_path.moveTo(m_border_radius_top_left + m_border_left,
				  m_border_top);
		inner_path.lineTo(width - m_border_radius_top_right -
					  m_border_right,
				  m_border_top);

		if (m_border_radius_top_right > 0) {
			const auto arc_top_right_width =
				2 * m_border_radius_top_right - m_border_left -
				m_border_right;
			const auto arc_top_right_height =
				2 * m_border_radius_top_right - m_border_top -
				m_border_bottom;

			if (arc_top_right_width > 0 &&
			    arc_top_right_height > 0) {
				inner_path.arcTo(
					width - 2 * m_border_radius_top_right +
						m_border_left,
					m_border_top, arc_top_right_width,
					arc_top_right_height, 90, -90);
			} else {
				inner_path.lineTo(width - m_border_right,
						  m_border_top);
			}
		}

		bordered_path.moveTo(m_border_radius_top_left, 0);
		bordered_path.lineTo(width - m_border_radius_top_right, 0);
		bordered_path.arcTo(width - 2 * m_border_radius_top_right, 0,
				    2 * m_border_radius_top_right,
				    2 * m_border_radius_top_right, 90, -90);

		inner_path.lineTo(width - m_border_right,
				  height - m_border_radius_bottom_right -
					  m_border_bottom);

		if (m_border_radius_bottom_right > 0) {
			const auto arc_bottom_right_width =
				2 * m_border_radius_bottom_right -
				m_border_left - m_border_right;
			const auto arc_bottom_right_height =
				2 * m_border_radius_bottom_right -
				m_border_top - m_border_bottom;

			if (arc_bottom_right_width > 0 &&
			    arc_bottom_right_height > 0) {
				inner_path.arcTo(
					width - 2 * m_border_radius_bottom_right +
						m_border_left,
					height -
						2 * m_border_radius_bottom_right +
						m_border_top,
					arc_bottom_right_width,
					arc_bottom_right_height, 0, -90);
			} else {
				inner_path.lineTo(width - m_border_right,
						  height - m_border_bottom);
			}
		}

		bordered_path.lineTo(width,
				     height - m_border_radius_bottom_right);
		bordered_path.arcTo(width - 2 * m_border_radius_bottom_right,
				    height - 2 * m_border_radius_bottom_right,
				    2 * m_border_radius_bottom_right,
				    2 * m_border_radius_bottom_right, 0, -90);

		inner_path.lineTo(m_border_radius_bottom_left + m_border_left,
				  height - m_border_bottom);

		if (m_border_radius_bottom_left > 0) {
			const auto arc_bottom_left_width =
				2 * m_border_radius_bottom_left -
				m_border_left - m_border_right;
			const auto arc_bottom_left_height =
				2 * m_border_radius_bottom_left - m_border_top -
				m_border_bottom;

			if (arc_bottom_left_width > 0 &&
			    arc_bottom_left_height > 0) {
				inner_path.arcTo(
					m_border_left,
					height -
						2 * m_border_radius_bottom_left +
						m_border_top,
					arc_bottom_left_width,
					arc_bottom_left_height, 270, -90);
			} else {
				inner_path.lineTo(m_border_left,
						  height - m_border_bottom);
			}
		}

		bordered_path.lineTo(m_border_radius_bottom_left, height);
		bordered_path.arcTo(0, height - 2 * m_border_radius_bottom_left,
				    2 * m_border_radius_bottom_left,
				    2 * m_border_radius_bottom_left, 270, -90);

		inner_path.lineTo(m_border_left,
				  m_border_radius_top_left + m_border_top);

		if (m_border_radius_top_left > 0) {
			const auto arc_top_left_width =
				2 * m_border_radius_top_left - m_border_left -
				m_border_right;
			const auto arc_top_left_height =
				2 * m_border_radius_top_left - m_border_top -
				m_border_bottom;

			if (arc_top_left_width > 0 && arc_top_left_height > 0) {
				inner_path.arcTo(m_border_left, m_border_top,
						 arc_top_left_width,
						 arc_top_left_height, 180, -90);
			} else {
				inner_path.lineTo(m_border_left, m_border_top);
			}
		}

		bordered_path.lineTo(0, m_border_radius_top_left);
		bordered_path.arcTo(0, 0, 2 * m_border_radius_top_left,
				    2 * m_border_radius_top_left, 180, -90);
	}

	bordered_path -= inner_path;

	painter.drawPath(inner_path);
	painter.setBrush(QBrush{ m_border_color, Qt::SolidPattern });
	painter.drawPath(bordered_path);
	painter.setBrush(QBrush{ m_background_color, Qt::SolidPattern });

	// Create a QTransform for rotation
	QTransform rotation;
	rotation.rotate(m_rotation);

	// Rotate the corner points of the initial bounding box
	std::array<QPointF, 4> corner_points{ QPointF(0, 0), QPointF(width, 0),
					      QPointF(width, height),
					      QPointF(0, height) };

	for (size_t i{}; i < 4; ++i) {
		corner_points.at(i) = rotation.map(corner_points.at(i));
	}

	// Calculate the center of the rotated bounding box
	double centerX = (corner_points[0].x() + corner_points[2].x()) / 2.0;
	double centerY = (corner_points[0].y() + corner_points[2].y()) / 2.0;

	// Calculate the required translation to center the rotated bounding box
	const auto dx = width / 2.0 - centerX;
	const auto dy = height / 2.0 - centerY;

	painter.setTransform(
		QTransform::fromTranslate(dx, dy).rotate(m_rotation));

	if (m_mov != nullptr) {
		m_pixmap = m_mov->currentPixmap().scaled(
			width, height, Qt::KeepAspectRatio,
			Qt::SmoothTransformation);
	}

	if (!m_pixmap.isNull()) {
		const auto scaled_width = qMin(width, height);
		const auto scaled_height = static_cast<int>(
			(static_cast<qreal>(scaled_width) / m_pixmap.width()) *
			m_pixmap.height());
		const auto y = (height - scaled_height) / 2;

		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.setClipPath(inner_path);
		painter.drawPixmap(QRect(0, y, scaled_width, scaled_height),
				   m_pixmap.scaled(scaled_width, scaled_height,
						   Qt::KeepAspectRatio,
						   Qt::SmoothTransformation));

	} else if (!m_img.isNull()) {
		const auto [img_width, img_height] =
			std::make_pair(m_img.width(), m_img.height());

		painter.setClipPath(inner_path);
		painter.drawImage(
			QRect{ (img_width < width ? (width - img_width) / 2 :
						    0),
			       (img_height < height ?
					(height - img_height) / 2 :
					0),
			       (img_width < width ? img_width : width),
			       (img_height < height ? img_height : height) },
			(img_height < height && img_width < width ?
				 m_img :
				 m_img.scaled(width, height,
					      Qt::KeepAspectRatio,
					      Qt::SmoothTransformation)));
	}

	if (!m_text.isEmpty()) {
		painter.setPen(m_text_color);
		painter.setFont(m_font);

		// Our canvas is affected by the border.
		painter.drawText(
			QRect{ m_border_left, m_border_top,
			       width - m_border_left - m_border_right,
			       height - m_border_top - m_border_bottom },
			m_text_alignment, m_text);
	}
}
} // namespace kato
