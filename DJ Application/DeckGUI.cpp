#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

using namespace juce;

//==============================================================================
// DeckGUI constructor
DeckGUI::DeckGUI(DJAudioPlayer* player, 
                AudioFormatManager& formatManagerToUse, 
                AudioThumbnailCache& cacheToUse)
                : audioPlayer(player),
                waveformDisplay(formatManagerToUse, cacheToUse),
                fChooser("Select a File to Load", File(), "*", true),
                playButton("PlayButton", DrawableButton::ImageFitted)
{
    // Load play and pause button images
    playImage = ImageFileFormat::loadFrom(BinaryData::play_button_png, BinaryData::play_button_pngSize);
    pauseImage = ImageFileFormat::loadFrom(BinaryData::pause_button_png, BinaryData::pause_button_pngSize);

    // Create play button and set images
    Image playImage = ImageFileFormat::loadFrom(BinaryData::play_button_png, BinaryData::play_button_pngSize);
    DrawableImage playDrawable(playImage);
    playButton.setImages(&playDrawable);
    playButton.setButtonText("Play");
    addAndMakeVisible(playButton);
    playButton.toFront(true);
    playButton.setEnabled(true);

    // Volume Slider setup
    volSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    volSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    volSlider.setRange(0.0, 2.0, 0.01);
    volSlider.setValue(0.1);
    volSlider.addListener(this);
    volSlider.setColour(Slider::thumbColourId, Colours::red);
    addAndMakeVisible(volSlider);

    // Attach a label to the volume slider
    volLabel.setText("Volume", dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);
    volLabel.setJustificationType(Justification::centred);
    volLabel.setFont(Font(18.0f));
    addAndMakeVisible(volLabel);

    // Speed Slider setup
    speedSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    speedSlider.setRange(0.0, 4.0, 0.1);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    speedSlider.setColour(Slider::thumbColourId, Colours::red);
    addAndMakeVisible(speedSlider);

    // Attach a label to the speed slider
    speedLabel.setText("Speed", dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setJustificationType(Justification::centred);
    speedLabel.setFont(Font(18.0f));
    addAndMakeVisible(speedLabel);

    // Position Slider setup
    posSlider.setRange(0.0, 1.0);
    posSlider.addListener(this);
    posSlider.setColour(Slider::thumbColourId, Colours::red);
    posSlider.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(posSlider);

    // Add buttons and waveform display
    addAndMakeVisible(playButton1);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(waveformDisplay);
    playButton1.addListener(this);
    loopButton.addListener(this);
    loadButton.addListener(this);
    speedSlider.addListener(this);

    // Set decimal places for volume slider
    volSlider.setNumDecimalPlacesToDisplay(2);

    // Call resized() and start the timer
    resized();
    startTimer(500);
}

// DeckGUI destructor
DeckGUI::~DeckGUI()
{
    stopTimer();
}

// Paint method
void DeckGUI::paint(Graphics& g)
{
    g.setColour(Colours::transparentBlack);
    g.drawRect(getLocalBounds(), 1);
    g.setColour(Colours::lightgrey);
    g.setFont(14.0f);
}

// Resize method
void DeckGUI::resized()
{
    const int buttonWidth = 80;
    const int buttonHeight = 30;
    const int sliderWidth = 150;
    const int sliderHeight = 100;
    const int waveformHeight = getHeight() * 0.4;
    const int margin = 5;

    // Position the components

    // Set the position and size of playButton1
    playButton1.setBounds(margin + buttonWidth * 2 + 10, getHeight() / 2 + 100, buttonWidth, buttonHeight);

    // Set the position and size of loadButton
    loadButton.setBounds(margin + buttonWidth * 2 + 10, getHeight() / 2 + 145, buttonWidth, buttonHeight);

    // Set the position and size of loopButton
    loopButton.setBounds(margin + buttonWidth * 2 + 10, getHeight() / 2 + 190, buttonWidth, buttonHeight);

    // Set the position and size of volSlider
    volSlider.setBounds(margin, getHeight() - margin - sliderHeight * 2 - 30, sliderWidth, sliderHeight * 2);

    // Set the position and size of speedSlider
    speedSlider.setBounds(getWidth() - sliderWidth - 80, getHeight() - margin - sliderHeight * 2 - 30, sliderWidth, sliderHeight * 2);

    // Set the position and size of posSlider
    posSlider.setBounds(margin, getHeight() - 30, sliderWidth * 3, sliderHeight / 4);

    // Set the position and size of waveformDisplay
    waveformDisplay.setBounds(10, 10, getWidth() - 20, getHeight() * 0.4);

    // Set the position and size of playButton
    playButton.setBounds(margin + buttonWidth * 2, getHeight() / 2 - 20, 100, 100);
}

// Button click event handler
void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton1)
    {
        if (audioPlayer->isPlaying())
        {
            togglePlayPause();
            playButton1.setButtonText("Play");
            audioPlayer->stop();
        }
        else
        {
            playButton1.setButtonText("Stop");
            togglePlayPause();
            audioPlayer->start();
        }
    }
    else if (button == &loadButton)
    {
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                File selectedFile = chooser.getResult();
                String fileName = selectedFile.getFileName();
                audioPlayer->loadURL(URL{ chooser.getResult() });
                waveformDisplay.loadURL(URL{ chooser.getResult() });
                //playListComponent.updateTrackNames(fileName);
            });
    }
}

// Slider value change event handler
void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        audioPlayer->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        audioPlayer->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        audioPlayer->setPositionRelative(slider->getValue());
    }
}

// Check if interested in file drag
bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    return true;
}

// Handle file drag and drop
void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    static int fileCounter = 0;

    if (files.size() == 1)
    {
        int fileNumber = fileCounter++;

        if (fileNumber % 2 == 0) {
            audioPlayer->loadURL(URL{ File{files[0]} });
        }
        else {
            audioPlayer->loadURL(URL{ File{files[0]} });
        }
    }
}

// Timer callback
void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(
        audioPlayer->getPositionRelative());
}

// Toggle Play/Pause button
void DeckGUI::togglePlayPause()
{
    if (audioPlayer->isPlaying())
    {
        DrawableImage pauseDrawable(pauseImage);
        playButton.setImages(&pauseDrawable);
        playButton.setButtonText("Pause");
        audioPlayer->stop();
    }
    else
    {
        DrawableImage playDrawable(playImage);
        playButton.setImages(&playDrawable);
        playButton.setButtonText("Play");
        audioPlayer->start();
    }
}

// Getters for buttons and sliders
TextButton& DeckGUI::getLoadButton()
{
    return loadButton;
}

TextButton& DeckGUI::getPlayButton()
{
    return playButton1;
}

TextButton& DeckGUI::getLoopButton()
{
    return loopButton;
}

Slider& DeckGUI::getVolSlider()
{
    return volSlider;
}

Slider& DeckGUI::getPosSlider()
{
    return posSlider;
}
