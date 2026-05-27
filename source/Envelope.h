//
// Created by logan on 5/27/2026.
//

#ifndef LMP_MK1_ENVELOPE_H
#define LMP_MK1_ENVELOPE_H

#pragma once

const float SILENCE = 0.0001f;

class Envelope
{
public:
    float nextValue()
    {
        level *= multiplier;
        return level;
    }
    float level;
    float multiplier;
};


#endif //LMP_MK1_ENVELOPE_H