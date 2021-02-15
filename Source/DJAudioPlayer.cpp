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
    // prepare transport source and resample source for playback
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // create holder for high-pass filter specifications
    juce::dsp::ProcessSpec spec;
    // setup high-pass filter specifications
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;
    // assign high-pass filter specifications
    filter.prepare(spec);
    // set type of filter to 'high-pass'
    filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    // reset filter to remove any junk in preperation for playback
    reset();
}

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // pass incoming audio buffer to resample source
    resampleSource.getNextAudioBlock(bufferToFill);
    // Adapted from code provided by Xenakios on 'The Audio Programmer' Discord channel on 2021-02-02 23:59 GMT
    // https://discord.com/channels/382895736356077570/382895736863457281/806305801768665109
    // convert juce AudioSourceChannelInfo buffer to juce AudioBlock buffer for funneling to high-pass filter
    juce::dsp::AudioBlock<float> audioBlock(bufferToFill.buffer->getArrayOfWritePointers(),
                                       bufferToFill.buffer->getNumChannels(),
                                       bufferToFill.startSample,
                                       bufferToFill.numSamples);
    // End adapted code
    // replace the audio buffer with the filtered version
    auto context = juce::dsp::ProcessContextReplacing<float> (audioBlock);
    // process audio buffer with high-pass filter
    filter.process(context);
}

void DJAudioPlayer::releaseResources()
{
    // release resources from sources
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    // load file into sources for playback
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
    // setter for playback volume
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
    // setter for playback speed
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
    // setter for play head position
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    // helper method to translate between a relative position and absolute
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
    // start audio playback
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    // stop audio playback
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative() const
{
    // return the relative position of the current moment in playback
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

void DJAudioPlayer::updateFilter(float freq, float res)
{
    // update high-pass filter on user input
    filter.setCutoffFrequency(freq);
    filter.setResonance(res);
}

void DJAudioPlayer::reset()
{
    // clear junk data out of filter
    filter.reset();
}
