#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlayListComponent.h"
#include "LoopController.h"

class DeckGUI : public Component,
    public Button::Listener,
    public Slider::Listener,
    public FileDragAndDropTarget,
    public Timer
{
public:
    // Constructor
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager& formatManagerToUse,
            AudioThumbnailCache& cacheToUse);

    // Destructor
    ~DeckGUI();

    // Paint method override
    void paint(Graphics&) override;

    // Resized method override
    void resized() override;

    // Button click event handler
    void buttonClicked(Button*) override;

    // Slider value change event handler
    void sliderValueChanged(Slider* slider) override;

    // Check if interested in file drag
    bool isInterestedInFileDrag(const StringArray& files) override;

    // Handle file drag and drop
    void filesDropped(const StringArray& files, int x, int y) override;

    // Toggle Play/Pause button
    void togglePlayPause();

    // Timer callback
    void timerCallback() override;

    // Getters for buttons and sliders
    TextButton& getLoadButton();
    TextButton& getPlayButton();
    TextButton& getLoopButton();
    Slider& getVolSlider();
    Slider& getPosSlider();

private:
    // Buttons and Controls
    TextButton playButton1{ "PLAY/STOP" };
    TextButton loadButton{ "LOAD" };
    DrawableButton playButton;
    TextButton loopButton{ "Loop" };
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;
    Label volLabel;
    Label speedLabel;
    
    // Waveform Display
    WaveformDisplay waveformDisplay;

    // Audio Player
    DJAudioPlayer* audioPlayer;

    // Play List Component
    PlayListComponent playListComponent;

    // Loop Controller
    LoopController loopcontroller;

    // Images for Play and Pause buttons
    Image playImage, pauseImage;

    // Flag to track if audio is playing
    bool isPlaying = false;

    // File Chooser for loading audio files
    FileChooser fChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
