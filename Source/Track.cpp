/*
  ==============================================================================

    Track.cpp
    Created: 6 Feb 2021 4:52:26pm
    Author:  Zac Bolton

  ==============================================================================
*/

#include "Track.h"

Track::Track(juce::String _name, double _lengthInSeconds) : name(_name), lengthInSeconds(_lengthInSeconds)
{
}

Track::~Track()
{
}

juce::String Track::getName()
{
    return name;
}
