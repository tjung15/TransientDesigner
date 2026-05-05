/*
  ==============================================================================

    TransientDesigner.cpp
    Created: 18 Mar 2026 2:27:54pm
    Author:  Thomas Jungwirth

  ==============================================================================
*/

#include "TransientDesigner.h"

void TransientDesignerProcessor::processBuffer(float* buffer, int c, int N)
{
    const float kFast = (1.f - gFast) * 2.f;
    const float kSlow = (1.f - gSlow) * 3.f;

    float fast = fbFast[c];
    float slow = fbSlow[c];

    for (int n = 0; n < N; ++n)
    {
        float x    = buffer[n];
        float absX = std::fabs(x);

        fast = kFast * absX + gFast * fast;
        slow = kSlow * absX + gSlow * slow;

        float diff = fast - slow;
        float gain = 1.f + 8.f * (attack * std::max(diff, 0.f) - sustain * std::min(diff, 0.f));

        buffer[n] = x * gain;
    }

    fbFast[c] = fast;
    fbSlow[c] = slow;
}

void TransientDesignerProcessor::prepareToPlay(float sampleRate)
{
    Fs = sampleRate;

    // ~10ms attack envelope, ~100ms sustain envelope
    gFast = std::exp(-1.f / (0.010f * sampleRate));
    gSlow = std::exp(-1.f / (0.100f * sampleRate));

    fbFast[0] = fbFast[1] = 0.f;
    fbSlow[0] = fbSlow[1] = 0.f;
}

void TransientDesignerProcessor::setAttack(float a)
{
    attack = a;
}

void TransientDesignerProcessor::setSustain(float s)
{
    sustain = s;
}
