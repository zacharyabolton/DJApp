/*
  ==============================================================================

    RotaryDialLookAndFeel.h
    Created: 4 Feb 2021 8:05:57pm
    Author:  Zac Bolton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RotaryDialLookAndFeel : public juce::LookAndFeel_V4
{
public:
    /** inputs: label for the rotary dial (string)
     constructor */
    RotaryDialLookAndFeel(juce::String label);
    /**
     destructor */
    ~RotaryDialLookAndFeel();
    /** inputs: reference to graphics to draw on (juce::Graphics); x position of graphics (int); y position of graphics (int); width of graphics (int); height of graphics (int); position of slider (float); starting angle of slider (float); ending angle of slider (float); reference to slider (juce::Slider&)
     draws rotary dial to the screen */
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float startAngle, float endAngle, juce::Slider& slider) override;
private:
    juce::String label;
};
