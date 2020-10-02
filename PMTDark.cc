#include "PMTDark.h"

PMTDark::PMTDark(const int seed) :
fNPMTs( 15808 ), // IWCD long tank geometry
fMinTubeID( 1 ),
fMaxTubeID( 15808 ),
fWinLow( -400. ),
fWinUp( 20000. ),
fRdark( 1. ),
fNnoise( 0 )
{
    fRand=new TRandom3(seed);
    fNoisePMT.clear();
    fNoiseTime.clear();
}


PMTDark::~PMTDark()
{
    if( !fRand ){ delete fRand; fRand=NULL;}
}


void PMTDark::SetNumberOfPMTs(const int npmts)
{
   fNPMTs=npmts;
}

void PMTDark::SetMinTubeID(const int tubeID)
{
   fMinTubeID=tubeID;
}

void PMTDark::SetMaxTubeID(const int tubeID)
{
   fMaxTubeID=tubeID;
}


void PMTDark::SetWindowLow(const double wlow)
{
    fWinLow=wlow;
}


void PMTDark::SetWindowUp(const double wup)
{
    fWinUp=wup;
}


void PMTDark::SetDarkRate(const double rate)
{
    fRdark=rate;     
}

void PMTDark::GenerateDarkNoise(const bool verbose)
{
    double windowSize=fWinUp - fWinLow;
    fAveNoise=(double)fNPMTs*fRdark*windowSize;
    // fRdark and windowSize are given in kHz and ns, respectively
    fAveNoise/=1000000.;

    fNoisePMT.clear();
    fNoiseTime.clear();

    fNnoise=fRand->Poisson( fAveNoise );
    fNoisePMT.reserve( fNnoise );
    fNoiseTime.reserve( fNnoise );
    Int_t iTubeID=0;
    for(int i=0; i<fNnoise; i++)
    {
        iTubeID=fMinTubeID + fRand->Integer( fNPMTs );
        if( iTubeID>fMaxTubeID )
        {
            cout<<" [ERROR] PMTDark::GenerateDarkNoise " <<endl;
            cout<<"   - Generated tube ID:" << iTubeID <<endl;
            cout<<"   - EXceeding max tube ID:" << fMaxTubeID <<endl;
            cout<<" -> EXIT " <<endl;
            exit(-1);
        }
        fNoisePMT[i]=iTubeID;
        fNoiseTime[i]=fRand->Uniform(fWinLow, fWinUp);
    } 

    if( verbose )
    {
        cout<<" [INFO] PMTDark::GenerateDarkNoise " <<endl;
        cout<<"   - Window Low:" << fWinLow <<" (ns)" <<endl;
        cout<<"   - Window Up:" << fWinUp <<" (ns)" <<endl;
        cout<<"   - # PMTs:" << fNPMTs <<endl;
        cout<<"   - Dark rate:" << fRdark <<" (kHz)" <<endl;
        cout<<"   - Mean # noise hits:" << fAveNoise <<endl;
        cout<<"   - # generated noise hits:" << fNnoise <<endl; 
    }
}


int PMTDark::GetNumberOfGeneratedDarkHits() const
{
    return fNnoise;
}


int PMTDark::GetNoiseTube(const int inoise) const
{
    if( inoise<0 || inoise>=fNnoise )
    {
        cout<<" [ERROR] PMTDark::GetNoiseTube " <<endl;
        cout<<"   - Out of rage in the current generate noise" <<endl;
        cout<<" -> EXIT " <<endl;
        exit( -1 );
    }
    return fNoisePMT[inoise];
}


double PMTDark::GetNoiseTime(const int inoise) const
{
    if( inoise<0 || inoise>=fNnoise )
    {
        cout<<" [ERROR] PMTDark::GetNoiseTime " <<endl;
        cout<<"   - Out of rage in the current generate noise" <<endl;
        cout<<" -> EXIT " <<endl;
        exit( -1 );
    }
    return fNoiseTime[inoise];
}
