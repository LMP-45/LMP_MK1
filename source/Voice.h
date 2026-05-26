//
// Created by logan on 5/26/2026.
//

#ifndef LMP_MK1_VOICE_H
#define LMP_MK1_VOICE_H

#include "Oscillator.h"
struct Voice
{
    int note;
    Oscillator osc; // this is new

    float level = 1.0f;
    void reset()
    {
        note = 0;
        osc.reset(); // this is new
    }
    // add this method
    float render()
    {
        return osc.nextSample() * level;
    }
};

#endif //LMP_MK1_VOICE_H