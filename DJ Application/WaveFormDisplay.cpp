#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                AudioThumbnailCache& cacheToUse) :
                                audioThumb(1000, formatManagerToUse, cacheToUse),
                                fileLoaded(false),
                                position(0)
{
    // Initialize the audio thumbnail with provided format manager and cache.
    audioThumb.addChangeListener(this); // Register this component as a change listener.
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(Graphics& g)
{
    // Set the background color and draw a border around the component.
    g.setColour(Colours::transparentBlack);
    g.drawRect(getLocalBounds(), 1);

    // Set the color for the waveform display.

    g.setColour(Colours::crimson);
    if (fileLoaded)
    {
        // Draw the audio waveform using the audio thumbnail.
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            0.4f
        );

        // Highlight the current playback position.
        g.setColour(Colours::white);
        g.drawRect(position * getWidth(), 0, getWidth() / 100, getHeight() * 1.05);
    }
    else
    {
        // Display a message when no audio file is loaded.
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
            Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{
    // This method is currently empty, as resizing is not implemented.
}

void WaveformDisplay::loadURL(URL audioURL)
{
    // Clear the existing audio thumbnail and attempt to load the audio file from the URL.
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        repaint(); // Repaint the component to display the loaded waveform.
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    // Called when the audio thumbnail data changes. Repaint the component to update the display.
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    // Update the current position and repaint to highlight the new position on the waveform.
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}
