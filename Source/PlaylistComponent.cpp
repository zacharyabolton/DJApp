/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 11 Jan 2021 8:59:07pm
    Author:  Zac Bolton

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1,
                                     DJAudioPlayer* _player2,
                                     WaveformDisplay* _waveFormDisplay1,
                                     WaveformDisplay* _waveFormDisplay2
                                     )
    : player1(_player1),
    player2(_player2),
    waveformDisplay1(_waveFormDisplay1),
    waveformDisplay2(_waveFormDisplay2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
    trackTitles.push_back("Track 5");
    trackTitles.push_back("Track 6");
    
    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("", 2, 200);
    
    tableComponent.setModel(this);
    
    addAndMakeVisible(tableComponent);
    
    // Final Music Library Code
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    loadButton.setComponentID("0");
    // END Final Music Library Code
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
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
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    // Final Music Library Code
    double rowH = getHeight() / 10;
    loadButton.setBounds(0, rowH * 0, getWidth(), rowH * 1);
    // END Final Music Library Code
    
    tableComponent.setBounds(0, rowH * 1, getWidth(), rowH * 11);
}

int PlaylistComponent::getNumRows()
{
    return static_cast<int>(trackTitles.size());
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
                        int rowNumber,
                        int width,
                        int height,
                        bool rowIsSelected
                        )
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
               int rowNumber,
               int columnId,
               int width,
               int height,
               bool rowIsSelected
               )
{
    g.drawText(trackTitles[rowNumber],
               2,
               0,
               width - 4,
               height,
               juce::Justification::centredLeft,
               true
               );
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                         int columnId,
                                         bool isRowSelected,
                                         Component *existingComponentToUpdate
                                         )
{
    if (columnId == 2)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{"play"};
            juce::String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    int id = std::stoi(button->getComponentID().toStdString());

    if (button == &loadButton)
    {
        juce::FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            auto result = chooser.getResult();
            DBG(result.getFileNameWithoutExtension());
            auto url = juce::URL{result};
            
            player1->loadURL(url);
            player2->loadURL(url);
            waveformDisplay1->loadURL(url);
            waveformDisplay2->loadURL(url);
            
            
        }
    }
}

// Final Music Library Code

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray &files)
{
    DBG("PlaylistComponent::isInterestedInFileDrag");
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    DBG("PlaylistComponent::filesDropped");
    for (juce::String filename : files)
    {
        juce::URL fileURL = juce::URL{juce::File{filename}};
        player1->loadURL(fileURL);
        player2->loadURL(fileURL);
        waveformDisplay1->loadURL(fileURL);
        waveformDisplay2->loadURL(fileURL);
        return;
    }
}

// END Final Music Library Code
