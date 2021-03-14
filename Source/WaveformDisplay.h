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
    /** inputs: reference to audio format manager (juce::AudioFormatMananger&); reference to audio thumbnail cache (juce::AudioThumbnailCache&)
     constructor */
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);
    /**
     destructor */
    ~WaveformDisplay() override;
    /** inputs: reference to graphics to paint to (juce::Graphics&)
     from https://docs.juce.com/master/classComponent.html#a7cf1862f4af5909ea72827898114a182
     "The paint() method gets called when a region of a component needs redrawing, either because the component's repaint() method has been called, or because something has happened on the screen that means a section of a window needs to be redrawn." */
    void paint (juce::Graphics&) override;
    /**
     from https://docs.juce.com/master/classComponent.html#ad896183a68d71daf5816982d1fefd960
     "Called when this component's size has been changed." */
    void resized() override;
    /** inputs: pointer to change broadcaster (juce::ChangeBroadcaster*)
     from https://docs.juce.com/master/classChangeListener.html#a027420041071315201df11e19a36ea18
     "Your subclass should implement this method to receive the callbac" */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    /** inputs: URL to song to be loaded (juce::URL) | load file from disk for wave form display */
    void loadURL(juce::URL audioURL);
    /** inputs: position of the playhead in seconds (double) | set the relative position of the playhead */
    void setPositionRelative(double pos);
    /** inputs: title of the track to set (string) | set the title to be displayed of the track currently playing */
    void setCurrentTrackTitle(juce::String title);
    /** outputs: title of the current track (string) | get the title to be displayed of the track currently playing */
    juce::String getCurrentTrackTitle();

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    juce::String currentTrackTitle;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
