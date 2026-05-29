#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils.h"

//==============================================================================
LMP_MK1AudioProcessor::LMP_MK1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    castParameter(apvts, ParameterID::osc1Level, osc1LevelParam);
    castParameter(apvts, ParameterID::osc2Level, osc2LevelParam);

    castParameter(apvts, ParameterID::decay, decayParam);
    castParameter(apvts, ParameterID::release, releaseParam);
    castParameter(apvts, ParameterID::attack, attackParam);
    castParameter(apvts, ParameterID::sustain, sustainParam);
    castParameter(apvts, ParameterID::polyphony, polyphonyParam);
    apvts.state.addListener(this);
}

LMP_MK1AudioProcessor::~LMP_MK1AudioProcessor()
{
    apvts.state.removeListener(this);

}

//==============================================================================
const juce::String LMP_MK1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LMP_MK1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LMP_MK1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LMP_MK1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LMP_MK1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LMP_MK1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LMP_MK1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void LMP_MK1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LMP_MK1AudioProcessor::getProgramName (int index)
{
    return {};
}

void LMP_MK1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LMP_MK1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.allocateResources(sampleRate, samplesPerBlock);
    parameterChanged.store(true);
    reset();
}

void LMP_MK1AudioProcessor::releaseResources()
{
    synth.deallocateResources();

}

void LMP_MK1AudioProcessor::reset()
{
    synth.reset();
}


#ifndef JucePlugin_PreferredChannelConfigurations
bool LMP_MK1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;



   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LMP_MK1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    bool expected = true;
    if (parameterChanged.compare_exchange_strong(expected, false)) {
        update();
    }


    splitBufferByEvents(buffer, midiMessages);

}

void LMP_MK1AudioProcessor::update ()
{
    float oscVol = osc1LevelParam->get();
    float osc2Vol = osc2LevelParam->get();
    synth.update(oscVol);
    synth.update2(osc2Vol);

    float sampleRate = float(getSampleRate());
    float inverseSampleRate = 1.0f / sampleRate;

    synth.envAttack = std::exp(-inverseSampleRate * std::exp(5.5f - 0.075f * attackParam->get()));
    synth.envDecay = std::exp(-inverseSampleRate * std::exp(5.5f - 0.075f * decayParam->get()));

    synth.envSustain = sustainParam->get() / 100.0f;

    float envRelease = releaseParam->get();
    if (envRelease < 1.0f) {
        synth.envRelease = 0.75f;  // extra fast release
    } else {
        synth.envRelease = std::exp(-inverseSampleRate * std::exp(5.5f - 0.075f * envRelease));
    }

    synth.numVoices = static_cast<int>(polyphonyParam->get());

}

void LMP_MK1AudioProcessor::splitBufferByEvents(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    int bufferOffset = 0;
    for (const auto metadata : midiMessages) {
        // Render the audio that happens before this event (if any).
        int samplesThisSegment = metadata.samplePosition - bufferOffset;
        if (samplesThisSegment > 0) {
            render(buffer, samplesThisSegment, bufferOffset);
            bufferOffset += samplesThisSegment;
        }
        // Handle the event. Ignore MIDI messages such as sysex.
        if (metadata.numBytes <= 3) {
            uint8_t data1 = (metadata.numBytes >= 2) ? metadata.data[1] : 0;
            uint8_t data2 = (metadata.numBytes == 3) ? metadata.data[2] : 0;
            handleMIDI(metadata.data[0], data1, data2);
        }
    }
    // Render the audio after the last MIDI event. If there were no
    // MIDI events at all, this renders the entire buffer.
    int samplesLastSegment = buffer.getNumSamples() - bufferOffset;
    if (samplesLastSegment > 0) {
        render(buffer, samplesLastSegment, bufferOffset);
    }
    midiMessages.clear();

}

void LMP_MK1AudioProcessor::handleMIDI(uint8_t data0, uint8_t data1, uint8_t data2)
{
    synth.midiMessage(data0, data1, data2);
}

void LMP_MK1AudioProcessor::render(
juce::AudioBuffer<float>& buffer, int sampleCount, int bufferOffset)
{
    float* outputBuffers[2] = { nullptr, nullptr };
    outputBuffers[0] = buffer.getWritePointer(0) + bufferOffset;
    if (getTotalNumOutputChannels() > 1) {
        outputBuffers[1] = buffer.getWritePointer(1) + bufferOffset;
    }
    synth.render(outputBuffers, sampleCount);
}


//==============================================================================
bool LMP_MK1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LMP_MK1AudioProcessor::createEditor()
{
    return new LMP_MK1AudioProcessorEditor(*this);

}

//==============================================================================
void LMP_MK1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void LMP_MK1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LMP_MK1AudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout
LMP_MK1AudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
    ParameterID::osc1Level,
    "Osc1 Level",
    juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
    100.0f,
    juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
    ParameterID::osc2Level,
    "Osc2 Level",
    juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
    100.0f,
    juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
    ParameterID::attack,
    "attack",
    juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
    0.0f,
    juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::decay,
        "decay",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        50.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::sustain,
        "sustain",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        100.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::release,
        "release",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        30.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
    ParameterID::polyphony,
    "polyphony",
    juce::NormalisableRange<float>(1.0f, 8.0f, 1.0f),
    1.0f,
    juce::AudioParameterFloatAttributes().withLabel("voices")));

    return layout;
}

