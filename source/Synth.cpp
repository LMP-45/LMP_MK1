//
// Created by logan on 5/26/2026.
//

#include "Synth.h"
#include "Utils.h"

Synth::Synth()
{
    sampleRate = 44100.0f;
}
void Synth::allocateResources(double sampleRate_, int /*samplesPerBlock*/)
{
    sampleRate = static_cast<float>(sampleRate_);

    outputLevelSmoother.reset(sampleRate, 0.05f); // 50 ms smoothing
    outputLevelSmoother.setCurrentAndTargetValue(1.0f);

    output2LevelSmoother.reset(sampleRate, 0.05f);
    output2LevelSmoother.setCurrentAndTargetValue(1.0f);
}

void Synth::deallocateResources()
{
    // do nothing
}
void Synth::reset()
{
    voice.reset();
    noiseGen.reset();
}
void Synth::render(float** outputBuffers, int sampleCount)
{
    float* outputBufferLeft = outputBuffers[0];
    float* outputBufferRight = outputBuffers[1];

    for (int sample = 0; sample < sampleCount; ++sample)
    {
        float output = 0.0f;
        float output2 = 0.0f;

        if (voice.note > 0)
        {
            output = voice.render();
            output2 = voice.render2();
            // smooth volume
            output *= outputLevelSmoother.getNextValue();
            output2 *= output2LevelSmoother.getNextValue();

        }

        outputBufferLeft[sample] = output + output2;

        if (outputBufferRight != nullptr)
            outputBufferRight[sample] = output + output2;
    }

    protectYourEars(outputBufferLeft, sampleCount);
    protectYourEars(outputBufferRight, sampleCount);
}
void Synth::midiMessage(uint8_t data0, uint8_t data1, uint8_t data2)
{
    switch (data0 & 0xF0) {
        // Note off
    case 0x80:
        noteOff(data1 & 0x7F);
        break;
        // Note on
    case 0x90: {
            uint8_t note = data1 & 0x7F;
            uint8_t velo = data2 & 0x7F;
            if (velo > 0) {
                noteOn(note, velo);
            } else {
                noteOff(note);
            }
            break;
    }
    }
}

void Synth::noteOn(int note, int velocity)
{

    voice.note = note;
    float freq = 440.0f * std::exp2(float(note - 69) / 12.0f); // this changed
    voice.osc.amplitude = velocity / 127.0f;
    voice.osc.inc = freq / sampleRate;
    voice.osc.reset();


   float freq2 = 440.0f * std::exp2(float(note - 69) / 12.0f); // this changed
    voice.osc2.amplitude = (velocity / 127.0f) * 0.5f;
    voice.osc2.period = sampleRate / freq2;
    voice.osc2.reset();
}

void Synth::noteOff(int note)
{
    if (voice.note == note) {
        voice.note = 0;

    }
}

void Synth::update(float volumeParam)
{
    float volume = volumeParam / 100.0f;

    // optional curve
    volume *= volume;

    outputLevelSmoother.setTargetValue(volume);
}

void Synth::update2(float volumeParam2)
{
    float volume2 = volumeParam2 / 100.0f;

    // optional curve
    volume2 *= volume2;

    output2LevelSmoother.setTargetValue(volume2);
}
