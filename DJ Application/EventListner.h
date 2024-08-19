#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"

// Declaration of the KeyboardController class
class KeyboardController : public KeyListener,
    public FocusChangeListener,
    public Component,
    public ApplicationCommandTarget
{
public:
    // Constructor for KeyboardController
    KeyboardController(DJAudioPlayer* player1, DJAudioPlayer* player2);

    // Key press event handler
    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;

    // Key state change event handler
    bool keyStateChanged(bool isKeyDown, Component* originatingComponent) override;

    // Focus gained event handler
    void focusGained(FocusChangeType cause) override;

    // Focus lost event handler
    void focusLost(FocusChangeType cause) override;

    // Global focus changed event handler
    void globalFocusChanged(Component* focusedComponent) override;

    // Perform a command
    bool perform(const InvocationInfo& info) override;

    // Get all available commands
    void getAllCommands(Array<CommandID>& commands) override;

    // Get information about a specific command
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;

    // Get the next command target
    ApplicationCommandTarget* getNextCommandTarget() override;

private:
    DJAudioPlayer* audioPlayer1;
    DJAudioPlayer* audioPlayer2;
    int activeDeckIndex;
    double gain1 = 1.0;
    double gain2 = 1.0;
    double position1 = 0.0;
    double position2 = 0.0;
};
