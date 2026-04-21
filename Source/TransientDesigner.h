/*
  ==============================================================================

    TransientDesigner.h
    Created: 18 Mar 2026 2:27:54pm
    Author:  Thomas Jungwirth

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class TransientDesignerProcessor{
    public:

        // Buffer processing
        void processBuffer(float* buffer, int c, int N);

        // Setup
        void prepareToPlay(float sampleRate);

        // Parameters
        void setAttack(float a);   // typical range: -1 to 1
        void setSustain(float s);  // typical range: -1 to 1

    private:

        // USER PARAMETERS
        float attack  = 0.f; // -1 to 1
        float sustain = 0.f; // -1 to 1

        // SAMPLE RATE
        float Fs = 44100.f;

        // ENVELOPE FILTER COEFFICIENTS (from your MATLAB)
        float gFast = 0.9991f;
        float gSlow = 0.9999f;

        // PER-CHANNEL STATE (like currentPhase in tremolo)
        float fbFast[2] = {0.f, 0.f};
        float fbSlow[2] = {0.f, 0.f};

    };
