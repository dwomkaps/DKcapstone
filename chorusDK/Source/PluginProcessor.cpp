/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusDKAudioProcessor::ChorusDKAudioProcessor()

{
    //default values
    cutoff = 18000;
    q = 0.7;
    //biquadDK::biquadDK();
    

}

ChorusDKAudioProcessor::~ChorusDKAudioProcessor()
{
}

//==============================================================================
const juce::String ChorusDKAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int ChorusDKAudioProcessor::getNumParameters()
{
    return kNumParams;
}

float ChorusDKAudioProcessor::getParameter(int index)
{
    switch (index) {
    case kCutoff:
        return biquad[0].getCutoff();
    case kQ:
        return biquad[0].getQ();
    default:
        return 0.0f;
    }
}

void ChorusDKAudioProcessor::setParameter(int index, float newValue)
{
    switch (index) {
    case kCutoff:
        for (int i = 0; i < kChannels; i++) {
            biquad[i].setCutoff(newValue);
        }
        break;
    case kQ:
        for (int i = 0; i < kChannels; i++) {
            biquad[i].setQ(newValue);
        }
        break;

    }
}

const juce::String ChorusDKAudioProcessor::getParameterName(int index)
{
    switch (index) {
    case kCutoff:
        return "cutoff";
    case kQ:
        return "Q";
    default:
        return "";
    }
    
}

const juce::String ChorusDKAudioProcessor::getParameterText(int index)
{
    return juce::String();
}

bool ChorusDKAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChorusDKAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChorusDKAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChorusDKAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChorusDKAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChorusDKAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChorusDKAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ChorusDKAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChorusDKAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ChorusDKAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for (int i = 0; i < kChannels; i++) {
        biquad[i].setSampleRate(sampleRate);
        biquad[i].setCutoff(18000); //initial cutoff
        biquad[i].setQ(0.7);
        biquad[i].prepareToPlay();
    }
}

void ChorusDKAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChorusDKAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ChorusDKAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //input data
        auto* inputData = buffer.getReadPointer (channel);

        //output data
        auto* outputData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; sample++) {
            float inputSample = inputData[sample];

            outputData[sample] = biquad[channel].lowpass(inputSample);
        }

        // ..do something to the data...
    }
}

//==============================================================================
bool ChorusDKAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ChorusDKAudioProcessor::createEditor()
{
    return new ChorusDKAudioProcessorEditor (*this);
}

//==============================================================================
void ChorusDKAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ChorusDKAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChorusDKAudioProcessor();
}
