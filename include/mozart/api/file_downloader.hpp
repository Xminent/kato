#ifndef MOZART_API_FILE_DOWNLOADER_HPP
#define MOZART_API_FILE_DOWNLOADER_HPP

#include <QNetworkAccessManager>
#include <optional>

namespace mozart
{
struct FileDownloader : QObject {
	static void
	download(const QUrl &url,
		 const std::function<void(std::optional<QByteArray>)> &cb);

    private:
	explicit FileDownloader() = default;
};
} // namespace mozart

#endif // MOZART_API_FILE_DOWNLOADER_HPP
