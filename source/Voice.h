//
// Created by logan on 5/26/2026.
//

#ifndef LMP_MK1_VOICE_H
#define LMP_MK1_VOICE_H

#include "Envelope.h"
#include "Oscillator.h"
#include "Oscillator2.h"
struct Voice
{
    int note;
    int note2;
    Oscillator osc;
    Oscillator2 osc2;
    Envelope env;

    float level = 1.0f;
    float saw;
    void reset()
    {
        note = 0;
        saw = 0.0f;
        osc.reset(); // this is new
        osc2.reset();
    }
    // add this method
    float render()
    {
        return osc.nextSample() * level;
    }

    float render2()
    {
        float sample = osc2.nextSample();
        saw = saw * 0.997f - sample;
        return saw;
    }
};

#endif //LMP_MK1_VOICE_H