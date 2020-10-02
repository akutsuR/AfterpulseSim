#include <iostream>
#include <string>

#include "TRandom3.h"

// Generate single p.e of R12199_02 3'' PMT for mPMT
// Taken from G4float* PMT3inchR12199_02::Getqpe() in WCSimPMTObject.cc

class PMTSinglePE
{
    public:
        PMTSinglePE(const int);
        virtual ~PMTSinglePE();
        double GetDigitizedSPE();
        double GetRawSPE();

    private:
        TRandom3 *fRand;
        static const float fqpe0[501];
        double fResolutionPE;

        void ApplyThreshold(double&, bool&);
        double DoTruncate(const double, const double);
};
