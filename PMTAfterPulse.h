#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

#include "TROOT.h"
#include "TFile.h"
#include "TF1.h"
#include "TRandom3.h"

class PMTAfterPulse
{
    public:
        PMTAfterPulse();
        virtual ~PMTAfterPulse();

        void SetRandomSeed(const int);
        void SetHZCRate();
        void Initialize();
        bool GenerateAfterPulse(const float);
        float GetAfterPulseTiming();
        
    private:  
        TF1 *fRateVsCharge;
        TF1 *fTiming;
        TRandom3* fRand;

        std::string fFileRateVsCharge;
        std::string fFileTiming;
        std::string fNameRateVsCharge;
        std::string fNameTiming;

        int         fRandSeed;
};
