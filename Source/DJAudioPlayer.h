/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 4 Jan 2021 7:30:10pm
    Author:  Zac Bolton

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "PassFilter.h"

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
     From https://docs.juce.com/master/classAudioSource.html "Once playback has finished, the releaseResources() method is called to put the stream back into an 'unprepared' state."
     release memorey resources at end of audio life-cycle
     */
    void releaseResources() override;
    
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

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
