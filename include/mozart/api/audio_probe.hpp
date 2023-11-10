#ifndef MOZART_API_AUDIO_PROBE_HPP
#define MOZART_API_AUDIO_PROBE_HPP

#include <QAudioBuffer>
#include <QAudioInput>
#include <QIODevice>

namespace mozart
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
} // namespace mozart

#endif // MOZART_API_AUDIO_PROBE_HPP
