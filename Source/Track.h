/*
  ==============================================================================

    Track.h
    Created: 6 Feb 2021 4:52:26pm
    Author:  Zac Bolton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Track {
public:
    Track(juce::String name, double lengthInSeconds);
    ~Track();
    
    juce::String getName();

private:
    juce::String name;
    double lengthInSeconds;
};
