#include <QDir>
#include <QPixmap>
#include <kato/api/file_downloader.hpp>
#include <kato/api/image_cache.hpp>

namespace kato
{
bool ImageCache::contains(const QString &filename)
{
	return !filename.isEmpty() && QFile::exists(get_path(filename));
}

QString ImageCache::get_path(const QString &filename)
{
	return QStandardPaths::writableLocation(
		       QStandardPaths::AppDataLocation) +
	       "/cache/" + filename;
}

std::optional<QPixmap> ImageCache::get_image(const QString &filename)
{
	if (!contains(filename)) {
		return std::nullopt;
	}

	return QPixmap{ get_path(filename) };
}

void ImageCache::cache_image(const QUrl &url,
			     const std::function<void(const QPixmap &)> &cb)
{
	if (!QDir{ cache_directory() }.exists()) {
		QDir{}.mkpath(cache_directory());
	}

	if (url.fileName().isEmpty()) {
		return;
	}

	auto path = get_path(url.fileName());

	if (QFile::exists(path)) {
		return cb(QPixmap{ path });
	}

	FileDownloader::download(url, [p = std::move(path), cb](auto data) {
		if (!data) {
			// NOTE: Debating replacing std::optional with something like tl::expected to forward errors. Future task.
			return;
		}

		QFile file{ p };

		if (!file.open(QIODevice::WriteOnly)) {
			return;
		}

		file.write(*data);
		file.close();

		QPixmap pm;
		pm.loadFromData(*data);
		cb(pm);
	});
}

void ImageCache::remove_image(const QString &filename)
{
	if (!contains(filename)) {
		return;
	}

	QFile::remove(get_path(filename));
}

void ImageCache::download(const QUrl &url,
			  const std::function<void(const QPixmap &)> &cb)
{
	cache_image(url, cb);
}

const QString &ImageCache::cache_directory()
{
	static const auto ret = QStandardPaths::writableLocation(
					QStandardPaths::AppDataLocation) +
				"/cache";
	return ret;
}

ImageCache &ImageCache::instance()
{
	static ImageCache instance;
	qDebug() << "Cache Directory: "
		 << QStandardPaths::writableLocation(
			    QStandardPaths::AppDataLocation) +
			    "/cache";
	return instance;
}
} // namespace kato
