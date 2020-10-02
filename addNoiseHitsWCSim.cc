#include "addNoiseHitsWCSim.h"

void AddNoiseHits(const bool addAP, const bool addDN)
{
    int nEntries=fTree[eIN][eHitT]->GetEntries();
    cout<<" Start processing " << nEntries <<" events.... " <<endl;

    vector<int> photon_ids(1, -1); // Dummy
    WCSimRootTrigger* anEvent=NULL;
    WCSimRootCherenkovDigiHit *aHit=NULL;
    int nDigiHits=0;
    int nAP=0;
    int nDN=0;

    Float_t t_afterpulse=0.;
    Float_t q_afterpulse=0.;

    Float_t t_darknoise=0.;
    Float_t q_darknoise=0.;
    Int_t tube=-999999;
    Int_t mPMT_id=-99999;
    Int_t mPMT_PMTid=-999999;

    bool verbose=true;
    for(int iEntry=0; iEntry<nEntries; iEntry++)
    {
        fTree[eIN][eHitT]->GetEntry( iEntry );
        anEvent=fWCEvent->GetTrigger(0);

        nDigiHits=anEvent->GetNcherenkovdigihits();
        if( nDigiHits==0 )
        {
            cout<<" No DAQ trigger is issued -> SKIP THiS EVENT " <<endl;
            continue;
        }

        nAP=0;
        nDN=0;

        // afterpulse
        if( addAP )
        {
            float t_first=GetFirstHitTime(anEvent);
            for(int j=0; j<nDigiHits; j++)
            {
                aHit=dynamic_cast<WCSimRootCherenkovDigiHit*>( anEvent->GetCherenkovDigiHits()->At(j) );
                if( fAP->GenerateAfterPulse( aHit->GetQ() ) )
                {
                    q_afterpulse=fPE->GetDigitizedSPE();
                    t_afterpulse=aHit->GetT() + fAP->GetAfterPulseTiming()*1000.; // mus -> ns
                    anEvent->AddCherenkovDigiHit(q_afterpulse,
                                                 t_afterpulse+t_first,
                                                 aHit->GetTubeId(),
                                                 aHit->GetmPMTId(),
                                                 aHit->GetmPMT_PMTId(),
                                                 photon_ids);
                    nAP+=1;
                }
            };
        }

        // dark noise
        if( addDN )
        {
            fDN->GenerateDarkNoise( verbose );
            if( verbose ){ verbose=false; }

            int nDarkHits=fDN->GetNumberOfGeneratedDarkHits();
            for(int j=0; j<nDarkHits; j++)
            {
                tube        =fDN->GetNoiseTube(j);
                q_darknoise =fPE->GetDigitizedSPE();
                t_darknoise =fDN->GetNoiseTime(j);
                mPMT_id     =fmPMT_ID[tube-1];
                mPMT_PMTid  =fmPMTpmt_ID[tube-1];
                anEvent->AddCherenkovDigiHit(q_darknoise,
                                             t_darknoise,
                                             tube,
                                             mPMT_id,
                                             mPMT_PMTid,
                                             photon_ids);
                nDN+=1;
            }
        }

        if( iEntry%10==0 )
        {
                std::cout<<" Entry:" << iEntry
                         <<" - nDigiHits(Before):" << nDigiHits
                         <<" - nAP:" << nAP
                         <<" - nDN:" << nDN
                         <<" -- nDigiHits(After):" << anEvent->GetNcherenkovdigihits()
                         <<std::endl;
        }

        fFile[eOUT]->cd();
        fTree[eOUT][eHitT]->Fill();

        fWCEvent->ReInitialize();
    }
}


float GetFirstHitTime(const WCSimRootTrigger *anEvent)
{
    float tmin=9999999999999.;
    WCSimRootCherenkovDigiHit *aDigi=NULL;
    for(int iHit=0; iHit<anEvent->GetNcherenkovdigihits(); iHit++)
    {
        aDigi=dynamic_cast<WCSimRootCherenkovDigiHit*>( anEvent->GetCherenkovDigiHits()->At(iHit) );
        if( aDigi->GetT()<tmin ){ tmin=aDigi->GetT(); }
    }

    return tmin;
}



void InitiPMTNoise(const int seed, const bool use_hzc)
{
    fAP=new PMTAfterPulse();
    fAP->SetRandomSeed( seed );
    fAP->Initialize();
    if( use_hzc )
    {
        // Use HZC afterpulse rate instead of Hamamatsu's one
        fAP->SetHZCRate();
    }
               
    fDN=new PMTDark( seed+1 );
    fDN->SetMinTubeID( fMinTubeID );
    fDN->SetMaxTubeID( fMaxTubeID );
    fDN->SetWindowLow( fDarkTWinLow );
    fDN->SetWindowUp( fDarkTWinUp );
    fDN->SetDarkRate( fDarkRate );

    fPE=new PMTSinglePE( seed-1 );
}


void SetInTree(const string infile)
{
    fFile[eIN]=TFile::Open( infile.c_str() );
    fTree[eIN][eHitT]=(TTree*)fFile[eIN]->Get( STREES[eHitT].c_str() );

    fWCEvent=new WCSimRootEvent();
    fTree[eIN][eHitT]->SetBranchAddress("wcsimrootevent", &fWCEvent);
}


void SetOutTree(const string outfile)
{
    fFile[eOUT]=TFile::Open( outfile.c_str(), "recreate" );
    fTree[eOUT][eHitT]=fTree[eIN][eHitT]->CloneTree(0);
    fTree[eOUT][eHitT]->SetBranchAddress("wcsimrootevent", &fWCEvent);
}


void WriteOutputs(const string infile, const string outfile)
{
    fFile[eIN]->cd();
    for(int i=0; i<NTREES; i++)
    {
        if( i!=eHitT )
        {
            fTree[eIN][i]=(TTree*)fFile[eIN]->Get( STREES[i].c_str() );
        }
    }

    ///////////////////////////////////////
    Float_t WCXRotation[3]={0.};
    Float_t WCYRotation[3]={0.};
    Float_t WCZRotation[3]={0.};
    Float_t WCDetCentre[3]={0.};
    Float_t WCDetRadius=0;
    Float_t WCDetHeight=0.;
    fTree[eIN][eSetT]->SetBranchAddress("WCXRotation", WCXRotation);
    fTree[eIN][eSetT]->SetBranchAddress("WCYRotation", WCYRotation);
    fTree[eIN][eSetT]->SetBranchAddress("WCZRotation", WCZRotation);
    fTree[eIN][eSetT]->SetBranchAddress("WCDetCentre", WCDetCentre);
    fTree[eIN][eSetT]->SetBranchAddress("WCDetRadius", &WCDetRadius);
    fTree[eIN][eSetT]->SetBranchAddress("WCDetHeight", &WCDetHeight);
    fTree[eIN][eSetT]->GetEntry(0);

    fFile[eOUT]->cd();
    fTree[eOUT][eSetT]=new TTree(STREES[eSetT].c_str(), "");
    fTree[eOUT][eSetT]->Branch("WCXRotation", WCXRotation,  "WCXRotation[3]/F");
    fTree[eOUT][eSetT]->Branch("WCYRotation", WCYRotation,  "WCYRotation[3]/F");
    fTree[eOUT][eSetT]->Branch("WCZRotation", WCZRotation,  "WCZRotation[3]/F");
    fTree[eOUT][eSetT]->Branch("WCDetCentre", WCDetCentre,  "WCDetCentre[3]/F");
    fTree[eOUT][eSetT]->Branch("WCDetRadius", &WCDetRadius, "WCDetRadius/F");
    fTree[eOUT][eSetT]->Branch("WCDetHeight", &WCDetHeight, "WCDetHeight/F");
    fTree[eOUT][eSetT]->Fill();
    //////////////////////////////////////
    fTree[eOUT][eGeoT]=fTree[eIN][eGeoT]->CloneTree();
    fTree[eOUT][eOptT]=fTree[eIN][eOptT]->CloneTree();

    for(int i=0; i<NTREES; i++)
    {
        fTree[eOUT][i]->Write();
    }

    fFile[eOUT]->Close();
    fFile[eIN]->cd();
    fFile[eIN]->Close();
}


void LoadGeometryIDPMT(const string infile)
{
    TFile *f=TFile::Open(infile.c_str());
    TTree *t=(TTree*)f->Get("wcsimGeoT");
    WCSimRootGeom *geo=new WCSimRootGeom();
    t->SetBranchAddress("wcsimrootgeom",    &geo);
    t->GetEntry( 0 );

    WCSimRootPMT aPMT;
    fNPMTs_ID=geo->GetWCNumPMT();

    fPosPMT_ID.reserve( fNPMTs_ID );
    fDirPMT_ID.reserve( fNPMTs_ID );
    fmPMT_ID.reserve( fNPMTs_ID );
    fmPMTpmt_ID.reserve( fNPMTs_ID );

    Double_t x, y, z;
    Int_t iTubeID=0;
    for(int i=0; i<fNPMTs_ID; i++)
    {
        aPMT=geo->GetPMT( i );
        iTubeID=aPMT.GetTubeNo();

        fMinTubeID=iTubeID<fMinTubeID ? iTubeID : fMinTubeID;
        fMaxTubeID=iTubeID>fMaxTubeID ? iTubeID : fMaxTubeID;

        x=aPMT.GetPosition(0);
        y=aPMT.GetPosition(1);
        z=aPMT.GetPosition(2);
        fPosPMT_ID.push_back( TVector3(x,y,z) );

        x=aPMT.GetOrientation(0);
        y=aPMT.GetOrientation(1);
        z=aPMT.GetOrientation(2);
        fDirPMT_ID.push_back( TVector3(x,y,z) );

        fmPMT_ID.push_back( aPMT.GetmPMTNo() );
        fmPMTpmt_ID.push_back(aPMT.GetmPMT_PMTNo());
    }
    f->Close();
    if( !t ){ delete t; t=NULL; }
    if( !geo ){ delete geo; geo=NULL; }
    
    cout<<" Loaded Geometry information " <<endl;
}

bool ParseCmdArg(int argc, char **argv)
{
    cout<<" Parsing command line arguments..... " <<endl;
	for(int i=1; i<argc; i++)
	{
		cout<<"     - argv[" << i <<"] :" <<argv[i] <<endl;
	}
	cout<<endl;


	for(int i=1; i<argc; i++)
	{
		if( string(argv[i])=="-i" )			            { fInFileName   =TString( argv[i+1] );	i++;}
		else if( string(argv[i])=="-o" )		        { fOutFileName	=TString( argv[i+1] );	i++;}
		else if( string(argv[i])=="-s" )		        { fSeed	        =atoi( argv[i+1] );	    i++;}
		else if( string(argv[i])=="-a" )		        { fAddAP	    =atoi( argv[i+1] );	    i++;}
		else if( string(argv[i])=="-d" )		        { fAddDN	    =atoi( argv[i+1] );	    i++;}
		else if( string(argv[i])=="--UseHZC" )		    { fUseHZC       =atoi( argv[i+1] );	    i++;}
        else if( string(argv[i])=="--DarkTWinLow")      { fDarkTWinLow  =atof( argv[i+1] );     i++;}
        else if( string(argv[i])=="--DarkTWinUp")       { fDarkTWinUp   =atof( argv[i+1] );     i++;}
        else if( string(argv[i])=="--DarkRate")         { fDarkRate     =atof( argv[i+1] );     i++;}
		else
		{
			return false;
		}
	}
	return true;
}


//////////////////////////////////////////////
int main(int argc, char **argv)
{
    bool CorrectCmdArgs=ParseCmdArg(argc, argv);
    if( !CorrectCmdArgs )
    {
        std::cout<<" Wrong input command line arguments " <<std::endl;
        std::cout<<" Please check the arguments..... "    <<std::endl;
        return -1;
    }

    LoadGeometryIDPMT(fInFileName); // Order is important
    InitiPMTNoise(fSeed, fUseHZC);
    SetInTree(fInFileName);
    SetOutTree(fOutFileName);
    AddNoiseHits(fAddAP, fAddDN);
    WriteOutputs(fInFileName, fOutFileName);
    
return 0;
}
