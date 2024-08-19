#include "LoopController.h"

// Constructor for LoopController
LoopController::LoopController() : loopEnabled(false), loopStart(0.0), loopEnd(0.0)
{
}

// Destructor for LoopController
LoopController::~LoopController()
{
}

// Set the loop start time
void LoopController::setLoopStart(double startTime)
{
    loopStart = startTime;
}

// Set the loop end time
void LoopController::setLoopEnd(double endTime)
{
    loopEnd = endTime;
}

// Toggle the loop state
void LoopController::toggleLoop()
{
    loopEnabled = !loopEnabled;
    if (!loopEnabled)
    {
        resetLoop();
    }
}

// Reset the loop (no operation in this implementation)
void LoopController::resetLoop()
{
    // No operation for resetting loop in this implementation
}

// Check if the loop is enabled
bool LoopController::isLoopEnabled() const
{
    return loopEnabled;
}

// Get the loop start time
double LoopController::getLoopStart() const
{
    return loopStart;
}

// Get the loop end time
double LoopController::getLoopEnd() const
{
    return loopEnd;
}
