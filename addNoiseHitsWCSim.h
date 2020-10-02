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

void InitiPMTNoise(const int, const bool);
void LoadGeometryIDPMT(const string);
void SetInTree(const string);
void SetOutTree(const string);
void AddNoiseHits(const bool, const bool);
void WriteOutputs(const string, const string);

