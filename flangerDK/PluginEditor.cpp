/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlangerDKAudioProcessorEditor::FlangerDKAudioProcessorEditor (FlangerDKAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), delayTimeLabel("", "Delay (ms):"), 
    dryWetLabel("", "dry/wet:"), feedbackLabel("", "feedback:"), inGainLabel("", "input gain:"),
    outGainLabel("", "output gain:")

{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    delayTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    delayTimeSlider.addListener(this);
    delayTimeSlider.setRange(.003, 1.0, 0.001);
    addAndMakeVisible(&delayTimeSlider);

    dryWetSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dryWetSlider.addListener(this);
    dryWetSlider.setRange(0.0, 1.0, 0.01);
    addAndMakeVisible(&dryWetSlider);

    feedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    feedbackSlider.addListener(this);
    feedbackSlider.setRange(0.0, 0.99, 0.01);
    addAndMakeVisible(&feedbackSlider);

    inGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    inGainSlider.addListener(this);
    inGainSlider.setRange(-64.0, 16.0, 0.01); //gain in dB
    addAndMakeVisible(&inGainSlider);

    outGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outGainSlider.addListener(this);
    outGainSlider.setRange(-64.0, 16.0, 0.01);
    addAndMakeVisible(&outGainSlider);


    delayTimeLabel.attachToComponent(&delayTimeSlider, false);
    delayTimeLabel.setFont(juce::Font(11.0));

    dryWetLabel.attachToComponent(&dryWetSlider, false);
    dryWetLabel.setFont(juce::Font(11.0));

    feedbackLabel.attachToComponent(&feedbackSlider, false);
    feedbackLabel.setFont(juce::Font(11.0));

    inGainLabel.attachToComponent(&inGainSlider, false);
    inGainLabel.setFont(juce::Font(11.0));

    outGainLabel.attachToComponent(&outGainSlider, false);
    outGainLabel.setFont(juce::Font(11.0));

    startTimer(50);

}

FlangerDKAudioProcessorEditor::~FlangerDKAudioProcessorEditor()
{
}

void FlangerDKAudioProcessorEditor::timerCallback() {
    FlangerDKAudioProcessor* ourProcessor = getProcessor();

    delayTimeSlider.setValue(ourProcessor->delayTime, juce::dontSendNotification);
    dryWetSlider.setValue(ourProcessor->dryWet, juce::dontSendNotification);
    feedbackSlider.setValue(ourProcessor->feedback, juce::dontSendNotification);
    inGainSlider.setValue(ourProcessor->inGain, juce::dontSendNotification);
    outGainSlider.setValue(ourProcessor->outGain, juce::dontSendNotification);


}

void FlangerDKAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &delayTimeSlider) {
        getProcessor()->setParameterNotifyingHost(FlangerDKAudioProcessor::kDelayTime, (float)delayTimeSlider.getValue());
    }
    else if (slider == &dryWetSlider) {
        getProcessor()->setParameterNotifyingHost(FlangerDKAudioProcessor::kDryWet, (float)dryWetSlider.getValue());
    }
    else if (slider == &feedbackSlider) {
        getProcessor()->setParameterNotifyingHost(FlangerDKAudioProcessor::kFeedback, (float)feedbackSlider.getValue());
    }
    else if (slider == &inGainSlider) {
        getProcessor()->setParameterNotifyingHost(FlangerDKAudioProcessor::kInGain, (float)inGainSlider.getValue());
    }
    else if (slider == &outGainSlider) {
        getProcessor()->setParameterNotifyingHost(FlangerDKAudioProcessor::kOutGain, (float)outGainSlider.getValue());
    }

}
//==============================================================================
void FlangerDKAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::yellowgreen);

    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("basic ass delay", getLocalBounds(), juce::Justification::centred, 1);
}

void FlangerDKAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // sets the position and size of the slider with arguments (x, y, width, height)
    delayTimeSlider.setBounds(20, 20, 150, 70);
    dryWetSlider.setBounds(200, 80, 150, 70);
    feedbackSlider.setBounds(200, 20, 150, 70);
    inGainSlider.setBounds(20, 150, 150, 70);
    outGainSlider.setBounds(200, 150, 150, 70);
}
