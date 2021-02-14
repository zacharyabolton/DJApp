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
#include <fstream>
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
    
    // setup playlist column headers
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
    // setup architecture and basic styles for playlist
    tableComponent.setModel(this);
    tableComponent.setColour(juce::TableListBox::backgroundColourId, juce::Colours::black);
    tableComponent.getHeader().setColour(juce::TableHeaderComponent::backgroundColourId, juce::Colours::black);
    tableComponent.getHeader().setColour(juce::TableHeaderComponent::textColourId, juce::Colours::whitesmoke);
    
    addAndMakeVisible(tableComponent);
    
    // setup architecture and basic styles for playlist meta controls (load and search)
    addAndMakeVisible(loadButton);
    addAndMakeVisible(searchField);
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkorange);
    loadButton.addListener(this);
    searchField.addListener(this);
    searchField.setTextToShowWhenEmpty("Search...", juce::Colours::white);
    
    // setup file for saving playlist state to on quit
    loadFile = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userHomeDirectory).getChildFile("playlist.json");
    // load in playlist from last quit, if applicable
    loadFromFile();
}

PlaylistComponent::~PlaylistComponent()
{
    // save playlist to file in home dir before finishing so that app
    // can reload tracks on next startup.
    saveToFile();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    // set playlist styles
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    
    if (tracks.size() > 0)
    {
        // if playlist is not empty but no tracks are being displayed
        // means an unsatisfied search query. Inform the user.
        g.drawText ("Nothing found.", getLocalBounds(),
                    juce::Justification::centred, true);
    }
    else {
        // if playlist is empty, tell the user that they can fill it
        // by dropping files onto playlist target
        g.drawText ("Drop files here...", getLocalBounds(),
                    juce::Justification::centred, true);
    }
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    // set track listing (row) height to be 30 pixels
    double rowH = 30;
    // load button is same height as track listing and half component width
    loadButton.setBounds(0, rowH * 0, getWidth() / 2, rowH * 1);
    // search field is same height as track listing and half component width
    searchField.setBounds(getWidth() / 2, rowH * 0, getWidth() / 2, rowH * 1);
    
    if (searchResults.size() > 0)
    {
        // if playlist contains tracks, draw to fill component's bounds so as to
        // hide UI instructions underneath
        tableComponent.setBounds(0, rowH * 1, getWidth(), getHeight() - rowH);
    }
    else {
        // if playlist does not contain tracks, collapse, so as to show
        // UI instructions for filling playlist, unerneath
        tableComponent.setBounds(0, rowH * 1, getWidth(), 0);
    }
}

int PlaylistComponent::getNumRows()
{
    // return number of rows in _displayed_ playlist, i.e. number of
    // tracks returned by current search query
    return static_cast<int>(searchResults.size());
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
        // fill selected row orange
        g.fillAll(juce::Colours::orange);
    }
    else {
        // fill all other rows dark grey
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
        // draw track title
        g.drawText(searchResults[rowNumber]->getName(),
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
        // draw track length
        g.drawText(searchResults[rowNumber]->getLength(),
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
            // setup 'load to left-hand deck' button,
            // attach listener and ID, and return to be refreshed
            juce::TextButton* btn = new juce::TextButton{"load left"};
            juce::String id{std::to_string(rowNumber * 3 + 0)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        if (columnId == 4)
        {
            // setup 'load to right-hand deck' button,
            // attach listener and ID, and return to be refreshed
            juce::TextButton* btn = new juce::TextButton{"load right"};
            juce::String id{std::to_string(rowNumber * 3 + 1)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        if (columnId == 5)
        {
            // setup 'delete track' button,
            // attach listener and ID, and return to be refreshed
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
    if (button == &loadButton)
    {
        // if load button has been clicked open file picker
        juce::FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            // if user has chosen a file to load, add to playlist
            auto result = chooser.getResult();
            addTrack(result);
        }
        // load button is only child button without id, so
        // return to stop rest of execution and avoid error
        return;
    }
    // if not load button, get button ID
    int id = std::stoi(button->getComponentID().toStdString());
    if (id % 3 == 0)
    {
        // if button is 'load to left-hand deck' button, do so
        juce::URL url = searchResults[static_cast<int>(id / 3)]->getURL();
        juce::String title = searchResults[static_cast<int>(id / 3)]->getName();
        player1->loadURL(url);
        waveformDisplay1->loadURL(url);
        waveformDisplay1->setCurrentTrackTitle(title);
    }
    if (id % 3 == 1)
    {
        // if button is 'load to right-hand deck' button, do so
        juce::URL url = searchResults[static_cast<int>(id / 3)]->getURL();
        juce::String title = searchResults[static_cast<int>(id / 3)]->getName();
        player2->loadURL(url);
        waveformDisplay2->loadURL(url);
        waveformDisplay2->setCurrentTrackTitle(title);
    }
    if (id % 3 == 2)
    {
        // if button is 'delete track' button, do so
        removeTrack(static_cast<int>(id / 3));
    }
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray &files)
{
    // accepts all files
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (juce::String filename : files)
    {
        // loop through tracks and add each to playlist
        auto result = juce::File{filename};
        addTrack(result);
    }
}

juce::String PlaylistComponent::getLengthInMinutesAndSeconds(juce::URL audioURL)
{
    // parse file with AudioFormatManager to get length in seconds (double) and convert
    // into string of form "MM:SS" (minutes and seconds)
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

void PlaylistComponent::addTrack(juce::File result, juce::String length)
{
    // add a track to the playlist
    
    // get URL from File
    auto url = juce::URL{result};

    if (length == "")
    {
        // if length is not passed in it means it is a fresh file
        // and should setup for the first time
        std::unique_ptr<Track> track(new Track(result.getFileNameWithoutExtension(), getLengthInMinutesAndSeconds(url), url));
        tracks.push_back(std::move(track));
        
        std::unique_ptr<Track> displayedTrack(new Track(result.getFileNameWithoutExtension(), getLengthInMinutesAndSeconds(url), url));
        searchResults.push_back(std::move(displayedTrack));
    }
    else {
        // if length is passed in it means it is a track coming from
        // loadFile and does not need to be setup from scratch
        std::unique_ptr<Track> track(new Track(result.getFileNameWithoutExtension(), length, url));
        tracks.push_back(std::move(track));
        
        std::unique_ptr<Track> displayedTrack(new Track(result.getFileNameWithoutExtension(), length, url));
        searchResults.push_back(std::move(displayedTrack));
    }
    
    // resize everything to display new track
    resized();
    tableComponent.resized();
}

void PlaylistComponent::removeTrack(int trackNum)
{
    // remove track from both search results vector and tracks vector
    juce::URL trackToBeRemoved = searchResults[trackNum]->getURL();
    for (int t = 0; t < tracks.size(); ++t)
    {
        if (tracks[t]->getURL() == trackToBeRemoved)
        {
            tracks.erase(tracks.begin() + t);
        }
    }
    searchResults.erase(searchResults.begin() + trackNum);
    
    // resize everything to display new track list
    resized();
    tableComponent.resized();
    
    // refresh search results to reflect new tracks vector state
    textEditorTextChanged(searchField);
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor & textEditor)
{
    // searching procedure
    for (int t = 0; t < tracks.size(); ++t)
    {
        // look through all tracks for substring
        // if substring found
        if (tracks[t]->getName().containsIgnoreCase(textEditor.getText()))
        {
            // if not already in search results
            if (!tracks[t]->isResultOfSearch())
            {
                searchResults.clear();
                tracks[t]->setAsResultOfSearch(true);
                for (int r = 0; r < tracks.size(); ++r)
                {
                    if (tracks[r]->isResultOfSearch())
                    {
                        std::unique_ptr<Track> track(new Track(tracks[r]->getName(), tracks[r]->getLength(), tracks[r]->getURL()));
                        searchResults.push_back(std::move(track));
                    }
                }
            }
            else {
                // if already in search results
                tracks[t]->setAsResultOfSearch(true);
            }
        }
        else {
            // if substring not found
            // if not already in search results
            if (!tracks[t]->isResultOfSearch())
            {
                tracks[t]->setAsResultOfSearch(false);
            }
            else {
                // if already in search results
                for (int r = 0; r < searchResults.size(); ++r)
                {
                    if (tracks[t]->getURL() == searchResults[r]->getURL())
                    {
                        searchResults.erase(searchResults.begin() + r);
                    }
                }
                tracks[t]->setAsResultOfSearch(false);
            }
        }
        // resize and repaint everything to reflect results of search
        resized();
        tableComponent.resized();
        repaint();
        tableComponent.repaint();
    }
}

void PlaylistComponent::loadFromFile()
{
    // code adapted from https://forum.juce.com/t/example-for-creating-a-file-and-doing-something-with-it/31998/2
    if (! loadFile.existsAsFile())
    {
        // File doesn't exist
        // return and proceed with execution
        return;
    }
    else {
        std::unique_ptr<juce::FileInputStream> input (loadFile.createInputStream());

        if (! input->openedOk())
        {
            // Failed to open file
            // return and proceed with execution
            return;
        }

        juce::String content = input->readString();
        auto j = json::parse(content.toStdString());

        // convert json objects for tracks into class Track objects, usable by playlist component
        // and add to playlist
        for (auto& element : j) {
            auto urlStr = element["url"].get<std::string>();
            juce::String url = urlStr;
            juce::File loadTrack{url};
            auto lengthStr = element["length"].get<std::string>();
            juce::String length = lengthStr;
            addTrack(loadTrack, length);
        }
    }
    // END adapted code
}

void PlaylistComponent::saveToFile()
{
    // create empty json object to store json date to be saved to file
    json j{};
    
    for (int t = 0; t < tracks.size(); ++t)
    {
        // convert class Track objects objects for tracks json objects, for
        // storing to state save file
        j[t]["name"] = tracks[t]->getName().toStdString();
        j[t]["length"] = tracks[t]->getLength().toStdString();
        std::string url = tracks[t]->getURL().toString(false).toStdString();
        // code adapted from https://stackoverflow.com/a/20412841
        // convert url style paths to system style paths
        const std::string urlSpace = "%20";
        const std::string pathSpace = "\ ";
        std::string::size_type n = 0;
        while ( ( n = url.find( urlSpace, n ) ) != std::string::npos )
        {
            url.replace( n, urlSpace.size(), pathSpace );
            n += pathSpace.size();
        }
        // end adapted code
        // convert url style paths to system style paths
        std::string path = url.substr(7, url.length());
        j[t]["url"] = path;
    }

    // code adapted from https://forum.juce.com/t/example-for-creating-a-file-and-doing-something-with-it/31998/2
    if (! loadFile.existsAsFile())
    {
        // File doesn't exist
        // return and proceed with execution
        return;
    }
    
    juce::TemporaryFile tempFile (loadFile);

    juce::FileOutputStream output (tempFile.getFile());

    if (! output.openedOk())
    {
        // FileOutputStream didn't open correctly
        // return and proceed with execution
        return;
    }

    output.setNewLineString("\n");

    juce::String contents {};
    contents << j.dump(4);
    
    output.writeText(contents, false, false, "\n");

    output.flush(); // (called explicitly to force an fsync on posix)

    if (output.getStatus().failed())
    {
        // An error occurred in the FileOutputStream
        // return and proceed with execution
        return;
    }

    tempFile.overwriteTargetFileWithTemporary();
    // END adapted code
}
