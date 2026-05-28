//
// Created by logan on 5/26/2026.
//

#ifndef LMP_MK1_SYNTH_H
#define LMP_MK1_SYNTH_H

#include "Voice.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include "NoiseGenerator.h"

class Synth
{
public:
    Synth();
    void allocateResources(double sampleRate, int samplesPerBlock);
    void deallocateResources();
    void reset();
    void render(float** outputBuffers, int sampleCount);
    void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);
    void update(float volumeParam);
    void update2(float volumeParam2);
    juce::LinearSmoothedValue<float> outputLevelSmoother;
    juce::LinearSmoothedValue<float> output2LevelSmoother;
    float envDecay;
    float envAttack;
    float envSustain;
    float envRelease;
private:

    void noteOn(int note, int velocity);
    void noteOff(int note);

    float sampleRate;

    Voice voice;public:
    NoiseGenerator noiseGen;

};


#endif //LMP_MK1_SYNTH_H