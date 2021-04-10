/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlangerDKAudioProcessor::FlangerDKAudioProcessor() : delayBuffer(2, 1)
{
    //set default values
    delayTime = 0.5; //s
    dryWet = 0.5;
    feedback = 0.0;
    inGain = 0.0;
    outGain = 0.0;

    

    delayBufferLength = 1;

    delayReadPosition = 0;
    delayWritePosition = 0;

}

FlangerDKAudioProcessor::~FlangerDKAudioProcessor()
{
}

//==============================================================================
const juce::String FlangerDKAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int FlangerDKAudioProcessor::getNumParameters() {
    return kNumParams;
}

float FlangerDKAudioProcessor::getParameter(int index) {
    switch (index) {
    case kDelayTime:
        return delayTime;
    case kDryWet:
        return dryWet;
    case kFeedback:
        return feedback;
    case kInGain:
        return inGain;
    case kOutGain:
        return outGain;
    default:
        return 0.0;
    }
}

void FlangerDKAudioProcessor::setParameter(int index, float newValue) {
    switch (index) {
    case kDelayTime:
        delayTime = newValue;
        delayReadPosition = (int)(delayWritePosition - (delayTime * getSampleRate()) + delayBufferLength) % delayBufferLength;
        break;
    case kDryWet:
        dryWet = newValue;
        break;
    case kFeedback:
        feedback = newValue;
        break;
    case kInGain:
        inGain = newValue;
        break;
    case kOutGain:
        outGain = newValue;
        break;
    default:
        break;
    }
}

const juce::String FlangerDKAudioProcessor::getParameterName(int index) {
    switch (index) {
    case kDelayTime:
        return "delay";
    case kDryWet:
        return "dry/wet";
    case kFeedback:
        return "feedback";
    case kInGain:
        return "input gain";
    case kOutGain:
        return "output gain";
    default:
        return "";
    }
}

const juce::String FlangerDKAudioProcessor::getParameterText(int index) {
    return juce::String();
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
    delayBufferLength = ((int)2.0 * sampleRate);
    if (delayBufferLength < 1)
        delayBufferLength = 1;

    delayBuffer.setSize(2, delayBufferLength);
    delayBuffer.clear();

    delayReadPosition = (int)(delayWritePosition - (delayTime * getSampleRate()) + delayBufferLength) % delayBufferLength;

    for (int i = 0; i < getTotalNumInputChannels(); i++) {
        delayLine[i].prepareToPlay(sampleRate);
    }


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
    //fix the delay thing with the dsprelated techniqur
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();

    int dpr, dpw;


    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        //auto* delayData = delayBuffer.getWritePointer(juce::jmin(channel, delayBuffer.getNumChannels() - 1));

        //dpr = delayReadPosition;
        //dpw = delayWritePosition;
        delayLine[channel].setDprDpw(delayReadPosition, delayWritePosition);

        for (int i = 0; i < numSamples; ++i) {

            const float dry = channelData[i];
            const float in = applyGain(dry, inGain);

            float wetSignal = 0.0;
            float out = 0.0;

            //float noiseSample = noise.nextFloat() * 0.001f - 0.0005f; //noise generator between 0 and 1.0
            //noiseSample = juce::jmap(noiseSample, (float)0.0, (float)1.0, (float)-0.1, (float)0.1);
            //noiseSample = applyGain(noiseSample, -36.0);

            /*
            long rpi = (long)floor(delayData[dpr]); //linear interpolation
            wetSignal = delayData[dpr] - (double)rpi; //transfer to class delayDK
            */
            wetSignal = delayLine[channel].delayProcess(in, feedback);

            double dryOut = dry * (1.0 - dryWet);
            double wetOut = wetSignal * dryWet;

            out = applyGain((dryOut + wetOut), outGain); 

            //delayData[dpw] = in + (wetSignal * feedback); //this should go in delayDK as well
            /*
            if (++dpr >= delayBufferLength) //maybe these but possibly not try without first
                dpr = 0;
            if (++dpw >= delayBufferLength)
                dpw = 0;
            */
            channelData[i] = out;
        }

    }

    delayReadPosition = dpr;
    delayWritePosition = dpw;

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
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

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlangerDKAudioProcessor();
}