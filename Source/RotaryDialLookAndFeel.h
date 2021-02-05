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
    RotaryDialLookAndFeel(juce::String label);
    ~RotaryDialLookAndFeel();
    
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float startAngle, float endAngle, juce::Slider& slider) override;
private:
    juce::String label;
};