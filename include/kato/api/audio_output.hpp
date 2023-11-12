#ifndef KATO_API_AUDIO_OUTPUT_HPP
#define KATO_API_AUDIO_OUTPUT_HPP

#include <QAudioOutput>
#include <QIODevice>

namespace kato
{
struct AudioOutputDevice : QIODevice {
	Q_OBJECT

    public:
	explicit AudioOutputDevice(QObject *parent = nullptr);

	void start(const QAudioOutput *destination);

	qint64 readData(char *data, qint64 maxlen) override;

	qint64 writeData(const char * /*data*/, qint64 /*count*/) override
	{
		return 0;
	}

    private:
	QAudioFormat m_format;
};
} // namespace kato

#endif // KATO_API_AUDIO_OUTPUT_HPP