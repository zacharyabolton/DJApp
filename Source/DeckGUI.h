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
                 public juce::Timer
{
public:
    /**
     constructor */
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    /**
     destructor */
    ~DeckGUI() override;
    /**
     from https://docs.juce.com/master/classComponent.html#a7cf1862f4af5909ea72827898114a182
     "The paint() method gets called when a region of a component needs redrawing, either because the component's repaint() method has been called, or because something has happened on the screen that means a section of a window needs to be redrawn." */
    void paint (juce::Graphics&) override;
    /**
     from https://docs.juce.com/master/classComponent.html#ad896183a68d71daf5816982d1fefd960
     "Called when this component's size has been changed." */
    void resized() override;
    /**
     from https://docs.juce.com/master/classButton_1_1Listener.html#a81499cef24b7189cd0d1581fd9dc9e14
     "Called when the button is clicked." */
    void buttonClicked(juce::Button* button) override;
    /**
     from https://docs.juce.com/master/classSlider_1_1Listener.html#a127bfe68835dc3e584cf3c2a427a27e5
     "Called when the slider's value is changed." */
    void sliderValueChanged(juce::Slider* slider) override;
    /**
     from https://docs.juce.com/master/classTimer.html#a8adc40ca0fb4170737ba12e30481b9d8
     "The user-defined callback routine that actually gets called periodically." */
    void timerCallback() override;
    /** wave form display GUI - exposed for use by playlist component */
    WaveformDisplay waveformDisplay;

private:
    
    juce::TextButton playButton{"PLAY"};
    juce::TextButton stopButton{"STOP"};
    
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    
    DJAudioPlayer* player;
    
    RotaryDialLookAndFeel freqDialLookAndFeel{"Cutoff"};
    RotaryDialLookAndFeel resDialLookAndFeel{"Resonance"};
    
    juce::Slider freqDial;
    juce::Slider resDial;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
