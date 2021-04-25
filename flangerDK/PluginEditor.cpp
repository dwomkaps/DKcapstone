/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlangerDKAudioProcessorEditor::FlangerDKAudioProcessorEditor(FlangerDKAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), delayLabel("", "Delay (ms):"), delay2Label("", "Delay 2 (ms)"),
    dryWetLabel("", "dry/wet:"), feedbackLabel("", "feedback:"), rateLabel("", "LFO rate"),
    depthLabel("", "LFO depth"), pan1Label("", "Delay 1 Pan"), pan2Label("", "Delay 2 Pan")

{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 300);

    delaySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    delaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&delaySlider);
    delayLabel.attachToComponent(&delaySlider, false);
    delayLabel.setFont(juce::Font(11.0));
    delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kDelay", delaySlider);

    delay2Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    delay2Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&delay2Slider);
    delay2Label.attachToComponent(&delay2Slider, false);
    delay2Label.setFont(juce::Font(11.0));
    delay2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kDelay2", delay2Slider);

    dryWetSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&dryWetSlider);
    dryWetLabel.attachToComponent(&dryWetSlider, false);
    dryWetLabel.setFont(juce::Font(11.0));
    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kDryWet", dryWetSlider);

    feedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&feedbackSlider);
    feedbackLabel.attachToComponent(&feedbackSlider, false);
    feedbackLabel.setFont(juce::Font(11.0));
    feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kFeedback", feedbackSlider);

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

    pan1Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pan1Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&pan1Slider);
    pan1Label.attachToComponent(&pan1Slider, false);
    pan1Label.setFont(juce::Font(11.0));
    pan1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kPan1", pan1Slider);

    pan2Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pan2Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(&pan2Slider);
    pan2Label.attachToComponent(&pan2Slider, false);
    pan2Label.setFont(juce::Font(11.0));
    pan2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kPan2", pan2Slider);

}

FlangerDKAudioProcessorEditor::~FlangerDKAudioProcessorEditor()
{
}




//==============================================================================
void FlangerDKAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::yellowgreen);

    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("flanger", getLocalBounds(), juce::Justification::centred, 1);
}

void FlangerDKAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // sets the position and size of the slider with arguments (x, y, width, height)
    delaySlider.setBounds(20, 20, 150, 70);
    delay2Slider.setBounds(160, 20, 150, 70);
    pan1Slider.setBounds(300, 20, 150, 70);
    pan2Slider.setBounds(440, 20, 150, 70);
    dryWetSlider.setBounds(400, 180, 150, 70);
    feedbackSlider.setBounds(20, 180, 150, 70);
    rateSlider.setBounds(20, 100, 150, 70);
    depthSlider.setBounds(400, 100, 150, 70);
}
