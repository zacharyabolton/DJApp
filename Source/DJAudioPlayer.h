/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 4 Jan 2021 7:30:10pm
    Author:  Zac Bolton

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class DJAudioPlayer :   public juce::AudioSource,
                        public juce::AudioProcessor
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
    
    // final project gui component code
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Returns the name of this processor."
     */
    const juce::String getName() const override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Renders the next block.
     
     When this method is called, the buffer contains a number of channels which is at least as great as the maximum number of input and output channels that this processor is using. It will be filled with the processor's input data and should be replaced with the processor's output."
     */
    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override;
    // END final project gui component code
    
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
    
    // final project gui component code
    /** update the state of the filter as the user changes parameters */
    void updateFilter(double freq, double res);
    // END final project gui component code
    
private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};

    // final project gui component code
    void reset() override;
    
    juce::dsp::StateVariableTPTFilter<float> filter;
    //juce::dsp::stateVariableTPTFilter<float> filter;
    
//    juce::dsp::ProcessorDuplicator<
//        juce::dsp::StateVariableFilter::Filter<double>,
//        juce::dsp::StateVariableFilter::Parameters<double>
//    > stateVariableFilter;
//    
//    double lastSampleRate;
    // END final project gui component code
};
