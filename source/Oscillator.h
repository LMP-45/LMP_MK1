//
// Created by logan on 5/26/2026.
//

#ifndef LMP_MK1_OSCILLATOR_H
#define LMP_MK1_OSCILLATOR_H

#include <cmath>

const float TWO_PI = 6.2831853071795864f;

class Oscillator
{
public:
    float amplitude = 1.0f;
    float inc = 0.0f;
    float phase = 0.0f;

    void setInc(float newInc)
    {
        inc = newInc;
        dsin = 2.0f * std::cos(inc * TWO_PI);
    }

    void reset()
    {
        phase = 0.0f;

        sin0 = amplitude * std::sin(phase * TWO_PI);
        sin1 = amplitude * std::sin((phase - inc) * TWO_PI);

        dsin = 2.0f * std::cos(inc * TWO_PI);
    }

    float nextSample()
    {
        float sinx = dsin * sin0 - sin1;

        sin1 = sin0;
        sin0 = sinx;

        return sinx;
    }

private:
    float sin0;
    float sin1;
    float dsin;
};


#endif //LMP_MK1_OSCILLATOR_H