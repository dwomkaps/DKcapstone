/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusDKAudioProcessorEditor::ChorusDKAudioProcessorEditor (ChorusDKAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), cutoffLabel("", "cutoff"), qLabel("", "Q"), rateLabel("", "LFO rate"), 
    depthLabel("", "LFO depth"), delayLabel("", "delay (ms)"), feedbackLabel("", "feedback"), dryWetLabel("", "dry/wet")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    cutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&cutoffSlider);
    cutoffLabel.attachToComponent(&cutoffSlider, false);
    cutoffLabel.setFont(juce::Font(11.0));
    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kCutoff", cutoffSlider);

    qSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&qSlider);
    qLabel.attachToComponent(&qSlider, false);
    qLabel.setFont(juce::Font(11.0));
    qAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kQ", qSlider);

    rateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&rateSlider);
    rateLabel.attachToComponent(&rateSlider, false);
    rateLabel.setFont(juce::Font(11.0));
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kRate", rateSlider);

    depthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&depthSlider);
    depthLabel.attachToComponent(&depthSlider, false);
    depthLabel.setFont(juce::Font(11.0));
    depthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kDepth", depthSlider);

    delaySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    delaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&delaySlider);
    delayLabel.attachToComponent(&delaySlider, false);
    delayLabel.setFont(juce::Font(11.0));
    delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kDelay", delaySlider);

    feedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&feedbackSlider);
    feedbackLabel.attachToComponent(&feedbackSlider, false);
    feedbackLabel.setFont(juce::Font(11.0));
    feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kFeedback", feedbackSlider);

    dryWetSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&dryWetSlider);
    dryWetLabel.attachToComponent(&dryWetSlider, false);
    dryWetLabel.setFont(juce::Font(11.0));
    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kDryWet", dryWetSlider);

    

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
    qSlider.setBounds(240, 20, 140, 70);
    rateSlider.setBounds(20, 180, 140, 70);
    depthSlider.setBounds(130, 180, 140, 70);
    delaySlider.setBounds(20, 100, 140, 70);
    feedbackSlider.setBounds(240, 100, 140, 70);
    dryWetSlider.setBounds(240, 180, 140, 70);


}
/*
void ChorusDKAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &cutoffSlider) {
        getProcessor()->setParameterNotifyingHost(ChorusDKAudioProcessor::kCutoff, (float)cutoffSlider.getValue());
    }
    else if(slider == &qSlider){
        getProcessor()->setParameterNotifyingHost(ChorusDKAudioProcessor::kQ, (float)qSlider.getValue());
    }
    
}*/
