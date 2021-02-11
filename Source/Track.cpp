/*
  ==============================================================================

    Track.cpp
    Created: 6 Feb 2021 4:52:26pm
    Author:  Zac Bolton

  ==============================================================================
*/

#include "Track.h"

Track::Track(juce::String _name,
             juce::String _length,
             juce::URL _url)
    : name(_name),
    length(_length),
    url(_url)
{
}

Track::~Track()
{
}

juce::String Track::getName()
{
    return name;
}

juce::String Track::getLength()
{
    return length;
}

juce::URL Track::getURL()
{
    return url;
}

void Track::setAsResultOfSearch(bool isResultOfSearch)
{
    searchResult = isResultOfSearch;
}

bool Track::isResultOfSearch()
{
    return searchResult;
}
