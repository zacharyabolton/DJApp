/*
  ==============================================================================

    DeckGUI.h
    Created: 5 Jan 2021 5:45:42pm
    Author:  Zac Bolton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "RotaryDialLookAndFeel.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** Impliment Button::Listener */
    void buttonClicked(juce::Button* button) override;
    
    /** Impliment Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;
    
    /** Impliment FileDragAndDropTarget */
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    
    void timerCallback() override;
    
    WaveformDisplay waveformDisplay;

private:
    
    juce::TextButton playButton{"PLAY"};
    juce::TextButton stopButton{"STOP"};
    juce::TextButton loadButton{"LOAD"};
    
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    
    DJAudioPlayer* player;
    
    // Final GUI Component Code
    // dials for the pass filter
    RotaryDialLookAndFeel freqDialLookAndFeel{"Cutoff"};
    RotaryDialLookAndFeel resDialLookAndFeel{"Resonance"};
    
    juce::Slider freqDial;
    juce::Slider resDial;
    // END Final GUI Component Code
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
