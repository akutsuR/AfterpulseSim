#include <vector>

void fit_afterpulse_rate_hzc()
{
	const Int_t NPMTS=2;
	const std::string TreeNames[NPMTS]
			={"fraction_hzc72",
			  "fraction_hzc82"
			 };
	std::vector<Float_t> pe; // 72 + 82
	std::vector<Float_t> pe_72;
	std::vector<Float_t> pe_82;
	std::vector<Float_t> rate;
	std::vector<Float_t> rate_72;
	std::vector<Float_t> rate_82;
    std::vector<Float_t> err;
	std::vector<Float_t> err_72;
	std::vector<Float_t> err_82;
    

	Float_t Q;
	Float_t R;
    Float_t E;
	string sQ="charge";
	string sR="fraction_c";	
    string sE="error_c";
	TFile *fin=TFile::Open("ap_fraction_3.root");
		for(Int_t i=0; i<NPMTS; i++)
		{
			TTree *t=(TTree*)fin->Get( TreeNames[i].c_str() );
			t->SetBranchAddress(sQ.c_str(), &Q);
			t->SetBranchAddress(sR.c_str(), &R);
			t->SetBranchAddress(sE.c_str(), &E);

			cout<<" i:" << i <<" - " << t->GetEntries() <<endl;
			for(Int_t j=0; j<t->GetEntries(); j++)
			{
				t->GetEntry( j );

				pe.push_back( Q );
				rate.push_back( R );
                err.push_back( E );
                if( i==0 )
                {
				    pe_72.push_back( Q );
				    rate_72.push_back( R );
                    err_72.push_back( E );
                }
                else
                {
				    pe_82.push_back( Q );
				    rate_82.push_back( R );
                    err_82.push_back( E );
                }
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

	TGraphErrors *g=new TGraphErrors();
	TGraphErrors *g_72=new TGraphErrors();
	TGraphErrors *g_82=new TGraphErrors();
	g->SetMarkerStyle( 20 );
	g_72->SetMarkerStyle( 20 );
	g_82->SetMarkerStyle( 20 );
    g_72->SetMarkerColor( kBlue+2 );
    g_82->SetMarkerColor( kRed+2 );
    g_72->SetLineColor( kBlue-4 );
    g_82->SetLineColor( kRed-4 );

	for(Int_t i=0; i<nPoints; i++)
	{
		g->SetPoint(i, pe[idx[i]], rate[idx[i]]/100.);
        g->SetPointError(i, 0., err[idx[i]]/100.);
	}

    nPoints=(Int_t)pe_72.size();
	for(Int_t i=0; i<nPoints; i++)
	{
		g_72->SetPoint(i, pe_72[i], rate_72[i]/100.);
        g_72->SetPointError(i, 0., err_72[i]/100.);
	}

    nPoints=(Int_t)pe_82.size();
	for(Int_t i=0; i<nPoints; i++)
	{
		g_82->SetPoint(i, pe_82[i], rate_82[i]/100.);
        g_82->SetPointError(i, 0., err_82[i]/100.);
	}



	g->Fit("pol1");
	g->Draw("APL");
	
	TF1 *f=(TF1*)g->GetFunction("pol1");
	TFile *fout=new TFile("iwcd_3inch_afterpulse_fittedrate_hzc.root", "recreate");
		g->Write("pe_rate_allpmts_hzc");
		g_72->Write("pe_rate_apmt_hzc_72");
		g_82->Write("pe_rate_apmt_hzc_82");
		f->Write("fitted_pe_rate_hzc");
	fout->cd();
}
