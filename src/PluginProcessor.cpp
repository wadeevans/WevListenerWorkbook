/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ParametersAudioProcessor::ParametersAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
#endif
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    apvts.addParameterListener("gain", this);
}

ParametersAudioProcessor::~ParametersAudioProcessor()
{
    apvts.removeParameterListener("gain", this);
}

//==============================================================================
const juce::String ParametersAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ParametersAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ParametersAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ParametersAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ParametersAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ParametersAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int ParametersAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ParametersAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String ParametersAudioProcessor::getProgramName(int index)
{
    return {};
}

void ParametersAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
}

//==============================================================================
void ParametersAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ParametersAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ParametersAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void ParametersAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // auto gainValue = apvts.getRawParameterValue("gain")->load();
    // this is what we are trying to avoid by using the parameterChanged callback to update an atomic variable
    auto gainValue = gainAtomic.load();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto *channelData = buffer.getWritePointer(channel);

        // ..do something to the data...
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= gainValue;
        }
    }
}

//==============================================================================
bool ParametersAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *ParametersAudioProcessor::createEditor()
{
    return new ParametersAudioProcessorEditor(*this);
}

//==============================================================================
void ParametersAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ParametersAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout ParametersAudioProcessor::createParameterLayout()
{
    return {std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"gain", 1}, // parameter ID - used to reference it in code
        "Gain",                       // parameter name - shown in host automation
        0.0f,                         // minimum
        1.0f,                         // maximum
        1.0f                          // default
        )};
}

void ParametersAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == "gain")
    {
        gainAtomic.store(newValue);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new ParametersAudioProcessor();
}
