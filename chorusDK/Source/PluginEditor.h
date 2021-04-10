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
class ChorusDKAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    ChorusDKAudioProcessorEditor (ChorusDKAudioProcessor&);
    ~ChorusDKAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ChorusDKAudioProcessor& audioProcessor;

    juce::Label cutoffLabel, qLabel;
    juce::Slider cutoffSlider, qSlider;

    ChorusDKAudioProcessor* getProcessor() const {
        return static_cast <ChorusDKAudioProcessor*> (getAudioProcessor());
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusDKAudioProcessorEditor)
};
