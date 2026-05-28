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

    float level;


    float attackMultiplier;
    float decayMultiplier;
    float sustainLevel;
    float releaseMultiplier;

    void reset()
    {
        level = 0.0f;
        target = 0.0f;
        multiplier = 0.0f;
    }

    float nextValue()
    {
        level = multiplier * (level - target) + target;
        if (level + target > 3.0f) {
            multiplier = decayMultiplier;
            target = sustainLevel;
        }
        return level;
    }

    void release()
    {
        target = 0.0f;
        multiplier = releaseMultiplier;
    }

    inline bool isInAttack() const
    {
        return target >= 2.0f;
    }

    void attack()
    {
        level += SILENCE + SILENCE;
        target = 2.0f;
        multiplier = attackMultiplier;
    }


    inline bool isActive() const
    {
        return level > SILENCE;
    }

private:

    float target;
    float multiplier;

};


#endif //LMP_MK1_ENVELOPE_H