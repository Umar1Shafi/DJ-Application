// Include necessary header files.
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlayListComponent.h"
#include "EventListner.h"
#include "LoopController.h"

// MainComponent class definition inheriting multiple JUCE base classes.
class MainComponent : public KeyListener,
                    public AudioAppComponent,
                    public Button::Listener,
                    public Timer
{
public:
    // Constructor for the MainComponent class.
    MainComponent();

    // Destructor for the MainComponent class.
    ~MainComponent();

    // AudioAppComponent interface: Prepares the audio engine for playback.
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    // AudioAppComponent interface: Fills the audio buffer with audio data.
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    // AudioAppComponent interface: Releases audio-related resources.
    void releaseResources() override;

    // Paint method for rendering graphics.
    void paint(Graphics& g) override;

    // Resized method for handling component resizing.
    void resized() override;

    // KeyListener interface: Handles key press events.
    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;

    // KeyListener interface: Handles key state change events.
    bool keyStateChanged(bool isKeyDown, Component* originatingComponent) override;

    // Button::Listener interface: Handles button click events.
    void buttonClicked(Button* button) override;

    // Timer callback for periodic updates.
    void timerCallback() override;

    void sliderValueChanged(Slider* slider);

private:
    // AudioFormatManager for managing audio formats.
    AudioFormatManager formatManager;

    // AudioThumbnailCache for caching audio thumbnail data.
    AudioThumbnailCache thumbCache{ 100 };

    // DJAudioPlayer instance for the first audio player.
    DJAudioPlayer player1{ formatManager };

    // DeckGUI instance for the first deck's graphical interface.
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache };

    // DJAudioPlayer instance for the second audio player.
    DJAudioPlayer player2{ formatManager };

    // DeckGUI instance for the second deck's graphical interface.
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache };

    // MixerAudioSource for mixing audio from both players.
    MixerAudioSource mixerSource;

    // PlayListComponent for managing and displaying playlists.
    PlayListComponent playListComponent;

    // AudioDeviceManager for audio device management.
    AudioDeviceManager audioDeviceManager;

    // LoopController for controlling audio loop functionality in deck 1.
    LoopController loopController1;

    // LoopController for controlling audio loop functionality in deck 2.
    LoopController loopController2;

    // Pointer to the active DeckGUI instance.
    DeckGUI* deckGUI;

    // AudioTransportSource for deck 1.
    AudioTransportSource transportSource1;

    // AudioTransportSource for deck 2.
    AudioTransportSource transportSource2;

    // String to store the name of the loaded audio file.
    String loadedAudioFileName;

    // Pointer to KeyboardController for handling keyboard events.
    std::unique_ptr<KeyboardController> keyboardController;


    // Image for the background.
    Image background;

    // Variables for deck 1 state and properties.
    bool deck1Active;
    Slider pitchSlider;
    double gain1;
    double gain2;
    bool deck1Playing;
    bool deck2Playing;
    double position1 = 0.0;
    double position2 = 0.0;
    double sampleRate;
    double currentPosition;
    // JUCE macro to declare non-copyable class with leak detector.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
