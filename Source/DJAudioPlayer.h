/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 4 Jan 2021 7:30:10pm
    Author:  Zac Bolton

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class DJAudioPlayer :   public juce::AudioSource
{
public:
    /** constructor */
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    /** destructor */
    ~DJAudioPlayer();
    /**
     From https://docs.juce.com/master/classAudioSource.html "Tells the source to prepare for playing."
     sets up and allocates resources in preparation for audio file playback
     */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /**
     From https://docs.juce.com/master/classAudioSource.html "Called repeatedly to fetch subsequent blocks of audio data."
     recieves and manages chunks of audio in the form of buffers
     */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Called after playback has stopped, to let the object free up any resources it no longer needs."
     release memorey resources at end of audio life-cycle
     */
    void releaseResources() override;
    /** load file from disk for playback */
    void loadURL(juce::URL audioURL);
    /** sets a playback volume for the file between 0 (silent) and 1 (maximum loudness without clipping) */
    void setGain(double gain);
    /** sets a relative playback speed for the file from 0 (stopped) to 100 (100x the normal speed of the file) */
    void setSpeed(double ratio);
    /** sets the position of the playhead to a point in the file in seconds */
    void setPosition(double posInSecs);
    /** sets the position of the playhead to a relative point in the file */
    void setPositionRelative(double pos);
    /** start playing the file */
    void start();
    /** stop playing the file */
    void stop();
    /** get the relative position of the playhead */
    double getPositionRelative() const;
    
    // Final GUI Component Code

    /** update the state of the filter as the user changes parameters */
    void updateFilter(float freq, float res);
    
    // END Final GUI Component Code
    
private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
    // Final GUI Component Code
    
    void reset();

    juce::dsp::StateVariableTPTFilter<float> filter;
    
    // END Final GUI Component Code
};
