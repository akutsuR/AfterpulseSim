#include <iostream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "TMath.h"

#include "WCSimRootEvent.hh"
#include "WCSimRootGeom.hh"

#include "PMTAfterPulse.h"
#include "PMTDark.h"
#include "PMTSinglePE.h"


using std::cout;
using std::endl;
using std::string;
using std::vector;

const int NTREES=4;
enum ETREES{ eSetT=0,
             eGeoT,
             eHitT,
             eOptT
           };
const string STREES[ NTREES ]=
            {"Settings",
             "wcsimGeoT",
             "wcsimT",
             "wcsimRootOptionsT"
            };

const int N_FILE_TYPES=2;
enum E_FILE_TYPES{ eIN=0, eOUT };
TFile *fFile[N_FILE_TYPES];
TTree *fTree[N_FILE_TYPES][NTREES];
WCSimRootEvent *fWCEvent;

int             fNPMTs_ID;
vector<TVector3> fPosPMT_ID;
vector<TVector3> fDirPMT_ID;
vector<Int_t> fmPMT_ID;
vector<Int_t> fmPMTpmt_ID;

PMTAfterPulse *fAP=NULL;
PMTDark       *fDN=NULL;
PMTSinglePE   *fPE=NULL;
Int_t fMinTubeID;
Int_t fMaxTubeID;

string fInFileName="";
string fOutFileName="";

int fSeed;
bool fAddAP;
bool fAddDN;
bool fUseHZC;
float fDarkRate     =1.;        // kHz per 3inch PMT
float fDarkTWinLow  =-500.;     // ns
float fDarkTWinUp   =30500.;   // ns

void InitiPMTNoise(const int, const bool);
void LoadGeometryIDPMT(const string);
void SetInTree(const string);
void SetOutTree(const string);
void AddNoiseHits(const bool, const bool);
float GetFirstHitTime(const WCSimRootTrigger*);
void WriteOutputs(const string, const string);
bool ParseCmdArg(int, char**);
