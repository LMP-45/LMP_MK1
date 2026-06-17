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

    polyphonySlider.setSliderStyle(
    juce::Slider::LinearVertical);

    polyphonySlider.setTextBoxStyle(
        juce::Slider::TextBoxBelow,
        false,
        60,
        20);
    addAndMakeVisible(polyphonySlider);

    lfoRateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lfoRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
        false,
        60,
        20);
    addAndMakeVisible(lfoRateSlider);

    lfoDepthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lfoDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
        false,
        60,
        20);
    addAndMakeVisible(lfoDepthSlider);


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

    polyphonyAttachment = std::make_unique<SliderAttachment>(
       audioProcessor.apvts,
       "polyphony", polyphonySlider);

    lfoRateAttachment = std::make_unique<SliderAttachment>(
       audioProcessor.apvts,
       "lfoRate", lfoRateSlider);

    lfoDepthAttachment = std::make_unique<SliderAttachment>(
       audioProcessor.apvts,
       "lfoDepth", lfoDepthSlider);




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
            0, 320, 100, 30,
            juce::Justification::centred);

    g.drawText("Osc2 Level",
               80, 320, 100, 30,
               juce::Justification::centred);

    g.drawText("D",
               300, 200, 100, 50,
               juce::Justification::centred);

    g.drawText("A",
               230, 200, 100, 50,
               juce::Justification::centred);

    g.drawText("R",
               440, 200, 100, 50,
               juce::Justification::centred);

    g.drawText("S",
               370, 200, 100, 50,
               juce::Justification::centred);

    g.drawText("polyphony",
              160, 320, 100, 30,
              juce::Justification::centred);

    g.drawText("LFO Depth",
    300, 320, 100, 30,
             juce::Justification::centred);

    g.drawText("LFO Rate",
            230, 320, 100, 30,
            juce::Justification::centred);

}

void LMP_MK1AudioProcessorEditor::resized()
{
    osc1Slider.setBounds(20, 350, 60, 50);
    osc2Slider.setBounds(100, 350, 60, 50);

    decaySlider.setBounds(320, 110, 60, 100);
    attackSlider.setBounds(250, 110, 60, 100);
    releaseSlider.setBounds(460, 110, 60, 100);
    sustainSlider.setBounds(390, 110, 60, 100);

    polyphonySlider.setBounds(180, 350, 60, 100);

    lfoRateSlider.setBounds(250, 350, 60, 100);
    lfoDepthSlider.setBounds(320, 350, 60, 100);
}

