const int NPtBins = 7;
double PTBIN[NPtBins+1]={3., 4,  5., 6., 7,  8., 9, 10};
double lwm = 2.0, higm=4.0;

double CrystalBall(double* x, double* par);
void compDataSmrWidthVsPt ( )
{

	gStyle->SetOptFit(1111);
	gStyle->SetLineWidth(2);
	TH1::SetDefaultSumw2();
	gStyle->SetOptStat(0);
	TCanvas *c1 = new TCanvas("c1");
	//TFile *fin_ppSmr = new TFile(Form("../olddata/out_nophotos/Jpsi.toyMc_ia_%.i.root", index), "read");
	TFile *fin_ppSmr = new TFile(Form("../out_nophotos_3.065/Jpsi.toyMc_ia_52.root"), "read");
	TH3F *H_ppSmrQMPtvsM = (TH3F*)fin_ppSmr->Get( "hRcQM_PtMCent" );
	TH1F* H_Pt_ppSmr = (TH1F*)H_ppSmrQMPtvsM->ProjectionX("H_Pt_ppSmr");
	
	TFile *fin_pAuSmr = new TFile(Form("../out_nophotos_3.065/Jpsi.toyMc_ia_63.root"), "read");
	TH3F *H_pAuSmrQMPtvsM = (TH3F*)fin_pAuSmr->Get( "hRcQM_PtMCent" );
	TH1F* H_Pt_pAuSmr = (TH1F*)H_pAuSmrQMPtvsM->ProjectionX("H_Pt_pAuSmr");
	
	H_Pt_ppSmr->SetLineColor(2);
	H_Pt_pAuSmr->SetLineColor(4);
	H_Pt_ppSmr->Draw();
	H_Pt_pAuSmr->Draw("same");
	c1->SaveAs("./plots/H_PtSmr_pppAu.png");
	
	TH1F* H_ppSmrQMM[NPtBins-1];
	TH1F* H_ppSmrQMWth_Pt = new TH1F("H_ppSmrQMWth_Pt",   "smeared J/#psi peak width vs pt", NPtBins, PTBIN);
	TH1F* H_ppSmrQMMean_Pt = new TH1F("H_ppSmrQMMean_Pt", "smeared J/#psi peak mean vs pt",  NPtBins, PTBIN);
	TH1F* H_pAuSmrQMM[NPtBins-1];
	TH1F* H_pAuSmrQMWth_Pt = new TH1F("H_pAuSmrQMWth_Pt",   "smeared J/#psi peak width vs pt", NPtBins, PTBIN);
	TH1F* H_pAuSmrQMMean_Pt = new TH1F("H_pAuSmrQMMean_Pt", "smeared J/#psi peak mean vs pt",  NPtBins, PTBIN);
	
	TF1 *fitf = new TF1("fitf", CrystalBall, lwm, higm, 5);
	fitf->SetLineColor(1);
	fitf->SetParNames("NS", "mass", "Width", "n", "#alpha");
	fitf->SetParameters(17, 3.05,  0.035,  2,  1 );
	fitf->SetParLimits(1, 2.70, 3.3);
	fitf->SetParLimits(2, 0.00, 0.5);
	fitf->SetParLimits(3, 0.,  20);
	fitf->SetParLimits(4, 0.0, 20);
	
	TLatex tl;
	
	for(int i=0; i<NPtBins; i++){
		int	lb=H_Pt_ppSmr->FindBin( PTBIN[i] );
		int hb=H_Pt_ppSmr->FindBin( PTBIN[i+1] );

		H_ppSmrQMM[i] = (TH1F*)H_ppSmrQMPtvsM->ProjectionY( Form("H_ppSmrQMM_%.i", i), lb, hb);
		H_ppSmrQMM[i]->Rebin(10);
		H_ppSmrQMM[i]->Draw();

		H_ppSmrQMM[i]->Fit(fitf, "R", "", lwm, higm);

		H_ppSmrQMMean_Pt->SetBinContent(i+1, fitf->GetParameter(1));
		H_ppSmrQMMean_Pt->SetBinError(i+1, fitf->GetParError(1));
		H_ppSmrQMWth_Pt->SetBinContent(i+1, fitf->GetParameter(2));
		H_ppSmrQMWth_Pt->SetBinError(i+1, fitf->GetParError(2));

		tl.SetNDC();
		tl.SetTextSize(0.04);
		tl.DrawLatex(0.15, 0.8, Form("%.2f<pt<%.2f", PTBIN[i], PTBIN[i+1]));
		c1->SaveAs(Form("plots/H_ppSmrQMM_%.i.png",i+1));
		cout<<"------------------------------------------------"<<endl;
		
		H_pAuSmrQMM[i] = (TH1F*)H_pAuSmrQMPtvsM->ProjectionY( Form("H_pAuSmrQMM_%.i", i), lb, hb);
		H_pAuSmrQMM[i]->Rebin(10);
		H_pAuSmrQMM[i]->Draw();

		H_pAuSmrQMM[i]->Fit(fitf, "R", "", lwm, higm);

		H_pAuSmrQMMean_Pt->SetBinContent(i+1, fitf->GetParameter(1));
		H_pAuSmrQMMean_Pt->SetBinError(i+1, fitf->GetParError(1));
		H_pAuSmrQMWth_Pt->SetBinContent(i+1, fitf->GetParameter(2));
		H_pAuSmrQMWth_Pt->SetBinError(i+1, fitf->GetParError(2));

		tl.SetNDC();
		tl.SetTextSize(0.04);
		tl.DrawLatex(0.15, 0.8, Form("%.2f<pt<%.2f", PTBIN[i], PTBIN[i+1]));
		c1->SaveAs(Form("plots/H_pAuSmrQMM_%.i.png",i+1));
		cout<<"------------------------------------------------"<<endl;
		

	}

	H_ppSmrQMWth_Pt->SetAxisRange(0, 0.3, "Y");
	H_ppSmrQMWth_Pt->SetAxisRange(4, 9, "X");
	H_ppSmrQMWth_Pt->SetYTitle("J/#psi mass Width");
	H_ppSmrQMWth_Pt->SetXTitle("J/#psi p_{T} Bin");
	H_ppSmrQMWth_Pt->SetLineWidth(2);
	H_ppSmrQMWth_Pt->Draw("pe");
	H_pAuSmrQMWth_Pt->SetLineWidth(2);
	H_pAuSmrQMWth_Pt->SetLineColor(5);
	H_pAuSmrQMWth_Pt->Draw("pesame");

	//readin the width vs pt from real data
	TFile *f15pp = new TFile("outrootfile/BHT2_run15pp_out_width.root", "read");
	TH1F* Hmasswidth_pp = (TH1F*) f15pp->Get("Hmasswidth");
	TH1F* Hwid_pt_15pp = (TH1F*)H_ppSmrQMWth_Pt->Clone("Hwid_pt_15pp");
	Hwid_pt_15pp->Reset();

	TH1F* Hmassmean_pp = (TH1F*) f15pp->Get("Hmassmean");
	TH1F* Hmean_pt_15pp = (TH1F*)H_ppSmrQMMean_Pt->Clone("Hmean_pt_15pp");
	Hmean_pt_15pp->Reset();

	TFile *f15pau = new TFile("outrootfilepau/BHT2_run15pau_out_width.root", "read");
	TH1F* Hmasswidth_pau = (TH1F*) f15pau->Get("Hmasswidth");
	TH1F* Hwid_pt_15pau = (TH1F*)H_ppSmrQMWth_Pt->Clone("Hwid_pt_15pau");
	Hwid_pt_15pau->Reset();

	TH1F* Hmassmean_pau = (TH1F*) f15pau->Get("Hmassmean");
	TH1F* Hmean_pt_15pau = (TH1F*)H_ppSmrQMMean_Pt->Clone("Hmean_pt_15pau");
	Hmean_pt_15pau->Reset();

	//cut off the last two Bins;
	for(int i=1; i<8; i++){
		double iwid = Hmasswidth_pp->GetBinContent(i+1);
		double iwiderr = Hmasswidth_pp->GetBinError(i+1);
		
		double imean = Hmassmean_pp->GetBinContent(i+1);
		double imeanerr = Hmassmean_pp->GetBinError(i+1);

		cout<<"run15 width: "<<iwid<<endl;
		cout<<"run15 widtherr: "<<iwiderr<<endl;
		
		cout<<"run15 width: "<<imean<<endl;
		cout<<"run15 widerr: "<<imeanerr<<endl;
		cout<<"------------------------------"<<endl;

		Hwid_pt_15pp->SetBinContent(i, Hmasswidth_pp->GetBinContent(i+1));
		Hwid_pt_15pp->SetBinError(i, Hmasswidth_pp->GetBinError(i+1));
		Hmean_pt_15pp->SetBinContent(i, Hmassmean_pp->GetBinContent(i+1));
		Hmean_pt_15pp->SetBinError(i,   Hmassmean_pp->GetBinError(i+1));
	
	
		Hwid_pt_15pau->SetBinContent(i, Hmasswidth_pau->GetBinContent(i+1));
		Hwid_pt_15pau->SetBinError(i, Hmasswidth_pau->GetBinError(i+1));
		Hmean_pt_15pau->SetBinContent(i, Hmassmean_pau->GetBinContent(i+1));
		Hmean_pt_15pau->SetBinError(i,   Hmassmean_pau->GetBinError(i+1));
	
	
	}
	Hwid_pt_15pp->SetLineColor(2);
	Hwid_pt_15pp->SetLineWidth(2);
	Hwid_pt_15pp->Draw("pesame");
	Hwid_pt_15pau->SetLineColor(11);
	Hwid_pt_15pau->SetLineWidth(2);
	Hwid_pt_15pau->Draw("pesame");


	TLegend *leg2 = new TLegend(0.15, 0.6, 0.6,0.88);
	leg2->AddEntry( Hwid_pt_15pp,  "Run15 pp J/#psi width","lp");
	leg2->AddEntry( Hwid_pt_15pau,  "Run15 pAu J/#psi width","lp");
	leg2->AddEntry( H_ppSmrQMWth_Pt,  "smeared J/#psi width pp ","lp");
	leg2->AddEntry( H_pAuSmrQMWth_Pt,  "smeared J/#psi width pAu ","lp");
	leg2->Draw();

	c1->SaveAs("./plots/H_ppSmrQMWth_Pt.png");

	//draw mean vs pt

	H_ppSmrQMMean_Pt->SetAxisRange(3.0, 3.25, "Y");
	H_ppSmrQMMean_Pt->SetAxisRange(4.0, 9, "X");
	H_ppSmrQMMean_Pt->SetYTitle("J/#psi mass Mean");
	H_ppSmrQMMean_Pt->SetXTitle("J/#psi p_{T} Bin");
	H_ppSmrQMMean_Pt->SetLineWidth(2);
	H_ppSmrQMMean_Pt->Draw("pe");
	H_pAuSmrQMMean_Pt->SetLineWidth(2);
	H_pAuSmrQMMean_Pt->SetLineColor(5);
	H_pAuSmrQMMean_Pt->Draw("pesame");

	Hmean_pt_15pp->SetLineColor(2);
	Hmean_pt_15pp->SetLineWidth(2);
	Hmean_pt_15pp->Draw("pesame");

	Hmean_pt_15pau->SetLineColor(11);
	Hmean_pt_15pau->SetLineWidth(2);
	Hmean_pt_15pau->Draw("pesame");

	TLine * lj = new TLine(4, 3.096, 10, 3.096);
	lj->SetLineStyle(7);
	lj->Draw("lsame");
	TLegend *leg3 = new TLegend(0.15, 0.6, 0.6,0.88);
	leg3->AddEntry( lj,  "J/#psi m in PDG","l");
	leg3->AddEntry( Hmean_pt_15pp,  "Run15pp J/#psi","lp");
	leg3->AddEntry( Hmean_pt_15pau, "Run15pau J/#psi","lp");
	leg3->AddEntry( H_ppSmrQMMean_Pt,  "smeared J/#psi pp ","lp");
	leg3->AddEntry( H_pAuSmrQMMean_Pt,  "smeared J/#psi pAu ","lp");
	leg3->Draw();

	c1->SaveAs("./plots/H_ppSmrQMMean_Pt.png");

	TFile * outfile = new TFile("out_QMWth_Pt_.root", "recreate");
	outfile->cd();
	H_ppSmrQMWth_Pt->Write();
	outfile->Close();
}


//--------------------------------------------
double CrystalBall(double* x, double* par){
	Double_t N = par[0];
	Double_t mu = par[1];
	Double_t s = par[2];
	Double_t n = par[3];
	Double_t alpha1 = par[4];

	Double_t A = TMath::Power(n/fabs(alpha1), n) * TMath::Exp(-alpha1*alpha1/2.);
	Double_t B = n/fabs(alpha1) - fabs(alpha1);
	Double_t norm = (x[0]-mu)/s;

	if(norm > -alpha1) {
		return N * TMath::Exp(-0.5*norm*norm);
	} else {
		return N * A * TMath::Power(B-norm, -n);
	}
}
//------------------------------------------------------


