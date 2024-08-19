#include "MainComponent.h"

MainComponent::MainComponent() : deck1Active(true),
                                gain1(1.0),
                                gain2(1.0),
                                deck1Playing(false),
                                deck2Playing(false),
                                currentPosition(0.0)
{
    // Set the initial size of the MainComponent
    setSize(1000, 700);

    // Request microphone permission if required
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio) &&
        !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
            [&](bool granted) { if (granted)  setAudioChannels(2, 2); });
    }
    else
    {
        setAudioChannels(0, 2);
    }

    // Create and add GUI components
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playListComponent);

    // Register basic audio formats
    formatManager.registerBasicFormats();

    // Enable keyboard focus for this component
    setWantsKeyboardFocus(true);

    // Add a key listener for this component
    addKeyListener(this);

    // Create and add a KeyboardController
    keyboardController.reset(new KeyboardController(&player1, &player2));
    addKeyListener(keyboardController.get());

    // Add listeners for load and loop buttons in deck GUIs
    deckGUI1.getLoadButton().addListener(this);
    deckGUI2.getLoadButton().addListener(this);
    deckGUI1.getLoopButton().addListener(this);
    deckGUI2.getLoopButton().addListener(this);
}

MainComponent::~MainComponent()
{
    // Remove the key listener when the component is destroyed
    removeKeyListener(keyboardController.get());

    // Shutdown the audio system
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Prepare the audio players and mixer source
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    // Add the audio players to the mixer source
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

    // Store the sample rate
    this->sampleRate = sampleRate;

    // Prepare the transport sources
    transportSource1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    transportSource2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    double sampleRate = this->sampleRate;

    // Get the next audio block from transport sources
    transportSource1.getNextAudioBlock(bufferToFill);
    transportSource2.getNextAudioBlock(bufferToFill);

    // Check if deck 1 is active
    if (deck1Active)
    {
        // Check if looping is enabled in deck 1
        if (loopController1.isLoopEnabled())
        {
            // Check if the playhead has reached the end of the loop
            if (player1.getPositionRelative() >= 1.0)
            {
                // Reset the playhead position and restart playback
                position1 = 0.0;
                player1.setPositionRelative(position1);
                player1.start();
            }
        }
    }
    else
    {
        // Check if looping is enabled in deck 2
        if (loopController2.isLoopEnabled())
        {
            // Check if the playhead has reached the end of the loop
            if (player2.getPositionRelative() >= 1.0)
            {
                // Reset the playhead position and restart playback
                position2 = 0.0;
                player2.setPositionRelative(position2);
                player2.start();
            }
        }
    }

    // Mix the audio from both players
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // Release audio resources for players and mixer source
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

void MainComponent::paint(Graphics& g)
{
    // Load and draw the background image
    background = juce::ImageCache::getFromMemory(BinaryData::background_jpg, BinaryData::background_jpgSize);
    g.drawImageWithin(background, 0, 0, 1000, 800, juce::RectanglePlacement::stretchToFit);
}

void MainComponent::resized()
{
    // Set the bounds for GUI components
    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() * 0.75);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() * 0.75);
    playListComponent.setBounds(0, getHeight() * 0.75, getWidth(), getHeight() * 0.25);
}

bool MainComponent::keyPressed(const KeyPress& key, Component* originatingComponent)
{
    // Determine which deck GUI is active
    deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;

    // Handle keyboard shortcuts
    if (key == KeyPress::tabKey)
    {
        // Toggle the active deck
        deck1Active = !deck1Active;
        return true;
    }
    if (key == KeyPress::upKey)
    {
        // Increase the gain for the active deck
        if (deck1Active)
        {
            gain1 += 0.05;
            gain1 = std::min(1.0, gain1);
            player1.setGain(gain1);
            deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
            deckGUI->getVolSlider().setValue(gain1);
        }
        else
        {
            gain2 += 0.05;
            gain2 = std::min(1.0, gain2);
            player2.setGain(gain2);
            deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
            deckGUI->getVolSlider().setValue(gain2);
        }
        return true;
    }
    else if (key == KeyPress::downKey)
    {
        // Decrease the gain for the active deck
        if (deck1Active)
        {
            gain1 -= 0.05;
            gain1 = std::max(0.0, gain1);
            player1.setGain(gain1);
            deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
            deckGUI->getVolSlider().setValue(gain1);
        }
        else
        {
            gain2 -= 0.05;
            gain2 = std::max(0.0, gain2);
            player2.setGain(gain2);
            deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
            deckGUI->getVolSlider().setValue(gain2);
        }
        return true;
    }
    if (key == KeyPress::rightKey)
    {
        // Move the playhead position forward for the active deck
        if (deck1Active)
        {
            position1 = 0.010;
            position1 = std::min(1.0, position1);
            player1.setPositionRelative(player1.getPositionRelative() + position1);
            deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
            deckGUI->getPosSlider().setValue(player1.getPositionRelative() + position1);

            // Check if looping is enabled and the playhead reached the end
            if (player1.getPositionRelative() >= 1.0 && loopController1.isLoopEnabled())
            {
                // Reset the playhead position and restart playback
                position1 = 0.0;
                player1.setPositionRelative(position1);
                player1.start();
                deckGUI->getPosSlider().setValue(player1.getPositionRelative() + position1);
            }
        }
        else
        {
            position2 = 0.010;
            position2 = std::min(1.0, position2);
            player2.setPositionRelative(player2.getPositionRelative() + position2);
            deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
            deckGUI->getPosSlider().setValue(player2.getPositionRelative() + position2);

            // Check if looping is enabled and the playhead reached the end
            if (player2.getPositionRelative() >= 1.0 && loopController2.isLoopEnabled())
            {
                // Reset the playhead position and restart playback
                position2 = 0.0;
                player2.setPositionRelative(position2);
                player2.start();
                deckGUI->getPosSlider().setValue(player2.getPositionRelative() + position2);
            }
        }
        return true;
    }
    else if (key == KeyPress::leftKey)
    {
        // Move the playhead position backward for the active deck
        if (deck1Active)
        {
            position1 = 0.010;
            position1 = std::max(0.0, position1);
            player1.setPositionRelative(player1.getPositionRelative() - position1);
            deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
            deckGUI->getPosSlider().setValue(player1.getPositionRelative() - position1);
        }
        else
        {
            position2 = 0.010;
            position2 = std::max(0.0, position2);
            player2.setPositionRelative(player2.getPositionRelative() - position2);
            deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
            deckGUI->getPosSlider().setValue(player2.getPositionRelative() - position2);
        }
        return true;
    }
    if (key == KeyPress::spaceKey)
    {
        // Toggle play/pause for the active deck
        if (deck1Active)
        {
            if (deck1Playing)
            {
                player1.stop();
                deckGUI1.togglePlayPause();
                deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
                deckGUI->getPlayButton().setButtonText("Stop");
            }
            else
            {
                player1.start();
                deckGUI1.togglePlayPause();
                deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
                deckGUI->getPlayButton().setButtonText("Play");
            }
            deck1Playing = !deck1Playing;
        }
        else
        {
            if (deck2Playing)
            {
                player2.stop();
                deckGUI2.togglePlayPause();
                deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
                deckGUI->getPlayButton().setButtonText("Stop");
            }
            else
            {
                player2.start();
                deckGUI2.togglePlayPause();
                deckGUI = deck1Active ? &deckGUI1 : &deckGUI2;
                deckGUI->getPlayButton().setButtonText("Play");
            }
            deck2Playing = !deck2Playing;
        }
        return true;
    }
    if (key == KeyPress('l'))
    {
        // Toggle loop for the active deck
        if (deck1Active)
        {
            if (loopController1.isLoopEnabled())
            {
                loopController1.toggleLoop();
                deckGUI = &deckGUI1;
                deckGUI->getLoopButton().setButtonText("Loop");
            }
            else
            {
                loopController1.toggleLoop();
                deckGUI = &deckGUI1;
                deckGUI->getLoopButton().setButtonText("No Loop");
            }
        }
        else
        {
            if (loopController2.isLoopEnabled())
            {
                loopController2.toggleLoop();
                deckGUI = &deckGUI2;
                deckGUI->getLoopButton().setButtonText("Loop");
            }
            else
            {
                loopController2.toggleLoop();
                deckGUI = &deckGUI2;
                deckGUI->getLoopButton().setButtonText("No Loop");
            }
        }
        return true;
    }
    return false;
}

bool MainComponent::keyStateChanged(bool isKeyDown, Component* originatingComponent)
{
    // Handle key state changes if needed
    return true;
}

void MainComponent::buttonClicked(Button* button)
{
    // Handle button clicks for load and loop buttons in deck GUIs
    if (deck1Active)
    {
        if (button == &deckGUI1.getLoopButton())
        {
            // Toggle the loop state for deck 1
            loopController1.toggleLoop();

            // Update the button text based on the loop state
            deckGUI1.getLoopButton().setButtonText(loopController1.isLoopEnabled() ? "No Loop" : "Loop");
        }
    }
    else
    {
        if (button == &deckGUI2.getLoopButton())
        {
            // Toggle the loop state for deck 2
            loopController2.toggleLoop();

            // Update the button text based on the loop state
            deckGUI2.getLoopButton().setButtonText(loopController2.isLoopEnabled() ? "No Loop" : "Loop");
        }
    }
}

void MainComponent::timerCallback()
{
    MessageManagerLock mmLock;

    // Update the current position of the active deck
    if (deck1Active)
    {
        currentPosition = player1.getPositionRelative();
    }
    else
    {
        currentPosition = player2.getPositionRelative();
    }

    // Update the position slider in the active deck's GUI
    if (deck1Active)
    {
        deckGUI1.getPosSlider().setValue(currentPosition, NotificationType::dontSendNotification);
    }
    else
    {
        deckGUI2.getPosSlider().setValue(currentPosition, NotificationType::dontSendNotification);
    }
}



