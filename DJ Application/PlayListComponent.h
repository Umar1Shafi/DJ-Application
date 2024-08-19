#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

class PlayListComponent : public juce::Component,
    public TableListBoxModel,
    public Button::Listener
{
public:
    // Constructor for PlayListComponent class.
    PlayListComponent();

    // Destructor for PlayListComponent class.
    ~PlayListComponent() override;

    // Paint method for rendering graphics.
    void paint(juce::Graphics&) override;

    // Resized method for handling component resizing.
    void resized() override;

    // Get the number of rows in the table.
    int getNumRows() override;

    // Paint the background of a row.
    void paintRowBackground(Graphics&,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;

    // Paint the content of a cell in the table.
    void paintCell(Graphics&,
        int rowNumber,
        int coloumnId,
        int width,
        int height,
        bool rowIsSelected) override;

    // Refresh or update a component within a cell.
    Component* refreshComponentForCell(int rowNumber,
        int coloumnId,
        bool isRowSelected,
        Component* existingComponentToUpdate) override;

    // Handle button clicks (e.g., play button click).
    void buttonClicked(Button* button) override;

    // Update the list of track names with a new track name.
    void updateTrackNames(const juce::String& trackName);

private:
    TableListBox tableComponent;        // The table component for displaying track names.
    juce::StringArray trackNames;       // An array to store track names.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayListComponent)
};





