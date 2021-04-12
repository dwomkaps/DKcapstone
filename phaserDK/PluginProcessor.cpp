/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PhaserDKAudioProcessor::PhaserDKAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), tree(*this, nullptr, "Parameters", createParameters())
                        , allpass1(juce::dsp::IIR::Coefficients<float>::makeAllPass(44100.0, 5000.0))
                        , allpass2(juce::dsp::IIR::Coefficients<float>::makeAllPass(44100.0, 5000.0))
                        , allpass3(juce::dsp::IIR::Coefficients<float>::makeAllPass(44100.0, 5000.0))
                        , allpass4(juce::dsp::IIR::Coefficients<float>::makeAllPass(44100.0, 5000.0))
                        , allpass5(juce::dsp::IIR::Coefficients<float>::makeAllPass(44100.0, 5000.0))
                        , allpass6(juce::dsp::IIR::Coefficients<float>::makeAllPass(44100.0, 5000.0))
                        , allpass7(juce::dsp::IIR::Coefficients<float>::makeAllPass(44100.0, 5000.0))
                        , allpass8(juce::dsp::IIR::Coefficients<float>::makeAllPass(44100.0, 5000.0))
#endif
{
    lastSampleRate = 44100;
}

PhaserDKAudioProcessor::~PhaserDKAudioProcessor()
{
}

//==============================================================================
const juce::String PhaserDKAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PhaserDKAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PhaserDKAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PhaserDKAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PhaserDKAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PhaserDKAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PhaserDKAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PhaserDKAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PhaserDKAudioProcessor::getProgramName (int index)
{
    return {};
}

void PhaserDKAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PhaserDKAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    lastSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    allpass1.prepare(spec);
    allpass2.prepare(spec);
    allpass3.prepare(spec);
    allpass4.prepare(spec);
    allpass5.prepare(spec);
    allpass6.prepare(spec);
    allpass7.prepare(spec);
    allpass8.prepare(spec);

    allpass1.reset();
    allpass2.reset();
    allpass3.reset();
    allpass4.reset();
    allpass5.reset();
    allpass6.reset();
    allpass7.reset();
    allpass8.reset();


}

void PhaserDKAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PhaserDKAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PhaserDKAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't guaranteed to be empty - they may contain garbage).
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    juce::dsp::AudioBlock <float> block(buffer);
    updateFilter();
    int numPoles = getPoles();

    allpass1.process(juce::dsp::ProcessContextReplacing<float>(block));
    allpass2.process(juce::dsp::ProcessContextReplacing<float>(block));

    if (numPoles > 1) {
        allpass3.process(juce::dsp::ProcessContextReplacing<float>(block));
        allpass4.process(juce::dsp::ProcessContextReplacing<float>(block));
    }
    if (numPoles > 2) {
        allpass5.process(juce::dsp::ProcessContextReplacing<float>(block));
        allpass6.process(juce::dsp::ProcessContextReplacing<float>(block));
    }
    if (numPoles > 3) {
        allpass7.process(juce::dsp::ProcessContextReplacing<float>(block));
        allpass8.process(juce::dsp::ProcessContextReplacing<float>(block));
    }

}

//==============================================================================
bool PhaserDKAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PhaserDKAudioProcessor::createEditor()
{
    return new PhaserDKAudioProcessorEditor (*this);
}

//==============================================================================
void PhaserDKAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PhaserDKAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void PhaserDKAudioProcessor::updateFilter()
{
    float freq = *tree.getRawParameterValue("kCenterFreq");

    *allpass1.state = *juce::dsp::IIR::Coefficients<float>::makeAllPass(lastSampleRate, freq);
    *allpass2.state = *juce::dsp::IIR::Coefficients<float>::makeAllPass(lastSampleRate, freq);
    *allpass3.state = *juce::dsp::IIR::Coefficients<float>::makeAllPass(lastSampleRate, freq);
    *allpass4.state = *juce::dsp::IIR::Coefficients<float>::makeAllPass(lastSampleRate, freq);
    *allpass5.state = *juce::dsp::IIR::Coefficients<float>::makeAllPass(lastSampleRate, freq);
    *allpass6.state = *juce::dsp::IIR::Coefficients<float>::makeAllPass(lastSampleRate, freq);
    *allpass7.state = *juce::dsp::IIR::Coefficients<float>::makeAllPass(lastSampleRate, freq);
    *allpass8.state = *juce::dsp::IIR::Coefficients<float>::makeAllPass(lastSampleRate, freq);
}

int PhaserDKAudioProcessor::getPoles()
{
    int poleCounter = *tree.getRawParameterValue("kPoles");
    
    return poleCounter;
}

juce::AudioProcessorValueTreeState::ParameterLayout PhaserDKAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;

    params.add(std::make_unique<juce::AudioParameterFloat>("kCenterFreq", "Center Freq", 20.0, 20000.0, 5000.0));
    params.add(std::make_unique<juce::AudioParameterInt>("kPoles", "Poles", 1, 4, 1));
    params.add(std::make_unique<juce::AudioParameterFloat>("kFeedback", "Feedback", 0.0, 1.0, 0.0));
    params.add(std::make_unique<juce::AudioParameterFloat>("kLFOrate", "LFO rate", 0.1, 20.0, 5.0));
    params.add(std::make_unique<juce::AudioParameterFloat>("kLFOdepth", "LFO depth", 0.01, 1.0, 0.5));
    params.add(std::make_unique<juce::AudioParameterFloat>("kDryWet", "dry/wet", 0.0, 1.0, 0.5));
    
    
    return params;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PhaserDKAudioProcessor();
}
