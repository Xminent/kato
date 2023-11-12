#ifndef KATO_API_FILE_DOWNLOADER_HPP
#define KATO_API_FILE_DOWNLOADER_HPP

#include <QNetworkAccessManager>
#include <optional>

namespace kato
{
struct FileDownloader : QObject {
	static void
	download(const QUrl &url,
		 const std::function<void(std::optional<QByteArray>)> &cb);

    private:
	explicit FileDownloader() = default;
};
} // namespace kato

#endif // KATO_API_FILE_DOWNLOADER_HPP
