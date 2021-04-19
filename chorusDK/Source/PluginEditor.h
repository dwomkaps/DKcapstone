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
class ChorusDKAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ChorusDKAudioProcessorEditor (ChorusDKAudioProcessor&);
    ~ChorusDKAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    //void sliderValueChanged(juce::Slider* slider);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ChorusDKAudioProcessor& audioProcessor;

    juce::Label cutoffLabel, qLabel, rateLabel, depthLabel, delayLabel, feedbackLabel, dryWetLabel;
    juce::Slider cutoffSlider, qSlider, rateSlider, depthSlider, delaySlider, feedbackSlider, dryWetSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetAttachment;




    ChorusDKAudioProcessor* getProcessor() const {
        return static_cast <ChorusDKAudioProcessor*> (getAudioProcessor());
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusDKAudioProcessorEditor)
};
