//
// Created by logan on 5/27/2026.
//

#ifndef LMP_MK1_ENVELOPE_H
#define LMP_MK1_ENVELOPE_H

#pragma once
class Envelope
{
public:
    float nextValue()
    {
        level *= 0.9999f;
        return level;
    }
    float level;
};


#endif //LMP_MK1_ENVELOPE_H