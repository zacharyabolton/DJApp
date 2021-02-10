/*
  ==============================================================================

    WaveformDisplay.h
    Created: 7 Jan 2021 11:30:39am
    Author:  Zac Bolton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  :    public juce::Component,
                            public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    void loadURL(juce::URL audioURL);
    
    /** set the relative position of the playhead */
    void setPositionRelative(double pos);
    
    /** set the title to be displayed of the track currently playing */
    void setCurrentTrackTitle(juce::String title);
    /** get the title to be displayed of the track currently playing */
    juce::String getCurrentTrackTitle();

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    juce::String currentTrackTitle;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
