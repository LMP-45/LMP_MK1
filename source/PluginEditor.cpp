#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LMP_MK1AudioProcessorEditor::LMP_MK1AudioProcessorEditor
    (LMP_MK1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    osc1Slider.setSliderStyle(
    juce::Slider::LinearHorizontal);

    osc1Slider.setTextBoxStyle(
        juce::Slider::TextBoxBelow,
        false,
        60,
        20);

    osc1Slider.setTextValueSuffix(" %");

    addAndMakeVisible(osc1Slider);

    osc2Slider.setSliderStyle(
    juce::Slider::LinearHorizontal);

    osc2Slider.setTextBoxStyle(
        juce::Slider::TextBoxBelow,
        false,
        60,
        20);


    osc2Slider.setTextValueSuffix(" %");

    addAndMakeVisible(osc2Slider);

    decaySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
        false,
        60,
        20);
    addAndMakeVisible(decaySlider);

    attackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
        false,
        60,
        20);
    addAndMakeVisible(attackSlider);

    sustainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
        false,
        60,
        20);
    addAndMakeVisible(sustainSlider);

    releaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
        false,
        60,
        20);
    addAndMakeVisible(releaseSlider);

    // APVTS attachments
    osc1Attachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        "osc1Level",
        osc1Slider);

    osc2Attachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        "osc2Level",
        osc2Slider);

    decayAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts,
        "decay", decaySlider);

    attackAttachment = std::make_unique<SliderAttachment>(
       audioProcessor.apvts,
       "attack", attackSlider);

    releaseAttachment = std::make_unique<SliderAttachment>(
       audioProcessor.apvts,
       "release", releaseSlider);

    sustainAttachment = std::make_unique<SliderAttachment>(
       audioProcessor.apvts,
       "sustain", sustainSlider);

    setSize(700, 500);
}

LMP_MK1AudioProcessorEditor::~LMP_MK1AudioProcessorEditor()
{
}

//==============================================================================
void LMP_MK1AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);

    g.drawText("Osc1 Level",
    60, 300, 100, 50,
               juce::Justification::centred);

    g.drawText("Osc2 Level",
               180, 300, 100, 50,
               juce::Justification::centred);

    g.drawText("Decay Level",
               280, 300, 100, 50,
               juce::Justification::centred);

    g.drawText("attack Level",
               380, 300, 100, 50,
               juce::Justification::centred);

    g.drawText("release Level",
               480, 300, 100, 50,
               juce::Justification::centred);

    g.drawText("sustain Level",
               580, 300, 100, 50,
               juce::Justification::centred);

}

void LMP_MK1AudioProcessorEditor::resized()
{
    osc1Slider.setBounds(80, 200, 60, 100);
    osc2Slider.setBounds(200, 200, 60, 100);
    decaySlider.setBounds(300, 200, 60, 100);
    attackSlider.setBounds(400, 200, 60, 100);
    releaseSlider.setBounds(500, 200, 60, 100);
    sustainSlider.setBounds(600, 200, 60, 100);
}

