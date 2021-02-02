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
    /**
     From https://docs.juce.com/master/classAudioSource.html "Called repeatedly to fetch subsequent blocks of audio data."
     recieves and manages chunks of audio in the form of buffers
     */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
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

    //========================================================================
    //= PURE VIRTUAL MEMBER FUNCTIONS from juce::AudioProcessor class
    //========================================================================
    
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Called before playback starts, to let the processor prepare itself."
     sets up and allocates resources in preparation for audio file playback
     */
    void prepareToPlay (double sampleRate, int samplesPerBlockExpected) override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Called after playback has stopped, to let the object free up any resources it no longer needs."
     release memorey resources at end of audio life-cycle
     */
    void releaseResources() override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Renders the next block.
     When this method is called, the buffer contains a number of channels which is at least as great as the maximum number of input and output channels that this processor is using. It will be filled with the processor's input data and should be replaced with the processor's output."
     */
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Creates the processor's GUI."
     */
    juce::AudioProcessorEditor* createEditor() override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Your processor subclass must override this and return true if it can create an editor component."
     */
    bool hasEditor() const override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Returns the name of this processor."
     */
    const juce::String getName() const override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Returns true if the processor wants MIDI messages."
     */
    bool acceptsMidi() const override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Returns true if the processor produces MIDI messages."
     */
    bool producesMidi() const override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Returns the length of the processor's tail, in seconds."
     */
    double getTailLengthSeconds() const override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Returns the number of preset programs the processor supports.
     The value returned must be valid as soon as this object is created, and must not change over its lifetime.
     This value shouldn't be less than 1."
     */
    int getNumPrograms() override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Returns the number of the currently active program."
     */
    int getCurrentProgram() override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Called by the host to change the current program."
     */
    void setCurrentProgram (int index) override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Must return the name of a given program."
     */
    const juce::String getProgramName (int index) override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Called by the host to rename a program."
     */
    void changeProgramName (int index, const juce::String& newName) override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "The host will call this method when it wants to save the processor's internal state.
     This must copy any info about the processor's state into the block of memory provided, so that the host can store this and later restore it using setStateInformation()."
     */
    void getStateInformation (juce::MemoryBlock& destData) override;
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "The host will call this method when it wants to save the processor's internal state.
     This must copy any info about the processor's state into the block of memory provided, so that the host can store this and later restore it using setStateInformation()."
     */
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //========================================================================
    //= END PURE VIRTUAL MEMBER FUNCTIONS from juce::AudioProcessor class
    //========================================================================
    
    /**
     From https://docs.juce.com/master/classAudioProcessor.html
     "Renders the next block.
     When this method is called, the buffer contains a number of channels which is at least as great as the maximum number of input and output channels that this processor is using. It will be filled with the processor's input data and should be replaced with the processor's output."
     */
    void processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages) override;
    /** update the state of the filter as the user changes parameters */
    void updateFilter(double freq, double res);
    
    // END Final GUI Component Code
    
private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
    // Final GUI Component Code
    
    //========================================================================
    //= PURE VIRTUAL MEMBER FUNCTIONS from juce::AudioProcessor class
    //========================================================================
    
    void reset() override;
    
    //========================================================================
    //= END PURE VIRTUAL MEMBER FUNCTIONS from juce::AudioProcessor class
    //========================================================================

    juce::dsp::StateVariableTPTFilter<float> filter;
    
    // END Final GUI Component Code
};
