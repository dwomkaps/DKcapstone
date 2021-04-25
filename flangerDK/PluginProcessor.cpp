/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlangerDKAudioProcessor::FlangerDKAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), tree(*this, nullptr, "Parameters", createParameters())
#endif
{
    //set default values
    


}

FlangerDKAudioProcessor::~FlangerDKAudioProcessor()
{
}

//==============================================================================
const juce::String FlangerDKAudioProcessor::getName() const
{
    return JucePlugin_Name;
}


const juce::String FlangerDKAudioProcessor::getInputChannelName(int channelIndex) const {
    return juce::String(channelIndex + 1);
}

const juce::String FlangerDKAudioProcessor::getOutputChannelName(int channelIndex) const
{
    return juce::String(channelIndex + 1);
}



bool FlangerDKAudioProcessor::isInputChannelStereoPair(int index) const {
    return true;
}

bool FlangerDKAudioProcessor::isOutputChannelStereoPair(int index) const {
    return true;
}


bool FlangerDKAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool FlangerDKAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool FlangerDKAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

bool FlangerDKAudioProcessor::silenceInProducesSilenceOut() const {
    return false;
}

double FlangerDKAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

double FlangerDKAudioProcessor::applyGain(double sample, double gainDb)
{
    double type = 20.0;
    double index = gainDb / type;
    double power = pow(10.0, index);
    return sample * power;
}

int FlangerDKAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FlangerDKAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FlangerDKAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String FlangerDKAudioProcessor::getProgramName(int index)
{
    return juce::String();
}

void FlangerDKAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void FlangerDKAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    flanger1.prepare(spec);
    flanger2.prepare(spec);

    pan1.prepare(spec);
    pan2.prepare(spec);

    flanger1.reset();
    flanger2.reset();
    pan1.reset();
    pan2.reset();

}

void FlangerDKAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FlangerDKAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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

void FlangerDKAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage). 
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...

    
    juce::dsp::AudioBlock <float> block(buffer);
    juce::dsp::AudioBlock<float> block2(buffer);
    //auto context = juce::dsp::ProcessContextReplacing<float>(block);
    //auto context2 = juce::dsp::ProcessContextReplacing<float>(block2);

    const auto& inputBlock = juce::dsp::ProcessContextReplacing<float>(block).getInputBlock();
    auto& outputBlock = juce::dsp::ProcessContextReplacing<float>(block).getOutputBlock();

    auto context = juce::dsp::ProcessContextNonReplacing<float>(inputBlock, outputBlock);
    //const auto& inputBlock2 = context2.getInputBlock();
    //auto& outputBlock2 = context2.getOutputBlock();
    /*
    const auto& panInputBlock1 = context.getOutputBlock();
    auto& panOutputBlock1 = juce::dsp::ProcessContextReplacing<float>(block).getOutputBlock();

    const auto& panInputBlock2 = context.getOutputBlock();
    auto& panOutputBlock2 = juce::dsp::ProcessContextReplacing<float>(block).getOutputBlock();
    
    auto panContext1 = juce::dsp::ProcessContextNonReplacing<float>(panInputBlock1, panOutputBlock1);
    auto panContext2 = juce::dsp::ProcessContextNonReplacing<float>(panInputBlock2, panOutputBlock2);
    */
    updateFlanger();

    flanger1.process(context);
    flanger2.process(context);
    /*
    updatePan();

    pan1.process(panContext1);
    pan2.process(panContext2);
    */
}

//==============================================================================
bool FlangerDKAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FlangerDKAudioProcessor::createEditor()
{
    return new FlangerDKAudioProcessorEditor(*this);
}

//==============================================================================
void FlangerDKAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FlangerDKAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void FlangerDKAudioProcessor::updateFlanger()
{
    float rate = *tree.getRawParameterValue("kRate");
    float depth = *tree.getRawParameterValue("kDepth");
    float delay = *tree.getRawParameterValue("kDelay");
    float delay2 = *tree.getRawParameterValue("kDelay2");
    float feedback = *tree.getRawParameterValue("kFeedback");
    float dryWet = *tree.getRawParameterValue("kDryWet");

    flanger1.setRate(rate);
    flanger1.setDepth(depth);
    flanger1.setCentreDelay(delay);
    flanger1.setFeedback(feedback);
    flanger1.setMix(dryWet);
    flanger2.setCentreDelay(delay2);
    flanger2.setRate(rate);
    flanger2.setDepth(depth);
    flanger2.setFeedback(feedback);
    flanger2.setMix(dryWet);
}

void FlangerDKAudioProcessor::updatePan()
{
    float panValue1 = *tree.getRawParameterValue("kPan1");
    float panValue2 = *tree.getRawParameterValue("kPan2");

    pan1.setPan(panValue1);
    pan2.setPan(panValue2);

}

juce::AudioProcessorValueTreeState::ParameterLayout FlangerDKAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;

    params.add(std::make_unique<juce::AudioParameterFloat>("kRate", "Rate", 0.1, 20.0, 0.5));
    params.add(std::make_unique<juce::AudioParameterFloat>("kDepth", "Depth", 0.01, 1.0, 0.5));
    params.add(std::make_unique<juce::AudioParameterInt>("kDelay", "Delay", 1, 20, 3));
    params.add(std::make_unique<juce::AudioParameterInt>("kDelay2", "Delay", 1, 20, 3));
    params.add(std::make_unique<juce::AudioParameterFloat>("kFeedback", "Feedback", 0.0, 0.99, 0.0));
    params.add(std::make_unique<juce::AudioParameterFloat>("kDryWet", "Dry/Wet", 0.0, 1.0, 0.5));
    params.add(std::make_unique<juce::AudioParameterFloat>("kPan1", "Delay 1 Pan", -1.0, 1.0, 0.0));
    params.add(std::make_unique<juce::AudioParameterFloat>("kPan2", "Delay 2 Pan", -1.0, 1.0, 0.0));
    
    return params;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlangerDKAudioProcessor();
}