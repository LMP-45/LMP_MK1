//
// Created by logan on 5/26/2026.
//

#ifndef LMP_MK1_VOICE_H
#define LMP_MK1_VOICE_H

#include "Oscillator.h"
#include "Oscillator2.h"
struct Voice
{
    int note;
    int note2;
    Oscillator osc; // this is new
    Oscillator2 osc2;

    float level = 1.0f;
    void reset()
    {
        note = 0;

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
        return osc2.nextSample() * level;
    }
};

#endif //LMP_MK1_VOICE_H