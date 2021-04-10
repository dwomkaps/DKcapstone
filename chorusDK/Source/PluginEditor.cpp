/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusDKAudioProcessorEditor::ChorusDKAudioProcessorEditor (ChorusDKAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), cutoffLabel("", "cutoff"), qLabel("", "Q")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    cutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.addListener(this);
    cutoffSlider.setRange(20.0, 20000.0, 0.01);
    addAndMakeVisible(&cutoffSlider);

    qSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    qSlider.addListener(this);
    qSlider.setRange(0.01, 2.0, 0.01);
    addAndMakeVisible(&qSlider);


}

ChorusDKAudioProcessorEditor::~ChorusDKAudioProcessorEditor()
{
}

//==============================================================================
void ChorusDKAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::skyblue);

    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("a filter for now", getLocalBounds(), juce::Justification::centred, 1);
}

void ChorusDKAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    cutoffSlider.setBounds(20, 20, 140, 70);
    qSlider.setBounds(200, 20, 140, 70);


}

void ChorusDKAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &cutoffSlider) {
        getProcessor()->setParameterNotifyingHost(ChorusDKAudioProcessor::kCutoff, (float)cutoffSlider.getValue());
    }
    else if(slider == &qSlider){
        getProcessor()->setParameterNotifyingHost(ChorusDKAudioProcessor::kQ, (float)qSlider.getValue());
    }
}
