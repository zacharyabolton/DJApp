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
             juce::URL _url,
             juce::Button::Listener* _listener)
    : name(_name),
    length(_length),
    url(_url),
    listener(_listener)
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

std::vector<juce::TextButton*> Track::getButtons()
{
    std::vector<juce::TextButton*> buttons;
    buttons.push_back(&playLeft);
    buttons.push_back(&playRight);
    buttons.push_back(&remove);
    return buttons;
}
