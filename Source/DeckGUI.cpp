/*
  ==============================================================================

    DeckGUI.cpp
    Created: 5 Jan 2021 5:45:42pm
    Author:  Zac Bolton

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse)
    : waveformDisplay(formatManagerToUse, cacheToUse),
    player(_player)
{
    // reveal different sub components
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(freqDial);
    addAndMakeVisible(resDial);
    
    // My default styles
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::blue);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::orange);
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colours::maroon);
    
    // make play button green to indicate purpose
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    
    // set control readouts to be above controls
    volSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 100, 20);
    speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 100, 20);
    posSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 100, 20);
    
    // make high pass filter dials rotary
    freqDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    resDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    
    // set ranges and values for sliders and dials
    freqDial.setRange(20.0f, 20000.0f);
    freqDial.setTextValueSuffix("Hz");
    freqDial.setNumDecimalPlacesToDisplay(2);
    freqDial.setValue(20.0f);
    resDial.setRange(0.3f, 20.0f);
    resDial.setValue(0.71f);
    resDial.setNumDecimalPlacesToDisplay(2);
    freqDial.setSkewFactorFromMidPoint(1000.0f);
    volSlider.setRange(0.0, 100.0);
    volSlider.setValue(50.0);
    volSlider.setNumDecimalPlacesToDisplay(0);
    volSlider.setTextValueSuffix("% Volume");
    speedSlider.setRange(0.1, 2.0);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setValue(1.0);
    speedSlider.setTextValueSuffix("x Speed");
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(2);
    posSlider.setTextValueSuffix(" Position");
    
    
    // add listeners to all interactive components
    freqDial.addListener(this);
    resDial.addListener(this);
    playButton.addListener(this);
    stopButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    // set callback timer to broadcast 10 times every second
    // to keep animation smooth
    startTimer(100);
    
    // update high pass filter with initial values
    player->updateFilter(freqDial.getValue(), resDial.getValue());
}

DeckGUI::~DeckGUI()
{
    // stop timer on app quit
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    // set GUI trim color to be orange
    // and set font size to 14 points
    g.setColour (juce::Colours::orange);
    g.setFont (14.0f);
    // write out a label for high pass filter
    g.drawText("High-pass filter", getWidth() / 2, 0, getWidth() / 2, 40, juce::Justification::centred);
    // apply custom styles to high pass filter controls
    freqDial.setLookAndFeel(&freqDialLookAndFeel);
    resDial.setLookAndFeel(&resDialLookAndFeel);
}

void DeckGUI::resized()
{
    // keep high-pass filter controls within reasonable bounds on resize
    freqDial.setBounds(getWidth() / 2,
                       40,
                       getWidth() / 2,
                       (getHeight() - 40) / 2);
    resDial.setBounds(getWidth() / 2,
                      (getHeight() / 2) + 20,
                      getWidth() / 2,
                      (getHeight() - 40) / 2);
    // keep high-pass filter readouts within reasonable bounds on resize
    freqDial.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getWidth() / 4, 20);
    resDial.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getWidth() / 4, 20);
    // setup a constant row height by which to control the layout of other components
    double rowH = getHeight() / 8;
    double spacer = rowH / 3;
    // keep sizing of other components within reasonable bounds on resize
    playButton.setBounds(0, 0, getWidth() / 2, rowH);
    stopButton.setBounds(0, rowH, getWidth() / 2, rowH);
    volSlider.setBounds(0, rowH * 2 + spacer * 1, getWidth() / 2, rowH);
    speedSlider.setBounds(0, rowH * 3 + spacer * 2, getWidth() / 2, rowH);
    posSlider.setBounds(0, rowH * 4 + spacer * 3, getWidth() / 2, rowH);
    waveformDisplay.setBounds(0, rowH * 6, getWidth() / 2, rowH * 2);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        // if play button is clicked, start playback of loaded track
        player->start();
    }
    if (button == &stopButton)
    {
        // if stop button is clicked, cease playback of loaded track
        player->stop();
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        // if volume slider is changed, adjust gain accordingly
        player->setGain(slider->getValue() / 100);
    }
    if (slider == &speedSlider)
    {
        // if speed slider is changed, adjust playback speed accordingly
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        // if position slider is changed, adjust play head position occordingly
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &freqDial || slider == &resDial)
    {
        // if high-pass controls change, update high pass filter accordingly
        player->updateFilter(freqDial.getValue(), resDial.getValue());
    }
}

void DeckGUI::timerCallback()
{
    // update the wave form display play head visual to keep position
    // relative to current moment in playback
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}
