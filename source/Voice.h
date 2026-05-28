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
        env.reset();
    }
    // add this method
    float render()
    {
        float sample = osc.nextSample() * level;
        float envelope = env.nextValue();
        return sample * envelope;

    }

    float render2()
    {
        float sample = osc2.nextSample();
        saw = saw * 0.997f + sample;
        // these lines are new
        float envelope = env.nextValue();
        return saw * envelope;
    }

    void release()
    {
        env.release();
    }



};

#endif //LMP_MK1_VOICE_H