/*
  ==============================================================================

    PlaylistComponent.h
    Created: 11 Jan 2021 8:59:07pm
    Author:  Zac Bolton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "Track.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
// for convenience
using json = nlohmann::json;

//==============================================================================
/*
*/
class PlaylistComponent  :  public juce::Component,
                            public juce::TableListBoxModel,
                            public juce::Button::Listener,
                            public juce::FileDragAndDropTarget,
                            public juce::TextEditor::Listener
{
public:
    /** inputs: pointer to left hand audio player (DJAudioPlayer*); pointer to right hand audio player (DJAudioPlayer*); pointer to left hand wave form display (WaveFormDisplay*); pointer to right hand wave form display (WaveFormDisplay*); reference to audio format manager (juce::AudioFormatManager&)
     constructor */
    PlaylistComponent(DJAudioPlayer* player1,
                      DJAudioPlayer* player2,
                      WaveformDisplay* waveFormDisplay1,
                      WaveformDisplay* waveFormDisplay2,
                      juce::AudioFormatManager& formatManager
                      );
    /**
     destructor */
    ~PlaylistComponent() override;
    /** inputs: reference to graphics to draw on (juce::Graphics&)
     from https://docs.juce.com/master/classComponent.html#a7cf1862f4af5909ea72827898114a182
     "The paint() method gets called when a region of a component needs redrawing, either because the component's repaint() method has been called, or because something has happened on the screen that means a section of a window needs to be redrawn." */
    void paint (juce::Graphics&) override;
    /**
     from https://docs.juce.com/master/classComponent.html#ad896183a68d71daf5816982d1fefd960
     "Called when this component's size has been changed." */
    void resized() override;
    /** outputs: number of rows in playlist component (int)
     from https://docs.juce.com/master/classTableListBoxModel.html#ae7ff7c6523e8113ff1c13b39f7144bf2
     "This must return the number of rows currently in the table." */
    int getNumRows() override;
    /** inputs: reference to graphics to draw to (juce::Graphics&); target row number (int); width of row background to be drawn (int); height of row background to be drawn (int); flag indicating whether or not the target row is selected (bool)
     from https://docs.juce.com/master/classTableListBoxModel.html#a427fdaf7959f3858a7144b490227374a
     "This must draw the background behind one of the rows in the table." */
    void paintRowBackground(juce::Graphics&,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected
                            ) override;
    /** inputs: reference to graphics to draw to (juce::Graphics&); row number of target cell (int); column id of target cell (int); width of target cell to paint (int); height of target cell to paint (int); flag indicating if row of target cell is selected (bool)
     from https://docs.juce.com/master/classTableListBoxModel.html#a46310df3a1b63b4e874f40bb3dfb14f5
     "This must draw one of the cells." */
    void paintCell(juce::Graphics&,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected
                   ) override;
    /** inputs: row number of cell containing custom component (int); column id of cell containing custom component (int); flag indicating whether of not cell containing custom component is selected (bool); pointer to custom component to be created/updated (juce::Component*) | outputs: pointer to created/updated custom component (juce::Component*)
     from https://docs.juce.com/master/classTableListBoxModel.html#a07767e4e5a3812e486c187705b0921bd
     "This is used to create or update a custom component to go in a cell." */
    juce::Component* refreshComponentForCell(int rowNumber,
                                             int columnId,
                                             bool isRowSelected,
                                             Component *existingComponentToUpdate
                                             ) override;
    /** inputs: pointer to button recieving click (juce::Button*)
     from https://docs.juce.com/master/classButton_1_1Listener.html#a81499cef24b7189cd0d1581fd9dc9e14
     "Called when the button is clicked." */
    void buttonClicked(juce::Button* button) override;
    /** inputs: reference to array of strings representing dragged files (juce::StringArray&) | outputs: flag indicating whether or not app will accept files (bool)
     from https://docs.juce.com/master/classFileDragAndDropTarget.html#a44fa7f7e69dec8119c6b3a6ff9d2ddce
     "Callback to check whether this target is interested in the set of files being offered." */
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    /** inputs: reference to array of strings representing dragged files (juce::StringArray&); x location of drop (int); y location of drop (int)
     from https://docs.juce.com/master/classFileDragAndDropTarget.html#adc7848885ab2d9380f242c6445b019d4
     "Callback to indicate that the user has dropped the files onto this component." */
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    /** inputs: audio file URL (juce::URL) | outputs: lengths of audio file in minutes and seconds (string)
     returns length of track in minutes and seconds as a string when passed a juce::URL to a valid track */
    juce::String getLengthInMinutesAndSeconds(juce::URL audioURL);
    /** inputs: file to be added to playlist (juce::File); optional arguement - lenght of file in minutes and seconds - if already known [coming from save state] (string)
     add a track to the playlist when given a file */
    void addTrack(juce::File result, juce::String length = "");
    /** inputs: number of track to be removed (int)
     remove a track from the playlist when given the track's number in the playlist */
    void removeTrack(int trackNum);
    /** inputs: reference to text editor component registering change (juce::TextEditor&)
     from https://docs.juce.com/master/classTextEditor_1_1Listener.html#a17ec33c8bc4e83799f0edbfc559c761c
     "Called when the user changes the text in some way." */
    void textEditorTextChanged(juce::TextEditor & textEditor) override;
    /**
     load tracks that were in playlist on last quit, if any */
    void loadFromFile();
    /**
     save tracks currently in playlist for next startup , if any */
    void saveToFile();
    
private:
    
    juce::TableListBox tableComponent;
    std::vector<std::unique_ptr<Track>> tracks;
    std::vector<std::unique_ptr<Track>> searchResults;
    
    DJAudioPlayer* player1;
    DJAudioPlayer* player2;
    WaveformDisplay* waveformDisplay1;
    WaveformDisplay* waveformDisplay2;
    
    juce::TextButton loadButton{"LOAD"};
    juce::TextEditor searchField{"Search"};
    
    bool fileLoaded;
    
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    
    juce::File loadFile;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
