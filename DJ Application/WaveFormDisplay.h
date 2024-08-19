#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This class represents a waveform display component that visualizes an audio file's waveform.
*/
class WaveformDisplay : public Component,
    public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);

    // Destructor
    ~WaveformDisplay();

    // Paint method for rendering the waveform display
    void paint(Graphics&) override;

    // Resized method to handle component resizing
    void resized() override;

    // Callback method for change events
    void changeListenerCallback(ChangeBroadcaster* source) override;

    /*
        Load an audio file from the given URL.
    */
    void loadURL(URL audioURL);

    /*
        Set the relative position of the playhead.

    */
    void setPositionRelative(double pos);

private:
    // AudioThumbnail for displaying the waveform
    AudioThumbnail audioThumb;

    // Flag to track if an audio file is loaded
    bool fileLoaded;

    // Current playhead position (relative)
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
