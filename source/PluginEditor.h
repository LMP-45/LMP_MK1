#pragma once

#include "PluginProcessor.h"

//==============================================================================
class LMP_MK1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    LMP_MK1AudioProcessorEditor (LMP_MK1AudioProcessor&);
    ~LMP_MK1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LMP_MK1AudioProcessor& audioProcessor;

    juce::Slider osc1Slider;
    juce::Slider osc2Slider;

    juce::Slider decaySlider;
    juce::Slider attackSlider;
    juce::Slider releaseSlider;
    juce::Slider sustainSlider;

    juce::Slider polyphonySlider;

    using SliderAttachment =
    juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> osc1Attachment;
    std::unique_ptr<SliderAttachment> osc2Attachment;

    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;

    std::unique_ptr<SliderAttachment> polyphonyAttachment;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LMP_MK1AudioProcessorEditor)
};
