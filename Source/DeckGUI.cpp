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
    : player(_player),
    waveformDisplay(formatManagerToUse, cacheToUse)
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    
    // final project gui component code
    addAndMakeVisible(freqDial);
    addAndMakeVisible(resDial);
    // END final project gui component code
    
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    // final project gui component code
    freqDial.addListener(this);
    resDial.addListener(this);
    // END final project gui component code
    
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.1, 100.0);
    posSlider.setRange(0.0, 1.0);
    
    // final project gui component code
    freqDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    resDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    // END final project gui component code

    startTimer(100);
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
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8;
    playButton.setBounds(0, 0, getWidth() / 2, rowH);
    stopButton.setBounds(0, rowH, getWidth() / 2, rowH);
    volSlider.setBounds(0, rowH * 2, getWidth() / 2, rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth() / 2, rowH);
    
    // final project gui component code
    freqDial.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getWidth() / 2 * 0.4, 20);
    resDial.setTextBoxStyle(juce::Slider::TextBoxAbove, true, getWidth() / 2 * 0.4, 20);
    
    freqDial.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);
    resDial.setBounds(getWidth() / 2, getHeight() / 2, getWidth() / 2, getHeight() / 2);
    // END final project gui component code
    
    posSlider.setBounds(0, rowH * 4, getWidth() / 2, rowH);
    waveformDisplay.setBounds(0, rowH * 5, getWidth() / 2, rowH * 2);
    loadButton.setBounds(0, rowH * 7, getWidth() / 2, rowH);

}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked");
        player->start();

    }
    if (button == &stopButton)
    {
        DBG("Stop button was clicked ");
        player->stop();
    }
    if (button == &loadButton)
    {
        DBG("Load button was clicked");
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
    // final project gui component code
    if (slider == &freqDial)
    {
        std::cout << "DeckGUI::sliderValueChanged freqDial changed to " << slider->getValue() << std::endl;
    }
    if (slider == &resDial)
    {
        std::cout << "DeckGUI::sliderValueChanged resDial changed to " << slider->getValue() << std::endl;
    }
    // END final project gui component code
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (juce::String filename : files)
    {
        DBG("DeckGUI::filesDropped " << filename );
        juce::URL fileURL = juce::URL{juce::File{filename}};
        player->loadURL(fileURL);
        waveformDisplay.loadURL(fileURL);
        return;
} }

void DeckGUI::timerCallback()
{
//    DBG("DeckGUI::timerCallback");
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}
