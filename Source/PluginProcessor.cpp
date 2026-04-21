/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout TransientDesignerAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("attack",  "Attack",  -1.f, 1.f, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("sustain", "Sustain", -1.f, 1.f, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterBool> ("bypass",  "Bypass",  false));

    return { params.begin(), params.end() };
}

TransientDesignerAudioProcessor::TransientDesignerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
     :
#endif
       apvts (*this, nullptr, "Parameters", createParameterLayout())
{
}

TransientDesignerAudioProcessor::~TransientDesignerAudioProcessor()
{
}

//==============================================================================
const juce::String TransientDesignerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TransientDesignerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TransientDesignerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TransientDesignerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TransientDesignerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TransientDesignerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TransientDesignerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TransientDesignerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TransientDesignerAudioProcessor::getProgramName (int index)
{
    return {};
}

void TransientDesignerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TransientDesignerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    transientDesigner.prepareToPlay(sampleRate);
}

void TransientDesignerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TransientDesignerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TransientDesignerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (apvts.getRawParameterValue("bypass")->load() > 0.5f)
        return;

    float attackVal  = apvts.getRawParameterValue("attack") ->load();
    float sustainVal = apvts.getRawParameterValue("sustain")->load();

    transientDesigner.setAttack  (attackVal);
    transientDesigner.setSustain (sustainVal);

    int N = buffer.getNumSamples();
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        transientDesigner.processBuffer(channelData, channel, N);
    }
}

//==============================================================================
bool TransientDesignerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TransientDesignerAudioProcessor::createEditor()
{
    return new TransientDesignerAudioProcessorEditor (*this);
}

//==============================================================================
void TransientDesignerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void TransientDesignerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TransientDesignerAudioProcessor();
}
