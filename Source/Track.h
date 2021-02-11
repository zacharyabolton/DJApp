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
    Track(juce::String name, juce::String length, juce::URL url);
    ~Track();
    
    juce::String getName();
    juce::String getLength();
    juce::URL getURL();
    void setAsResultOfSearch(bool isResultOfSearch);
    bool isResultOfSearch();
    
private:
    juce::String name;
    juce::String length;
    juce::URL url;
    bool searchResult = true;
};
