Double_t AfterPulseDt(Double_t *x, Double_t *p)
{
	// Exponential + constant
	Double_t f=0.;
	Double_t X=-x[0]/p[1];
	f+=p[0]*TMath::Exp( X ) + p[2];

	// First gaussian
	X=(x[0]-p[4])*(x[0]-p[4]);
	X=X/(2.*p[5]*p[5]);
	f+=p[3]*TMath::Exp( -X );

	// Second gaussian
	X=(x[0]-p[7])*(x[0]-p[7]);
	X=X/(2.*p[8]*p[8]);
	f+=p[6]*TMath::Exp( -X );

	// Third gaussian
	X=(x[0]-p[10])*(x[0]-p[10]);
	X=X/(2.*p[11]*p[11]);
	f+=p[9]*TMath::Exp( -X );

	return f;
}

Double_t TrippleGaussian(Double_t *x, Double_t *p)
{
	Double_t f=0.;
	Double_t X=0.;
	for(Int_t i=0; i<3; i++)
	{
		X=(x[0]-p[1+i*3])*(x[0]-p[1+i*3]);
		X=X/(2.*p[2+i*3]*p[2+i*3]);
		f+=p[0+i*3]*TMath::Exp( -X );
	}
	return f;
}

void fit_afterpulse_dt()
{
	TFile *fin=TFile::Open("ap_fraction.root");
	TH1D *hdt=(TH1D*)fin->Get("time_interval")->Clone();
	hdt->SetDirectory( 0 );
	fin->Close();

	hdt->Rebin();
	hdt->Rebin();
	TH1D *htmp=(TH1D*)hdt->Clone();

	const Double_t xmin=0.0;
	const Double_t xmax=10.0;
	const Double_t xmin_fit=0.2;
	const Double_t xmax_fit=9.6;
	TF1 *fapdt=new TF1("fapdt", AfterPulseDt, xmin, xmax, 12);
	fapdt->SetLineColor( kBlue-4 );
	fapdt->SetParameter(0, 80.);
	fapdt->SetParameter(1, 3.);
	fapdt->SetParameter(2, 25.);
	fapdt->SetParameter(3, 400.);
	fapdt->SetParameter(4, 1.);
	fapdt->SetParameter(5, 0.45);
	fapdt->SetParameter(6, 1100.);
	fapdt->SetParameter(7, 2.7);
	fapdt->SetParameter(8, 0.65);
	fapdt->SetParameter(9, 1500.);
	fapdt->SetParameter(10, 3.5);
	fapdt->SetParameter(11, 0.40);

	for(Int_t i=0; i<12; i++){ fapdt->SetParLimits(i, 0.0, 100000000.); }
	fapdt->SetParLimits(2, 1.0, 40.);
	fapdt->SetParLimits(4, 0.8, 1.2);
	fapdt->SetParLimits(7, 2.3, 2.9);
	fapdt->SetParLimits(10, 3.3, 3.7);
	htmp->Fit("fapdt", "", "", xmin_fit, xmax_fit);

	TF1 *fexpflat=new TF1("fexpflat", "[0]*TMath::Exp(-x/[1])+[2]", xmin, xmax);
	fexpflat->SetParameter(0, fapdt->GetParameter(0));
	fexpflat->SetParameter(1, fapdt->GetParameter(1));
	fexpflat->SetParameter(2, fapdt->GetParameter(2));

	TF1 *ftgauss=new TF1("ftgauss", TrippleGaussian, xmin, xmax, 9);
	for(Int_t i=0; i<3; i++)
	{
		ftgauss->SetParameter(0+3*i, fapdt->GetParameter(3+i*3));
		ftgauss->SetParameter(1+3*i, fapdt->GetParameter(4+i*3));
		ftgauss->SetParameter(2+3*i, fapdt->GetParameter(5+i*3));
	}
	ftgauss->SetLineColor( kGreen+1 );

	TCanvas *c=new TCanvas();
	hdt->Draw("hist e");	
	fapdt->Draw("same");
	fexpflat->Draw("same");
	ftgauss->Draw("same");
    c->Print("~/public_html/tmp.png");
//////////////////////////////////////////
		
	TFile *fout=new TFile("iwcd_3inch_afterpulse_fitteddt.root", "recreate");
		ftgauss->Write();
	    fexpflat->Write();
	fout->Close();
}
