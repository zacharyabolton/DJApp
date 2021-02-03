/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 4 Jan 2021 7:30:10pm
    Author:  Zac Bolton

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
    : formatManager(_formatManager)
{
}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // Final GUI Component Code
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.sampleRate = sampleRate;
//    DBG("DJAudioPlayer::prepareToPlay getTotalNumOutputChannels() = " << getTotalNumOutputChannels());
//    spec.numChannels = getTotalNumOutputChannels();
    spec.numChannels = 2;

    filter.prepare(spec);

    filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);

    reset();

    // END Final GUI Component Code
}

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
    
    // Final GUI Component Code
    
    // Adapted from code provided by Xenakios on 'The Audio Programmer' Discord channel on 2021-02-02 23:59 GMT
    juce::dsp::AudioBlock<float> audioBlock(bufferToFill.buffer->getArrayOfWritePointers(),
                                       bufferToFill.buffer->getNumChannels(),
                                       bufferToFill.startSample,
                                       bufferToFill.numSamples);
    // End adapted code
    
    auto context = juce::dsp::ProcessContextReplacing<float> (audioBlock);
    
    // END Final GUI Component Code
    
    filter.process(context);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0.0 || gain > 1.0)
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0.0 || ratio > 100.0)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 100");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0.0 || pos > 1.0)
    {
        DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative() const
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

// Final GUI Component Code

void DJAudioPlayer::updateFilter(float freq, float res)
{
    filter.setCutoffFrequency(freq);
    filter.setResonance(res);
}

void DJAudioPlayer::reset()
{
    filter.reset();
}

// END Final GUI Component Code
