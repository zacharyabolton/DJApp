/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 7 Jan 2021 11:30:39am
    Author:  Zac Bolton

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse)
    : audioThumb(1000, formatManagerToUse, cacheToUse),
    fileLoaded(false),
    position(0)
{
    // attach listener
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    // draw an outline around the component
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    // draw the actual wave form in orange
    g.setColour (juce::Colours::orange);
    if (fileLoaded)
    {
        // draw one of the channel's wave form
        // (not neccessary to process both)
        audioThumb.drawChannel(
           g,
           getLocalBounds(),
           0.0,
           audioThumb.getTotalLength(),
           0,
           1.0f
        );
        // draw playhead indicator
        g.setColour(juce::Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, 0.01 * getWidth(), getHeight());
        g.setColour(juce::Colours::black);
        // draw the title of the track which is loaded into wave form display
        g.drawText (currentTrackTitle, getLocalBounds(),
                    juce::Justification::centred, true);
    }
    else {
        // inform the user that nothing is yet loaded into wave form display
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                    juce::Justification::centred, true);
    }
    
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    // clear out any old junk data from the wave form display cache
    audioThumb.clear();
    // set the source of the wave form display cache and store
    // whether it was a sucess or not
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    // if a change is detected, repaint the wave form display
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    // set position of the play head animation to
    // it's correct relative position over the wave form
    // display based on the play head's position in seconds
    if (pos != position && pos > 0.0)
    {
        position = pos;
        repaint();
    }
}

void WaveformDisplay::setCurrentTrackTitle(juce::String title)
{
    // update the track title text over the wave form display
    // in case of new track load from playlist component
    currentTrackTitle = title;
}

juce::String WaveformDisplay::getCurrentTrackTitle()
{
    // return the title of the track currently loaded
    // into the wave form display
    return currentTrackTitle;
}
