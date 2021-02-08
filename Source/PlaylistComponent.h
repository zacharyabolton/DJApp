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

//==============================================================================
/*
*/
class PlaylistComponent  :  public juce::Component,
                            public juce::TableListBoxModel,
                            public juce::Button::Listener,
                            public juce::FileDragAndDropTarget
{
public:
    PlaylistComponent(DJAudioPlayer* player1,
                      DJAudioPlayer* player2,
                      WaveformDisplay* waveFormDisplay1,
                      WaveformDisplay* waveFormDisplay2,
                      juce::AudioFormatManager& formatManager
                      );
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getNumRows() override;
    
    void paintRowBackground(juce::Graphics&,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected
                            ) override;
    
    void paintCell(juce::Graphics&,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected
                   ) override;
    
    juce::Component* refreshComponentForCell(int rowNumber,
                                             int columnId,
                                             bool isRowSelected,
                                             Component *existingComponentToUpdate
                                             ) override;
    
    void buttonClicked(juce::Button* button) override;
    
    // Final Music Library Code
    /** Impliment FileDragAndDropTarget */
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    
    juce::String getLengthInMinutesAndSeconds(juce::URL audioURL);
    
    // END Final Music Library Code

private:
    
    juce::TableListBox tableComponent;
//    std::vector<Track*> tracks;
    std::vector<std::unique_ptr<Track>> tracks;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
    
    // Final Music Library Code
    DJAudioPlayer* player1;
    DJAudioPlayer* player2;
    WaveformDisplay* waveformDisplay1;
    WaveformDisplay* waveformDisplay2;
    
    juce::TextButton loadButton{"LOAD"};
    
    bool fileLoaded;
    
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    // END Final Music Library Code
};
