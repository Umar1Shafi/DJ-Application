#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// DJAudioPlayer class, responsible for audio playback
class DJAudioPlayer : public AudioSource {
public:
    // Constructor
    DJAudioPlayer(AudioFormatManager& _formatManager);

    // Destructor
    ~DJAudioPlayer();

    // AudioSource interface implementation

    // Prepare audio source for playback
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    // Get the next audio block to fill the buffer
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    // Release audio resources
    void releaseResources() override;

    // Load an audio file from the given URL
    void loadURL(URL audioURL);

    // Set the gain (volume) for audio playback
    void setGain(double gain);

    // Set the speed (resampling ratio) for audio playback
    void setSpeed(double ratio);

    // Set the playback position in seconds
    void setPosition(double posInSecs);

    // Set the playback position relative to the audio duration (0.0 to 1.0)
    void setPositionRelative(double pos);

    // Get the current gain (volume) setting
    double getGain() const;

    // Get the duration of the loaded audio in seconds
    double getLength();

    // Start audio playback
    void start();

    // Stop audio playback
    void stop();

    // Check if audio is currently playing
    bool isPlaying() const;

    // Get the relative position of the playhead
    double getPositionRelative();

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
    bool playing;
};
