// Include necessary header files.
#include <JuceHeader.h>
#include "PlayListComponent.h"

// Constructor for PlayListComponent class.
PlayListComponent::PlayListComponent()
{
    // Initialize tableComponent with header and columns.
    tableComponent.setHeaderHeight(30);
    tableComponent.getHeader().addColumn("Track Title", 1, 300);
    tableComponent.getHeader().addColumn("", 2, 100);

    // Set the model for tableComponent to 'this'.
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    // Add initial track names (for demonstration).
    trackNames.add("AXCFDDFF");
    trackNames.add("AXCFDDFF");
    trackNames.add("AXCFDDFF");
    trackNames.add("AXCFDDFF");
    trackNames.add("AXCFDDFF");
    trackNames.add("AXCFDDFF");
}

// Destructor for PlayListComponent class.
PlayListComponent::~PlayListComponent()
{
}

// Paint method for rendering graphics.
void PlayListComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::transparentBlack);
    g.setColour(juce::Colours::transparentBlack);
    g.setFont(18.0f);
    g.drawText("Track List", getLocalBounds(),
        juce::Justification::centred, true);
}

// Resized method for handling component resizing.
void PlayListComponent::resized()
{
    int numRowsToShow = 4;
    int tableHeight = numRowsToShow * getHeight() / 16;
    tableComponent.setBounds(0, 30, getWidth(), getHeight() - 30);
}

// Get the number of rows in the table.
int PlayListComponent::getNumRows()
{
    return trackNames.size();
}

// Paint the background of a row.
void PlayListComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected) {
        g.fillAll(juce::Colours::orange);
    }
    else {
        g.fillAll(juce::Colours::black);
    }
}

// Paint the content of a cell in the table.
void PlayListComponent::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    g.fillAll(juce::Colours::transparentBlack);
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);

    if (columnId == 1)
    {
        g.drawText(trackNames[rowNumber],
            5, 0, width - 10,
            height,
            juce::Justification::centredLeft,
            true);
    }
    else if (columnId == 2)
    {
        // Load play button icon from binary data.
        juce::Image playButtonIcon = juce::ImageCache::getFromMemory(BinaryData::play_icon_png, BinaryData::play_icon_pngSize);

        // Draw the play button icon centered within the cell.
        g.drawImageWithin(playButtonIcon,
            (width - playButtonIcon.getWidth()) / 2,
            (height - playButtonIcon.getHeight()) / 2,
            playButtonIcon.getWidth(),
            playButtonIcon.getHeight(),
            juce::RectanglePlacement::centred);
    }
}

// Refresh or update a component within a cell.
Component* PlayListComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
    if (columnId == 2)
    {
        if (existingComponentToUpdate == nullptr)
        {
            // Create a TextButton component for the "Play" button.
            TextButton* btn = new TextButton{ "Play" };

            // Add a listener to handle button clicks.
            btn->addListener(this);

            // Return the newly created button component.
            existingComponentToUpdate = btn;
        }
        return existingComponentToUpdate;
    }
    return nullptr;
}

// Handle button clicks (e.g., play button click).
void PlayListComponent::buttonClicked(juce::Button* button)
{
    // Handle button clicks here.
}

// Update the list of track names with a new track name.
void PlayListComponent::updateTrackNames(const juce::String& trackName)
{
    trackNames.add(trackName);
    repaint();
}



