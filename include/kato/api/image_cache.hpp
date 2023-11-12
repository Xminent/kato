#ifndef KATO_API_IMAGE_CACHE_HPP
#define KATO_API_IMAGE_CACHE_HPP

#include <QFile>
#include <QNetworkAccessManager>
#include <QStandardPaths>
#include <optional>

namespace kato
{
struct ImageCache {
	[[nodiscard]] static bool contains(const QString &filename);
	[[nodiscard]] static QString get_path(const QString &filename);
	[[nodiscard]] static std::optional<QPixmap>
	get_image(const QString &filename);
	static void cache_image(const QUrl &url,
				const std::function<void(const QPixmap &)> &cb);
	static void remove_image(const QString &filename);
	static void download(const QUrl &url,
			     const std::function<void(const QPixmap &)> &cb);

	static const QString &cache_directory();

    private:
	explicit ImageCache() = default;

	[[nodiscard]] static ImageCache &instance();

	QNetworkAccessManager m_downloader;
};
} // namespace kato

#endif // KATO_API_IMAGE_CACHE_HPP
