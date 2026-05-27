//
// Created by logan on 5/27/2026.
//

#ifndef LMP_MK1_OSCILLATOR2_H
#define LMP_MK1_OSCILLATOR2_H
#pragma once

class Oscillator2
{
public:
    float amplitude;
    float inc;
    float phase;
    void reset()
    {
        phase = 0.0f;
    }
    float nextSample()
    {
        phase += inc;
        if (phase >= 1.0f) {
            phase -= 1.0f;
        }
        return amplitude * (2.0f * phase - 1.0f);
    }
};


#endif //LMP_MK1_OSCILLATOR2_H