#pragma once

// LoopController class for managing audio looping
class LoopController
{
public:
    // Constructor
    LoopController();

    // Destructor
    ~LoopController();

    // Set the loop start time
    void setLoopStart(double startTime);

    // Set the loop end time
    void setLoopEnd(double endTime);

    // Toggle the loop state
    void toggleLoop();

    // Reset the loop
    void resetLoop();

    // Check if the loop is enabled
    bool isLoopEnabled() const;

    // Get the loop start time
    double getLoopStart() const;

    // Get the loop end time
    double getLoopEnd() const;

private:
    bool loopEnabled;   // Flag to indicate if looping is enabled
    double loopStart;   // Start time of the loop in seconds
    double loopEnd;     // End time of the loop in seconds
};
