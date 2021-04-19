/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "biquadDK.h"

//==============================================================================
/**
*/
class ChorusDKAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ChorusDKAudioProcessor();
    ~ChorusDKAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void updateFilter();
    void updateChorus();
    void updateMix();
    /*
    enum Params {
        kCutoff = 0,
        kQ,
        kNumParams
    };

    double cutoff;
    double q;
    */
    juce::AudioProcessorValueTreeState tree;

private:
    
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highpass1;
    float lastSampleRate;
    //actual chorus time
    juce::dsp::Chorus<float> chorus;
    juce::dsp::DryWetMixer<float> mixer;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusDKAudioProcessor)
};
