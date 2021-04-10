/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "delayDK.h"
//==============================================================================
/**
*/
class FlangerDKAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    FlangerDKAudioProcessor();
    ~FlangerDKAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    int getNumParameters() override;
    float getParameter(int index) override;
    void setParameter(int index, float newValue) override;

    const juce::String getParameterName(int index) override;
    const juce::String getParameterText(int index) override;

    const juce::String getInputChannelName(int channelIndex) const override;
    const juce::String getOutputChannelName(int channelIndex) const override;
    bool isInputChannelStereoPair(int index) const override;
    bool isOutputChannelStereoPair(int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    double applyGain(double sample, double gainDb);

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;



    enum Params {
        kDelayTime = 0,
        kDryWet,
        kFeedback,
        kInGain,
        kOutGain,
        kNumParams
    };

    double delayTime;
    double dryWet;
    double feedback;
    double inGain;
    double outGain;


private:
    // circular buffer variables
    juce::AudioBuffer<float> delayBuffer;
    int delayBufferLength;
    int delayReadPosition;
    int delayWritePosition;

    juce::Random noise;
    //juce::dsp::DelayLine<float> delayLine;
    
    static const int kChannels = 2;
    delayDK delayLine[kChannels];
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlangerDKAudioProcessor)
};