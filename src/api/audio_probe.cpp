#include <mozart/api/audio_probe.hpp>

namespace mozart
{
AudioProbeDevice::AudioProbeDevice(QObject *parent)
	: QIODevice{ parent }
{
}

void AudioProbeDevice::start(const QAudioInput *source)
{
	Q_ASSERT(source);
	m_format = source->format();
	open(QIODevice::WriteOnly);
}

qint64 AudioProbeDevice::readData(char *data, qint64 maxlen)
{
	return 0;
}

qint64 AudioProbeDevice::writeData(const char *data, qint64 count)
{
	QAudioBuffer buffer{ { data, static_cast<int>(count) }, m_format };
	emit audio_available(buffer);
	return count;
}
} // namespace mozart
