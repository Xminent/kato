#include <mozart/api/audio_output.hpp>

namespace mozart
{
AudioOutputDevice::AudioOutputDevice(QObject *parent)
	: QIODevice{ parent }
{
}

void AudioOutputDevice::start(const QAudioOutput *destination)
{
	Q_ASSERT(destination);
	m_format = destination->format();
	open(QIODevice::ReadOnly);
}

qint64 AudioOutputDevice::readData(char *data, qint64 maxlen)
{
	// Implement your logic to read audio data here
	// and copy it to the 'data' buffer.
	// Return the number of bytes read.

	return 0; // Return 0 for now as a placeholder
}
} // namespace mozart