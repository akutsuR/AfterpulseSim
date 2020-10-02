#include "PMTAfterPulse.h"

PMTAfterPulse::PMTAfterPulse()
{
    fFileRateVsCharge   ="/disk01/usr5/rakutsu/shared/afterpulse/iwcd_3inch_afterpulse_fittedrate.root";
    fFileTiming         ="/disk01/usr5/rakutsu/shared/afterpulse/iwcd_3inch_afterpulse_fitteddt.root"; 
    fNameRateVsCharge   ="fitted_pe_rate";
    fNameTiming         ="ftgauss";

    fRandSeed           =456789;

    fRateVsCharge       =NULL;
    fTiming             =NULL;
    fRand               =NULL;
}

PMTAfterPulse::~PMTAfterPulse()
{ 
    if( !fRateVsCharge ){ delete fRateVsCharge; }
    if( !fTiming )      { delete fTiming; } 
    if( !fRand )        { delete fRand; }
}

void PMTAfterPulse::SetRandomSeed(const int iseed)
{
    fRandSeed=iseed;
}

void PMTAfterPulse::Initialize()
{
    // Initialize gRandom
    fRand=new TRandom3( fRandSeed );
    gRandom->SetSeed( fRand->Integer( 10000 ) );
    std::cout<<" [INFO] PMTAfterPulse::Initialize " <<std::endl;
    std::cout<<"   - Initialized TRandom3 with " << fRandSeed <<std::endl;

    TFile *f=NULL;
    // Load afterpulse rate vs first hit's charge (p.e.)
    f=TFile::Open( fFileRateVsCharge.c_str() );
    if( !f->IsOpen() )
    {
        std::cout<<" [ERROR] PMTAfterPulse::Initialize " <<std::endl;
        std::cout<<"   - Cannot open file:" << fFileRateVsCharge <<std::endl;
        exit( -1 );
    }
    else
    {
        std::cout<<"   - Loaded RateVsCharge:" << fFileRateVsCharge <<std::endl;
    }
    fRateVsCharge=(TF1*)f->Get( fNameRateVsCharge.c_str() );
    f->Close();


    // Load afterpulse timing (mu s)
    f=TFile::Open( fFileTiming.c_str() );
    if( !f->IsOpen() )
    {
        std::cout<<" [ERROR] PMTAfterPulse::Initialize " <<std::endl;
        std::cout<<"   - Cannot open file:" << fFileTiming <<std::endl;
        exit( -1 );
    }
    else
    {
        std::cout<<"   - Loaded Timing:" << fFileTiming <<std::endl;
    }
    fTiming=(TF1*)f->Get( fNameTiming.c_str() );
    f->Close();
    delete f;
}

bool PMTAfterPulse::GenerateAfterPulse(const float pe)
{
// pe: charge of first hit in p.e.
// Output:
//  - true: afterpulse is generated
//  - false: afterpulse is not generated

    bool isAP=false;
    double rate=fRateVsCharge->Eval( pe );
    if( rate>1. )
    { 
        isAP=true;
    }
    else
    {
        if( fRand->Uniform()<rate )
        {   
            isAP=true;
        }
    }
    //std::cout<<" pe:" << pe 
    //         << " - rate:" << rate 
    //         << " - isAP:" << isAP
    //         <<std::endl;
    return isAP;
}

float PMTAfterPulse::GetAfterPulseTiming()
{
    return fTiming->GetRandom(); 
}

void PMTAfterPulse::SetHZCRate()
{
    fFileRateVsCharge   ="/disk01/usr5/rakutsu/shared/afterpulse/iwcd_3inch_afterpulse_fittedrate_hzc.root";
    fNameRateVsCharge   ="fitted_pe_rate_hzc";
    TFile *f=NULL;
    // Load afterpulse rate vs first hit's charge (p.e.)
    f=TFile::Open( fFileRateVsCharge.c_str() );
    if( !f->IsOpen() )
    {
        std::cout<<" [ERROR] PMTAfterPulse::Initialize " <<std::endl;
        std::cout<<"   - Cannot open file:" << fFileRateVsCharge <<std::endl;
        exit( -1 );
    }
    else
    {
        std::cout<<"   - Loaded RateVsCharge:" << fFileRateVsCharge <<std::endl;
    }
    fRateVsCharge=(TF1*)f->Get( fNameRateVsCharge.c_str() );
    f->Close();

    std::cout<<" [INFO] PMTAterPulse::SetHZCRate "          <<std::endl;
    std::cout<<"   - HZC's afterpulse rate will be used"    <<std::endl;
}
