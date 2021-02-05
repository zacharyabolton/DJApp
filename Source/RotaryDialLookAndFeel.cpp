/*
  ==============================================================================

    RotaryDialLookAndFeel.cpp
    Created: 4 Feb 2021 8:05:57pm
    Author:  Zac Bolton

  ==============================================================================
*/

#include "RotaryDialLookAndFeel.h"

RotaryDialLookAndFeel::RotaryDialLookAndFeel(juce::String _label) : label(_label)
{
}

RotaryDialLookAndFeel::~RotaryDialLookAndFeel()
{
}

void RotaryDialLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float startAngle, float endAngle, juce::Slider& slider)
{
    float diameter = juce::jmin(width, height) * 0.8;
    float radius = diameter / 2;
    float centerX = (x + (width / 2));
    float centerY = (y + (height / 2));
    
    float angle = startAngle + (sliderPos * (endAngle - startAngle));
    
    g.setFillType(juce::FillType(juce::ColourGradient(juce::Colours::blue, centerX, centerY, juce::Colours::black, centerX + radius, centerY + radius, true)));
    g.fillEllipse(centerX - radius, centerY - radius, diameter, diameter);
    
    g.setColour(juce::Colours::orange);
    g.drawText(label, 0, height * 0.55, width, 20, juce::Justification::centred);
    
    juce::Path indicator;
    float indicatorWidth = diameter * 0.03;
    float indicatorHeight = radius * 0.5;
    indicator.addRectangle(-5.0f, radius * 0.5, indicatorWidth, indicatorHeight);
    g.fillPath(indicator, juce::AffineTransform::rotation(angle + juce::float_Pi).translated(centerX, centerY));
}
