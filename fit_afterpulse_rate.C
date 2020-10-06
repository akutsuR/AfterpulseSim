#include <vector>

void fit_afterpulse_rate()
{
	std::vector<Float_t> pe;
	std::vector<Float_t> rate;
	const Int_t NPMTS=4;
	const std::string TreeNames[NPMTS]
			={"fraction_hpk_1_c",
			  "fraction_hpk_2",
			  "fraction_hpk_a",
			  "fraction_hpk_b"
			 };

	Float_t Q;
	Float_t R;
	string sQ="charge";
	string sR="";	
	TFile *fin=TFile::Open("ap_fraction.root");
		for(Int_t i=0; i<NPMTS; i++)
		{
			TTree *t=(TTree*)fin->Get( TreeNames[i].c_str() );
			if( i==0 ){ sR="fraction"; }
			else	  { sR="fraction_c"; }
			t->SetBranchAddress(sQ.c_str(), &Q);
			t->SetBranchAddress(sR.c_str(), &R);

			cout<<" i:" << i <<" - " << t->GetEntries() <<endl;
			for(Int_t j=0; j<t->GetEntries(); j++)
			{
				t->GetEntry( j );
				pe.push_back( Q );
				rate.push_back( R );
				cout<<"     -- j:" << j <<" - Q:" << Q << " - R:" << R <<endl;
			}
			cout<<endl;
		}
	fin->Close();

///////////////////
	cout<<" pe.size():" << pe.size() <<endl;
	Int_t nPoints=pe.size();	
	std::vector<Int_t> idx;
	idx.reserve( nPoints );
	TMath::Sort(nPoints, &pe[0], &idx[0], kFALSE);
	//for(Int_t i=0; i<nPoints; i++)
	//{
	//	cout<<" bf:" << pe[i] <<" - af:" << pe[ idx[i] ] <<endl;
	//}

	TGraph *g=new TGraph();
	g->SetMarkerStyle( 20 );
	for(Int_t i=0; i<nPoints; i++)
	{
		g->SetPoint(i, pe[idx[i]], rate[idx[i]]/100.);
	}
	g->Fit("pol1");
	g->Draw("APL");
	
	TF1 *f=(TF1*)g->GetFunction("pol1");
	TFile *fout=new TFile("iwcd_3inch_afterpulse_fittedrate.root", "recreate");
		g->Write("pe_rate_allpmts");
		f->Write("fitted_pe_rate");
	fout->cd();
}
