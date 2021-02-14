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
    /**
     constructor */
    Track(juce::String name, juce::String length, juce::URL url);
    /**
     destructor */
    ~Track();
    /**
     returns the name of the track i.e. the the last section of the
     pathname without extension */
    juce::String getName();
    /**
     returns the length in the form "MM:SS" (minutes and seconds) */
    juce::String getLength();
    /**
     returns the path of the track's file */
    juce::URL getURL();
    /**
     set to true if currently part of a search, false if not */
    void setAsResultOfSearch(bool isResultOfSearch);
    /**
     returns whether or not this track is part of a
     current search query */
    bool isResultOfSearch();
    
private:
    juce::String name;
    juce::String length;
    juce::URL url;
    bool searchResult = true;
};
