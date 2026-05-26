#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Synth.h"

namespace ParameterID
{
#define PARAMETER_ID(str) const juce::ParameterID str(#str, 1);
    PARAMETER_ID (osc1Level)
#undef PARAMETER_ID
}

//==============================================================================
class LMP_MK1AudioProcessor  : public juce::AudioProcessor,
                                private juce::ValueTree::Listener
{
public:
    //==============================================================================
    LMP_MK1AudioProcessor();
    ~LMP_MK1AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset();
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState apvts { *this, nullptr, "Parameters",
createParameterLayout() };


private:
    void splitBufferByEvents(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void handleMIDI(uint8_t data0, uint8_t data1, uint8_t data2);
    void render(juce::AudioBuffer<float>& buffer, int sampleCount, int bufferOffset);

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override
    {
       parameterChanged.store(true);
    }

    Synth synth;
    juce::AudioParameterFloat* osc1LevelParam;
    std::atomic<bool> parameterChanged {false };

    void update();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LMP_MK1AudioProcessor)
};