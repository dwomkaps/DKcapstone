/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PhaserDKAudioProcessorEditor::PhaserDKAudioProcessorEditor (PhaserDKAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), centerFreqLabel("", "center freq"), polesLabel("", "poles"), 
    feedbackLabel("", "feedback"), LFOrateLabel("", "LFO rate (Hz)"), LFOdepthLabel("", "LFO depth"), dryWetLabel("", "dry/wet")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    centerFreqSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    centerFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    addAndMakeVisible(&centerFreqSlider);
    centerFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kCenterFreq", centerFreqSlider);

    polesSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    polesSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    addAndMakeVisible(&polesSlider);
    polesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kPoles", polesSlider);

    feedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    addAndMakeVisible(&feedbackSlider);
    feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kFeedback", feedbackSlider);

    LFOrateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    LFOrateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    addAndMakeVisible(&LFOrateSlider);
    LFOrateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kLFOrate", LFOrateSlider);

    LFOdepthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    LFOdepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    addAndMakeVisible(&LFOdepthSlider);
    LFOdepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kLFOdepth", LFOdepthSlider);

    dryWetSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    addAndMakeVisible(&dryWetSlider);
    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree, "kDryWet", dryWetSlider);

    centerFreqLabel.attachToComponent(&centerFreqSlider, false);
    centerFreqLabel.setFont(juce::Font(11.0));

    polesLabel.attachToComponent(&polesSlider, false);
    polesLabel.setFont(juce::Font(11.0));

    feedbackLabel.attachToComponent(&feedbackSlider, false);
    feedbackLabel.setFont(juce::Font(11.0));

    LFOrateLabel.attachToComponent(&LFOrateSlider, false);
    LFOrateLabel.setFont(juce::Font(11.0));

    LFOdepthLabel.attachToComponent(&LFOdepthSlider, false);
    LFOdepthLabel.setFont(juce::Font(11.0));

    dryWetLabel.attachToComponent(&dryWetSlider, false);
    dryWetLabel.setFont(juce::Font(11.0));

}

PhaserDKAudioProcessorEditor::~PhaserDKAudioProcessorEditor()
{
}

//==============================================================================
void PhaserDKAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::magenta);

    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("phaser", getLocalBounds(), juce::Justification::centred, 1);
}

void PhaserDKAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    centerFreqSlider.setBounds(20, 20, 140, 70);
    polesSlider.setBounds(130, 20, 140, 70);
    feedbackSlider.setBounds(240, 20, 140, 70);
    LFOrateSlider.setBounds(20, 200, 140, 70);
    LFOdepthLabel.setBounds(130, 200, 140, 70);
    dryWetSlider.setBounds(240, 200, 140, 70);

}
