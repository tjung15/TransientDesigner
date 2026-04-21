/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class TransientDesignerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TransientDesignerAudioProcessorEditor (TransientDesignerAudioProcessor&);
    ~TransientDesignerAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TransientDesignerAudioProcessor& audioProcessor;

    juce::Slider attackSlider;
    juce::Slider sustainSlider;
    juce::Label  attackLabel;
    juce::Label  sustainLabel;

    juce::ToggleButton bypassButton { "Bypass" };

    juce::AudioProcessorValueTreeState::SliderAttachment attackAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment sustainAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransientDesignerAudioProcessorEditor)
};
