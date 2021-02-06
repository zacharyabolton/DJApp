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
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    
    // Final GUI Component Code
    freqDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    resDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    
    addAndMakeVisible(freqDial);
    addAndMakeVisible(resDial);

    freqDial.setRange(20.0f, 20000.0f);
    freqDial.setTextValueSuffix("Hz");
    freqDial.setNumDecimalPlacesToDisplay(2);
    freqDial.setValue(20.0f);
    resDial.setRange(0.3f, 20.0f);
    resDial.setValue(0.71f);
    resDial.setNumDecimalPlacesToDisplay(2);
    
    freqDial.setSkewFactorFromMidPoint(1000.0f);
    
    freqDial.addListener(this);
    resDial.addListener(this);
    // END Final GUI Component Code
    
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(1.0);
    volSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setRange(0.1, 2.0);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setValue(1.0);
    speedSlider.setTextValueSuffix("x");
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(2);

    startTimer(100);
    
    player->updateFilter(freqDial.getValue(), resDial.getValue());
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    
    // Final GUI Component Code
    freqDial.setLookAndFeel(&freqDialLookAndFeel);
    resDial.setLookAndFeel(&resDialLookAndFeel);
    // END Final GUI Component Code
}

void DeckGUI::resized()
{
    // Final GUI Component Code
    freqDial.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);
    resDial.setBounds(getWidth() / 2, getHeight() / 2, getWidth() / 2, getHeight() / 2);
    
    freqDial.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getWidth() / 4, 20);
    resDial.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getWidth() / 4, 20);
    // END Final GUI Component Code
    
    double rowH = getHeight() / 8;
    
    playButton.setBounds(0, 0, getWidth() / 2, rowH);
    stopButton.setBounds(0, rowH, getWidth() / 2, rowH);
    volSlider.setBounds(0, rowH * 2, getWidth() / 2, rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth() / 2, rowH);
    
    posSlider.setBounds(0, rowH * 4, getWidth() / 2, rowH);
    waveformDisplay.setBounds(0, rowH * 5, getWidth() / 2, rowH * 2);
    loadButton.setBounds(0, rowH * 7, getWidth() / 2, rowH);

}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        player->start();
    }
    if (button == &stopButton)
    {
        player->stop();
    }
    if (button == &loadButton)
    {
        juce::FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(juce::URL{chooser.getResult()});
            waveformDisplay.loadURL(juce::URL{chooser.getResult()});
        }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    // Final GUI Component Code
    if (slider == &freqDial || slider == &resDial)
    {
        player->updateFilter(freqDial.getValue(), resDial.getValue());
    }
    // Final GUI Component Code
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files)
{
    if (files.size() != 1)
    {
        return false;
    }
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::URL fileURL = juce::URL{juce::File{files[0]}};
    player->loadURL(fileURL);
    waveformDisplay.loadURL(fileURL);
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}
