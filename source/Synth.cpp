//
// Created by logan on 5/26/2026.
//

#include "Synth.h"
#include "Utils.h"

Synth::Synth()
{
    sampleRate = 44100.0f;
    lfo = 0.0f;
    lfoValue = 0.0f;
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
    for (auto& voice : voices)
    {
        voice.reset();
    }
    noiseGen.reset();

    lfo = 0.0f;
    lfoValue = 0.0f;
    lfoStep = 0;
}
void Synth::render(float** outputBuffers, int sampleCount)
{
    float* outputBufferLeft = outputBuffers[0];
    float* outputBufferRight = outputBuffers[1];

    for (int sample = 0; sample < sampleCount; ++sample)
    {
        updateLFO();
        float mix = 0.0f;

        // advance smoothers ONCE per sample
        float osc1Gain = outputLevelSmoother.getNextValue();
        float osc2Gain = output2LevelSmoother.getNextValue();

        for (int v = 0; v < numVoices; ++v)
        {
            auto& voice = voices[v];

            if (voice.env.isActive())
            {
                float env = voice.env.nextValue();

                float output1 = voice.render(env) * osc1Gain;
                float output2 = voice.render2(env) * osc2Gain;

                mix += (output1 + output2) * 0.125f;
            }
        }

        outputBufferLeft[sample] = mix;

        if (outputBufferRight != nullptr)
            outputBufferRight[sample] = mix;
    }

    protectYourEars(outputBufferLeft, sampleCount);

    if (outputBufferRight != nullptr)
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
    Voice* freeVoice = nullptr;

    // find inactive voice
    for (int v = 0; v < numVoices; ++v)
    {
        if (!voices[v].env.isActive())
        {
            freeVoice = &voices[v];
            break;
        }
    }

    // if all voices busy, steal first voice
    if (freeVoice == nullptr)
    {
        freeVoice = &voices[0];
    }

    auto& voice = *freeVoice;



    voice.note = note;

    float freq = 440.0f * std::exp2(float(note - 69) / 12.0f);

    voice.osc.amplitude = velocity / 127.0f;
    voice.osc.inc = freq / sampleRate;
    //voice.osc.modulation = 1.0f;

    voice.osc2.amplitude = (velocity / 127.0f) * 0.5f;
    voice.osc2.period = sampleRate / freq;
    //voice.osc2.modulation = 1.0f;

    voice.baseInc = freq / sampleRate;
    voice.osc.inc = voice.baseInc;

    voice.basePeriod = sampleRate / freq;
    voice.osc2.period = voice.basePeriod;

    Envelope& env = voice.env;

    env.attackMultiplier = envAttack;
    env.decayMultiplier = envDecay;
    env.sustainLevel = envSustain;
    env.releaseMultiplier = envRelease;

    env.attack();

}

void Synth::noteOff(int note)
{
    for (int v = 0; v < numVoices; ++v)
    {
        auto& voice = voices[v];

        if (voice.note == note && voice.env.isActive())
        {
            voice.release();
        }
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

void Synth::updateLFO()
{
    if (--lfoStep <= 0)
    {
        lfoStep = LFO_MAX;

        lfo += lfoInc;

        if (lfo > PI)
            lfo -= TWO_PI;

        switch (lfoWaveform)
        {
        case 0: // Sine
            lfoValue = std::sin(lfo);
            break;

        case 1: // Triangle
            {
                float phase = (lfo + PI) / TWO_PI; // 0..1
                lfoValue = 1.0f - 4.0f * std::abs(phase - 0.5f);
                break;
            }

        case 2: // Saw
            {
                float phase = (lfo + PI) / TWO_PI; // 0..1
                lfoValue = 2.0f * phase - 1.0f;
                break;
            }

        case 3: // Square
            {
                lfoValue = (std::sin(lfo) >= 0.0f) ? 1.0f : -1.0f;
                break;
            }
        }

        float vibratoMod = 1.0f + lfoValue * lfoDepth * 0.05f;



        for (int v = 0; v < numVoices; ++v)
        {
            Voice& voice = voices[v];

            if (voice.env.isActive())
            {
                voice.osc.setInc(voice.baseInc * vibratoMod);

                voice.osc2.period = voice.basePeriod / vibratoMod;
            }
        }
    }
}
