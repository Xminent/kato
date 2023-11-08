#include <QDir>
#include <QNetworkReply>
#include <mozart/api/image_cache.hpp>

namespace mozart
{
void ImageCache::cache_images(const std::vector<QUrl> &urls)
{
	auto &downloader = instance().m_downloader;

	if (const auto cache_path = QStandardPaths::writableLocation(
					    QStandardPaths::AppDataLocation) +
				    "/cache";
	    !QDir{ cache_path }.exists()) {
		QDir{}.mkpath(cache_path);
	}

	for (const auto &url : urls) {
		if (url.fileName().isEmpty()) {
			continue;
		}

		auto path = get_path(url.fileName());

		if (QFile::exists(path)) {
			continue;
		}

		auto *reply = downloader.get(QNetworkRequest{ url });

		QObject::connect(reply, &QNetworkReply::finished,
				 [reply, p = std::move(path)] {
					 QFile file{ p };

					 if (!file.open(QIODevice::WriteOnly)) {
						 return;
					 }

					 file.write(reply->readAll());
					 file.close();
				 });
	}
}
} // namespace mozart
