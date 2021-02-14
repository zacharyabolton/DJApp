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
    // set the rotary dial diameter based on the min distance from center to edge of local bounds
    float diameter = juce::jmin(width, height) * 0.8;
    // set radius based on diamter above
    float radius = diameter / 2;
    // find x and y coords for pixel in center of local bounds
    float centerX = (x + (width / 2));
    float centerY = (y + (height / 2));
    // set the angle that the radial dial needs to be at
    // based on start and end angles and current slider position
    float angle = startAngle + (sliderPos * (endAngle - startAngle));
    // fill rotary slider with a gradint - center blue to black edges
    g.setFillType(juce::FillType(juce::ColourGradient(juce::Colours::blue, centerX, centerY, juce::Colours::black, centerX + radius, centerY + radius, true)));
    g.fillEllipse(centerX - radius, centerY - radius, diameter, diameter);
    // set rotary slider position indicator and label text to orange
    g.setColour(juce::Colours::orange);
    // draw rotary slider label for UI
    g.drawText(label, 0, height * 0.55, width, 20, juce::Justification::centred);
    juce::Path indicator;
    float indicatorWidth = diameter * 0.03;
    float indicatorHeight = radius * 0.5;
    // draw rotary slider position indicator
    indicator.addRectangle(-5.0f, radius * 0.5, indicatorWidth, indicatorHeight);
    g.fillPath(indicator, juce::AffineTransform::rotation(angle + juce::float_Pi).translated(centerX, centerY));
}
