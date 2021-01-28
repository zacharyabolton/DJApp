/*
  ==============================================================================

    PassFilter.cpp
    Created: 21 Jan 2021 6:19:59pm
    Author:  Zac Bolton

  ==============================================================================
*/

#include "PassFilter.h"
//#include "PassFilterEditor.h"

PassFilter::PassFilter() : lastSampleRate(42000.0f)
{
}

PassFilter::~PassFilter()
{
}

const juce::String PassFilter::getName() const
{
    return juce::String{"High pass"};
}

bool PassFilter::acceptsMidi() const
{
    return false;
}

bool PassFilter::producesMidi() const
{
    return false;
}

double PassFilter::getTailLengthSeconds() const
{
    return 0.0;
}

int PassFilter::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PassFilter::getCurrentProgram()
{
    return 0;
}

void PassFilter::setCurrentProgram (int index)
{
}

const juce::String PassFilter::getProgramName (int index)
{
    return {};
}

void PassFilter::changeProgramName (int index, const juce::String& newName)
{
}

void PassFilter::prepareToPlay (double sampleRate, int samplesPerBlockExpected)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.sampleRate = sampleRate;
//    DBG("PassFilter::prepareToPlay getTotalNumOutputChannels() = " << getTotalNumOutputChannels());
//    spec.numChannels = getTotalNumOutputChannels();
    spec.numChannels = 2;
    
    filter.prepare(spec);
    
    filter.setType(juce::dsp::StateVariableTPTFilterType::highpass); // unsure
    
    reset();
}

void PassFilter::releaseResources()
{
}

void PassFilter::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
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
    
    // CULL
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
//    }
    // END CULL
    
    filter.setCutoffFrequency(150.0f);
    
    auto audioBlock = juce::dsp::AudioBlock<float> (buffer);
    auto context = juce::dsp::ProcessContextReplacing<float> (audioBlock);
    
    filter.process(context);
}

bool PassFilter::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PassFilter::createEditor()
{
//    return new PassFilterEditor(*this, 400, 400); // CHANGE
    return new juce::GenericAudioProcessorEditor(*this);
}

void PassFilter::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PassFilter::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// CULL
void PassFilter::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
//    juce::dsp::AudioBlock<juce::AudioSourceChannelInfo> block (bufferToFill);
//    stateVariableFilter.process(juce::dsp::ProcessContextReplacing<double> (block));
}
// END CULL

void PassFilter::updateFilter(double freq, double res)
{
    DBG("PassFilter::updateFilter freq = " << freq << "; res = " << res);
    stateVariableFilter.state->setCutOffFrequency(lastSampleRate, freq, res);
}

void PassFilter::reset()
{
    filter.reset();
}
