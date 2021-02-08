/*
  ==============================================================================

    Track.h
    Created: 6 Feb 2021 4:52:26pm
    Author:  Zac Bolton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>

class Track {
public:
    Track(juce::String name, juce::String length, juce::URL url, juce::Button::Listener* listener);
    ~Track();
    
    juce::String getName();
    juce::String getLength();
    juce::URL getURL();
    std::vector<juce::TextButton*> getButtons();

private:
    juce::String name;
    juce::String length;
    juce::URL url;
    juce::TextButton playLeft{"play left"};
    juce::TextButton playRight{"play right"};
    juce::TextButton remove{"x"};
    juce::Button::Listener* listener;
};
