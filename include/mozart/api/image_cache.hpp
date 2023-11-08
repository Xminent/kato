#ifndef MOZART_API_IMAGE_CACHE_HPP
#define MOZART_API_IMAGE_CACHE_HPP
#include <QFile>
#include <QNetworkAccessManager>
#include <QStandardPaths>

namespace mozart
{
class ImageCache {
    public:
	[[nodiscard]] static bool contains(const QString &filename)
	{
		return !filename.isEmpty() && QFile::exists(get_path(filename));
	}

	[[nodiscard]] static QString get_path(const QString &filename)
	{
		return QStandardPaths::writableLocation(
			       QStandardPaths::AppDataLocation) +
		       "/cache/" + filename;
	}

	static void cache_images(const std::vector<QUrl> &urls);

    private:
	explicit ImageCache() = default;

	[[nodiscard]] static ImageCache &instance()
	{
		static ImageCache instance;
		return instance;
	}

	QNetworkAccessManager m_downloader;
};
} // namespace mozart

#endif // MOZART_API_IMAGE_CACHE_HPP
