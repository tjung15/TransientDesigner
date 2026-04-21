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
    for (int n = 0; n < N; ++n)
    {
        buffer[n] = processSample(buffer[n], c);
    }
}

float TransientDesignerProcessor::processSample(float x, int c)
{
    float absX = std::fabs(x);

    float envFast = (1.f - gFast) * 2.f * absX + gFast * fbFast[c];
    fbFast[c] = envFast;

    float envSlow = (1.f - gSlow) * 3.f * absX + gSlow * fbSlow[c];
    fbSlow[c] = envSlow;

    float diff = envFast - envSlow;

    float gain = 1.f + attack * std::max(diff, 0.f) - sustain * std::min(diff, 0.f);

    return x * gain;
}

void TransientDesignerProcessor::prepareToPlay(float sampleRate)
{
    Fs = sampleRate;

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
