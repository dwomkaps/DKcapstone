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
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    addAndMakeVisible(&cutoffSlider);

    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kCutoff", cutoffSlider);

    qSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    addAndMakeVisible(&qSlider);

    qAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kQ", qSlider);

    cutoffLabel.attachToComponent(&cutoffSlider, false);
    cutoffLabel.setFont(juce::Font(11.0));

    qLabel.attachToComponent(&qSlider, false);
    qLabel.setFont(juce::Font(11.0));

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
    cutoffSlider.setBounds(20, 100, 140, 70);
    qSlider.setBounds(200, 100, 140, 70);


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
