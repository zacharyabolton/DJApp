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
    /** inputs: reference to the audio format manager (juce::AudioFormatManager&)
     constructor */
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    /**
     destructor */
    ~DJAudioPlayer();
    /** inputs: expected number of samples per audio block [buffer] (int); "the sample rate that the output will be used at - this is needed by sources such as tone generators." (double)
     From https://docs.juce.com/master/classAudioSource.html "Tells the source to prepare for playing."
     sets up and allocates resources in preparation for audio file playback */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /** inputs: reference to the target channel buffer (juce::AudioSourceChannelInfo&)
     From https://docs.juce.com/master/classAudioSource.html "Called repeatedly to fetch subsequent blocks of audio data."
     recieves and manages chunks of audio in the form of buffers */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Called after playback has stopped, to let the object free up any resources it no longer needs."
     release memorey resources at end of audio life-cycle */
    void releaseResources() override;
    /** inputs: URL to audio file to be loaded (juce::URL) | load file from disk for playback */
    void loadURL(juce::URL audioURL);
    /** inputs: relative gain for output - between 0 [mute] and 1 [full volume] (double) | sets a playback volume for the file between 0 (silent) and 1 (maximum loudness without clipping) */
    void setGain(double gain);
    /** inputs: relative speed for output - with 1.0 being normal speed (double) | sets a relative playback speed for the file from 0.1 (10% normal speed of the file) to 2.0 (200% the normal speed of the file) */
    void setSpeed(double ratio);
    /** inputs: absolute position of the current moment in playback - in seconds (double) | sets the position of the playhead to a point in the file in seconds */
    void setPosition(double posInSecs);
    /** inputs: relative position of the current moment in playback - with 0 being the start and 1 being the end (double) | sets the position of the playhead to a relative point in the file */
    void setPositionRelative(double pos);
    /** start playing the file */
    void start();
    /** stop playing the file */
    void stop();
    /** outputs: relative position of the current moment in playback - with 0 being the start and 1 being the end (double) | get the relative position of the playhead */
    double getPositionRelative() const;
    /** inputs: the frequency for which bandwidths below are to be removed (double); the desired resonance (float) | update the state of the filter as the user changes parameters */
    void updateFilter(float freq, float res);

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};

    void reset();

    juce::dsp::StateVariableTPTFilter<float> filter;
};
