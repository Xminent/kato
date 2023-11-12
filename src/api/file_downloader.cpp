#include <QNetworkReply>
#include <kato/api/file_downloader.hpp>
#include <memory>


namespace kato
{
void FileDownloader::download(
	const QUrl &url,
	const std::function<void(std::optional<QByteArray>)> &cb)
{
	static const auto manager = [] {
		qDebug() << QSslSocket::supportsSsl()
			 << QSslSocket::sslLibraryBuildVersionString()
			 << QSslSocket::sslLibraryVersionString();

		auto ret = std::make_unique<QNetworkAccessManager>();
		ret->setRedirectPolicy(
			QNetworkRequest::NoLessSafeRedirectPolicy);
		return ret;
	}();

	if (!url.isValid() || url.isLocalFile()) {
		return;
	}

	qDebug() << "Downloading: " << url;

	auto *reply = manager->get(QNetworkRequest(url));

	QObject::connect(reply, &QNetworkReply::finished, [reply, cb] {
		if (reply->error() != QNetworkReply::NoError) {
			cb(std::nullopt);
			return;
		}

		cb(std::make_optional(reply->readAll()));

		reply->deleteLater();
	});
}
} // namespace kato
