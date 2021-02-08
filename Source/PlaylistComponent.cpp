/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 11 Jan 2021 8:59:07pm
    Author:  Zac Bolton

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "Track.h"
#include <string>
#include <iomanip>
#include <iostream>

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1,
                                     DJAudioPlayer* _player2,
                                     WaveformDisplay* _waveFormDisplay1,
                                     WaveformDisplay* _waveFormDisplay2,
                                     juce::AudioFormatManager& _formatManager
                                     )
    : player1(_player1),
    player2(_player2),
    waveformDisplay1(_waveFormDisplay1),
    waveformDisplay2(_waveFormDisplay2),
    formatManager(_formatManager)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    tableComponent.getHeader().addColumn("Track title",
                                         1, 490);
    tableComponent.getHeader().addColumn("Length",
                                         2, 100);
    tableComponent.getHeader().addColumn("",
                                         3, 90);
    tableComponent.getHeader().addColumn("",
                                         4, 90);
    tableComponent.getHeader().addColumn("",
                                         5, 30);
    
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
    double rowH = 30;
    loadButton.setBounds(0, rowH * 0, getWidth(), rowH * 1);
    tableComponent.setBounds(0, rowH * 1, getWidth(), getHeight() - rowH);
    // END Final Music Library Code
}

int PlaylistComponent::getNumRows()
{
    return static_cast<int>(tracks.size());
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
    if (columnId == 1)
    {
        g.drawText(tracks[rowNumber]->getName(),
                   2,
                   0,
                   width - 4,
                   height,
                   juce::Justification::centredLeft,
                   true
                   );
    }
    if (columnId == 2)
    {
        g.drawText(tracks[rowNumber]->getLength(),
                   2,
                   0,
                   width - 4,
                   height,
                   juce::Justification::centredLeft,
                   false
                   );
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                         int columnId,
                                         bool isRowSelected,
                                         Component *existingComponentToUpdate
                                         )
{
    if (columnId >= 3 && columnId <= 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            //                juce::TextButton* btn = new juce::TextButton{"x"};
            //                juce::String id{std::to_string(rowNumber)};
            //                btn->setComponentID(id);
            //                btn->addListener(this);
            //                existingComponentToUpdate = btn;
            juce::TextButton* btn = tracks[rowNumber]->getButtons()[columnId - 3];
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // Final Music Library Code
    int id = std::stoi(button->getComponentID().toStdString());

    if (button == &loadButton)
    {
        juce::FileChooser chooser{"Select a file...", juce::File::getSpecialLocation(juce::File::userMusicDirectory), "*.wav, *.m4a, *.mp3"};
        if (chooser.browseForFileToOpen())
        {
            auto result = chooser.getResult();
            auto url = juce::URL{result};
            
//            Track* track = new Track{result.getFileNameWithoutExtension(), getLengthInMinutesAndSeconds(url), url, this};
            std::unique_ptr<Track> track(new Track(result.getFileNameWithoutExtension(), getLengthInMinutesAndSeconds(url), url, this));
            tracks.push_back(std::move(track));
            
            player1->loadURL(url);
            player2->loadURL(url);
            waveformDisplay1->loadURL(url);
            waveformDisplay2->loadURL(url);
            
            resized();
            tableComponent.resized();
        }
    }
    // END Final Music Library Code
}

// Final Music Library Code

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray &files)
{
    
    for (juce::String filename : files)
    {
        auto result = juce::File{filename};
        if (result.getFileExtension() != ".wav" &&
            result.getFileExtension() != ".m4a" &&
            result.getFileExtension() != ".mp3")
        {
            DBG("PlaylistComponent::isInterestedInFileDrag not interested");
            return false;
        }
    }
    DBG("PlaylistComponent::isInterestedInFileDrag interested");
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    DBG("PlaylistComponent::filesDropped");
    for (juce::String filename : files)
    {
        auto result = juce::File{filename};
        auto url = juce::URL{result};
        
//        Track* track = new Track{result.getFileNameWithoutExtension(), getLengthInMinutesAndSeconds(url), url, this};
        std::unique_ptr<Track> track(new Track(result.getFileNameWithoutExtension(), getLengthInMinutesAndSeconds(url), url, this));
        tracks.push_back(std::move(track));
        
        player1->loadURL(url);
        player2->loadURL(url);
        waveformDisplay1->loadURL(url);
        waveformDisplay2->loadURL(url);

        resized();
        tableComponent.resized();
    }
}

juce::String PlaylistComponent::getLengthInMinutesAndSeconds(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
    DBG("PlaylistComponent::getLengthInMinutesAndSeconds: " << transportSource.getLengthInSeconds());
    int mins = floor(transportSource.getLengthInSeconds() / 60);
    int secs = floor(static_cast<int>(transportSource.getLengthInSeconds()) % 60);
    
    juce::String length = std::to_string(mins) + ":";
    
    if (secs < 10)
    {
        length = length + "0" + std::to_string(secs);
    }
    else {
        length = length + std::to_string(secs);
    }
    return length;
}

// END Final Music Library Code
