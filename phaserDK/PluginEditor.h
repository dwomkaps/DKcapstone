/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PhaserDKAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PhaserDKAudioProcessorEditor (PhaserDKAudioProcessor&);
    ~PhaserDKAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PhaserDKAudioProcessor& audioProcessor;

    juce::Label centerFreqLabel, polesLabel, feedbackLabel, LFOrateLabel, LFOdepthLabel, dryWetLabel;
    juce::Slider centerFreqSlider, polesSlider, feedbackSlider, LFOrateSlider, LFOdepthSlider, dryWetSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> centerFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> polesAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> LFOrateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> LFOdepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetAttachment;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaserDKAudioProcessorEditor)
};
