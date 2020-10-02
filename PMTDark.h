#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>

#include "TRandom3.h"

using std::cout;
using std::endl;
using std::vector;

class PMTDark 
{
    public :
        PMTDark(const int);
        virtual ~PMTDark();

        void SetNumberOfPMTs(const int);
        void SetMinTubeID(const int);
        void SetMaxTubeID(const int);
        void SetWindowLow(const double /* ns */);
        void SetWindowUp(const double /* ns */);
        void SetDarkRate(const double /* kHz */);

        void GenerateDarkNoise(const bool verbose=false);
        int GetNumberOfGeneratedDarkHits() const;
        int GetNoiseTube(const int /* 0-fNnoise */) const;
        double GetNoiseTime(const int /* 0-fNnoise */) const;


    private :
        TRandom3 *fRand;
        int fNPMTs;
        int fMinTubeID;
        int fMaxTubeID;
        double fWinLow;
        double fWinUp;
        double fRdark;
        double fAveNoise;

        int fNnoise;
        vector<int> fNoisePMT;
        vector<double> fNoiseTime;
};
