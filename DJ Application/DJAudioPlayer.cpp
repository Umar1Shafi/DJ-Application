#include "DJAudioPlayer.h"

// Constructor
DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
                            : formatManager(_formatManager)
{
}

// Destructor
DJAudioPlayer::~DJAudioPlayer()
{
}

// Prepare the audio source for playback
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Prepare the transport source and resampling source
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

// Get the next audio block to fill the buffer
void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

// Release audio resources
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

// Load an audio file from the given URL
void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)
    {
        // Create a new AudioFormatReaderSource and set it as the transport source
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

// Set the gain (volume) for the audio playback
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        // Log an error message if gain is out of range
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
}

// Set the speed (resampling ratio) for audio playback
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100.0)
    {
        // Log an error message if speed ratio is out of range
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

// Set the playback position in seconds
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

// Set the playback position relative to the audio duration (0.0 to 1.0)
void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        // Log an error message if relative position is out of range
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

// Start audio playback
void DJAudioPlayer::start()
{
    transportSource.start();
    playing = true;
}

// Stop audio playback
void DJAudioPlayer::stop()
{
    transportSource.stop();
    playing = false;
}

// Check if audio is currently playing
bool DJAudioPlayer::isPlaying() const
{
    return playing;
}

// Get the current playback position relative to the audio duration (0.0 to 1.0)
double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

// Get the current gain (volume) setting
double DJAudioPlayer::getGain() const
{
    return transportSource.getGain();
}

// Get the duration of the loaded audio in seconds
double DJAudioPlayer::getLength()
{
    return transportSource.getLengthInSeconds();
}
