#ifndef KATO_API_AUDIO_PROBE_HPP
#define KATO_API_AUDIO_PROBE_HPP

#include <QAudioBuffer>
#include <QAudioInput>
#include <QIODevice>

namespace kato
{
struct AudioProbeDevice : QIODevice {
	Q_OBJECT

    public:
	explicit AudioProbeDevice(QObject *parent = nullptr);

	void start(const QAudioInput *source);

    signals:
	void audio_available(const QAudioBuffer &buffer);

    private:
	qint64 readData(char *data, qint64 maxlen) override;
	qint64 writeData(const char *data, qint64 count) override;

	QAudioFormat m_format;
};
} // namespace kato

#endif // KATO_API_AUDIO_PROBE_HPP
