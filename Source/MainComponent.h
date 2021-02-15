#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    /**
     constructor */
    MainComponent();
    /**
     destructor */
    ~MainComponent() override;

    //==============================================================================
    /**
     from https://docs.juce.com/master/classAudioAppComponent.html#aa98326bd4ae4bd9bdc95e6d45d57e60c
     "Tells the source to prepare for playing." */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /**
     from https://docs.juce.com/master/classAudioAppComponent.html#a912d68c09e3ac5bd7a3dbc35065c0844
     "Called repeatedly to fetch subsequent blocks of audio data." */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    /**
     from https://docs.juce.com/master/classAudioAppComponent.html#a76666e730166d2db46ada4074d5858e5
     "Allows the source to release anything it no longer needs after playback has stopped." */
    void releaseResources() override;

    //==============================================================================
    /**
     from https://docs.juce.com/master/classComponent.html#a7cf1862f4af5909ea72827898114a182
     "Components can override this method to draw their content." */
    void paint (juce::Graphics& g) override;
    /**
     from https://docs.juce.com/master/classComponent.html#ad896183a68d71daf5816982d1fefd960
     "Called when this component's size has been changed." */
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{100};

    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbCache};
    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2, formatManager, thumbCache};
    
    juce::MixerAudioSource mixerSource;
    
    PlaylistComponent playlistComponent{&player1, &player2, &deckGUI1.waveformDisplay, &deckGUI2.waveformDisplay, formatManager};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    
};
