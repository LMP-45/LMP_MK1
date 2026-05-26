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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LMP_MK1AudioProcessorEditor)
};
