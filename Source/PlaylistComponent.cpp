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
#include <stdlib.h>

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
    tableComponent.setColour(juce::TableListBox::backgroundColourId, juce::Colours::black);
    tableComponent.getHeader().setColour(juce::TableHeaderComponent::backgroundColourId, juce::Colours::black);
    tableComponent.getHeader().setColour(juce::TableHeaderComponent::textColourId, juce::Colours::whitesmoke);
    
    addAndMakeVisible(tableComponent);
    
    addAndMakeVisible(loadButton);
    addAndMakeVisible(searchField);
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkorange);
    loadButton.addListener(this);
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
    g.drawText ("Drop files here...", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    // Final Music Library Code
    double rowH = 30;
    loadButton.setBounds(0, rowH * 0, getWidth() / 2, rowH * 1);
    searchField.setBounds(getWidth() / 2, rowH * 0, getWidth() / 2, rowH * 1);
    
    if (tracks.size() > 0)
    {
        tableComponent.setBounds(0, rowH * 1, getWidth(), getHeight() - rowH);
    }
    else {
        tableComponent.setBounds(0, rowH * 1, getWidth(), 0);
    }
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

    if (existingComponentToUpdate == nullptr)
    {
        if (columnId == 3)
        {
            juce::TextButton* btn = new juce::TextButton{"load left"};
            juce::String id{std::to_string(rowNumber * 3 + 0)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        if (columnId == 4)
        {
            juce::TextButton* btn = new juce::TextButton{"load right"};
            juce::String id{std::to_string(rowNumber * 3 + 1)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        if (columnId == 5)
        {
            juce::TextButton* btn = new juce::TextButton{"x"};
            juce::String id{std::to_string(rowNumber * 3 + 2)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // Final Music Library Code
    int id = std::stoi(button->getComponentID().toStdString());
    DBG("PlaylistComponent::buttonClicked: button clicked: id = " << id);
    if (button == &loadButton)
    {
        juce::FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            auto result = chooser.getResult();
            addTrack(result);
        }
        return;
    }
    if (id % 3 == 0)
    {
        juce::URL url = tracks[static_cast<int>(id / 3)]->getURL();
        juce::String title = tracks[static_cast<int>(id / 3)]->getName();
        player1->loadURL(url);
        waveformDisplay1->loadURL(url);
        waveformDisplay1->setCurrentTrackTitle(title);
    }
    if (id % 3 == 1)
    {
        juce::URL url = tracks[static_cast<int>(id / 3)]->getURL();
        juce::String title = tracks[static_cast<int>(id / 3)]->getName();
        player2->loadURL(url);
        waveformDisplay2->loadURL(url);
        waveformDisplay2->setCurrentTrackTitle(title);
    }
    if (id % 3 == 2)
    {
        removeTrack(static_cast<int>(id / 3));
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
        addTrack(result);
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

void PlaylistComponent::addTrack(juce::File result)
{
    auto url = juce::URL{result};

    std::unique_ptr<Track> track(new Track(result.getFileNameWithoutExtension(), getLengthInMinutesAndSeconds(url), url));
    tracks.push_back(std::move(track));
    
    resized();
    tableComponent.resized();
}

void PlaylistComponent::removeTrack(int trackNum)
{
    tracks.erase(tracks.begin() + trackNum);
    resized();
    tableComponent.resized();
}

// END Final Music Library Code
