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
class FlangerDKAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener, public juce::Timer
{
public:
    FlangerDKAudioProcessorEditor (FlangerDKAudioProcessor&);
    ~FlangerDKAudioProcessorEditor() override;

    //==============================================================================
    void timerCallback();
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FlangerDKAudioProcessor& audioProcessor;

    juce::Label delayTimeLabel, dryWetLabel, feedbackLabel, inGainLabel, outGainLabel;
    juce::Slider delayTimeSlider, dryWetSlider, feedbackSlider, inGainSlider, outGainSlider;

    FlangerDKAudioProcessor* getProcessor() const {
        return static_cast <FlangerDKAudioProcessor*>(getAudioProcessor());
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlangerDKAudioProcessorEditor)
};
