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

    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass); // unsure

    reset();

    // END Final GUI Component Code
}

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
    
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    filter.setCutoffFrequency(150.0f);
    
    juce::dsp::AudioBlock<float> audioBlock(bufferToFill.buffer->getArrayOfWritePointers(),
                                       bufferToFill.buffer->getNumChannels(),
                                       bufferToFill.startSample,
                                       bufferToFill.numSamples);
    auto context = juce::dsp::ProcessContextReplacing<float> (audioBlock);
    
    filter.process(context);
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
//        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0.0 || ratio > 100.0)
    {
//        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 100");
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
//        DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");
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

//========================================================================
//= PURE VIRTUAL MEMBER FUNCTIONS from juce::AudioProcessor class
//========================================================================

void DJAudioPlayer::prepareToPlay (double sampleRate, int samplesPerBlockExpected)
{
//    // Final GUI Component Code
//    DBG("DJAudioPlayer::prepareToPlay");
//    juce::dsp::ProcessSpec spec;
//    spec.maximumBlockSize = samplesPerBlockExpected;
//    spec.sampleRate = sampleRate;
////    DBG("DJAudioPlayer::prepareToPlay getTotalNumOutputChannels() = " << getTotalNumOutputChannels());
////    spec.numChannels = getTotalNumOutputChannels();
//    spec.numChannels = 2;
//
//    filter.prepare(spec);
//
//    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass); // unsure
//
//    reset();
//
//    // END Final GUI Component Code
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    DBG("DJAudioPlayer::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)");
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    filter.setCutoffFrequency(150.0f);

    auto audioBlock = juce::dsp::AudioBlock<float> (buffer);
    auto context = juce::dsp::ProcessContextReplacing<float> (audioBlock);

    filter.process(context);
}

juce::AudioProcessorEditor* DJAudioPlayer::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

bool DJAudioPlayer::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

const juce::String DJAudioPlayer::getName() const
{
    return juce::String{"High pass"};
}

bool DJAudioPlayer::acceptsMidi() const
{
    return false;
}

bool DJAudioPlayer::producesMidi() const
{
    return false;
}

double DJAudioPlayer::getTailLengthSeconds() const
{
    return 0.0;
}

int DJAudioPlayer::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DJAudioPlayer::getCurrentProgram()
{
    return 0;
}

void DJAudioPlayer::setCurrentProgram (int index)
{
}

const juce::String DJAudioPlayer::getProgramName (int index)
{
    return {};
}

void DJAudioPlayer::changeProgramName (int index, const juce::String& newName)
{
}

void DJAudioPlayer::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DJAudioPlayer::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void DJAudioPlayer::reset()
{
    filter.reset();
}

//========================================================================
//= END PURE VIRTUAL MEMBER FUNCTIONS from juce::AudioProcessor class
//========================================================================

void DJAudioPlayer::processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    DBG("DJAudioPlayer::processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)");
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    filter.setCutoffFrequency(150.0f);

    auto audioBlock = juce::dsp::AudioBlock<double> (buffer);
    auto context = juce::dsp::ProcessContextReplacing<double> (audioBlock);

    filter.process(context);
}

void DJAudioPlayer::updateFilter(double freq, double res)
{
    DBG("DJAudioPlayer::updateFilter");
}

// END Final GUI Component Code
