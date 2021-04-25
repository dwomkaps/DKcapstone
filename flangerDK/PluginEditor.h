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
class FlangerDKAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FlangerDKAudioProcessorEditor (FlangerDKAudioProcessor&);
    ~FlangerDKAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FlangerDKAudioProcessor& audioProcessor;

    juce::Label delayLabel, delay2Label, dryWetLabel, feedbackLabel, rateLabel, depthLabel, pan1Label, pan2Label;
    juce::Slider delaySlider, delay2Slider, dryWetSlider, feedbackSlider, rateSlider, depthSlider, pan1Slider, pan2Slider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delay2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pan1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pan2Attachment;

    FlangerDKAudioProcessor* getProcessor() const {
        return static_cast <FlangerDKAudioProcessor*>(getAudioProcessor());
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlangerDKAudioProcessorEditor)
};
