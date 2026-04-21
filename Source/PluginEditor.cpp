/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TransientDesignerAudioProcessorEditor::TransientDesignerAudioProcessorEditor (TransientDesignerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
      attackAttachment  (p.apvts, "attack",  attackSlider),
      sustainAttachment (p.apvts, "sustain", sustainSlider),
      bypassAttachment  (p.apvts, "bypass",  bypassButton)
{
    attackSlider.setSliderStyle  (juce::Slider::RotaryVerticalDrag);
    attackSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible (attackSlider);

    sustainSlider.setSliderStyle  (juce::Slider::RotaryVerticalDrag);
    sustainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible (sustainSlider);

    attackLabel.setText ("Attack", juce::dontSendNotification);
    attackLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (attackLabel);

    sustainLabel.setText ("Sustain", juce::dontSendNotification);
    sustainLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (sustainLabel);

    addAndMakeVisible (bypassButton);

    setSize (400, 250);
}

TransientDesignerAudioProcessorEditor::~TransientDesignerAudioProcessorEditor()
{
}

//==============================================================================
void TransientDesignerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff1e1e1e));
}

void TransientDesignerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (20);

    // Bypass button sits at the top centre
    bypassButton.setBounds (area.removeFromTop (30).withSizeKeepingCentre (80, 24));

    area.removeFromTop (10);

    // Two knob columns side by side
    auto knobArea = area;
    int knobWidth = knobArea.getWidth() / 2;

    auto attackArea  = knobArea.removeFromLeft (knobWidth);
    auto sustainArea = knobArea;

    attackLabel .setBounds (attackArea .removeFromTop (20));
    sustainLabel.setBounds (sustainArea.removeFromTop (20));

    attackSlider .setBounds (attackArea);
    sustainSlider.setBounds (sustainArea);
}
