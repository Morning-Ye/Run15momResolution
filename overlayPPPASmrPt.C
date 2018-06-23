#include"TPaveStats.h"

const double lowm = 2.7; 
const double highm = 3.3;

void overlayPPPASmrPt(){
	TCanvas *c1 = new TCanvas("c1");
	gStyle->SetOptStat(0);

	TFile* fpp = new TFile("./outrootfile/BHT2_run15pp_out__pt_4_10_masssig.root", "read");
	TFile* fpau = new TFile("./outrootfilepau/BHT2_run15pAu_out__pt_4_10_masssig.root", "read");
	TH1F* h_pt_pp = (TH1F*) fpp->Get("Hsm_PmassSIGPt_2");
	TH1F* h_pt_pau = (TH1F*) fpau->Get("Hsm_PmassSIGPt_2");
	
	//TFile *fppSmr = new TFile("../olddata/out_nophotos/Jpsi.toyMc_ia_56.root", "read");
	TFile *fppSmr = new TFile("../out_nophotos_3.065/Jpsi.toyMc_ia_56.root", "read");
	TH3F *H_ppSmrQMPtvsM = (TH3F*)fppSmr->Get("hRcQM_PtMCent");
	TH1F* h_mass_ppSmr = (TH1F*)H_ppSmrQMPtvsM->ProjectionY("h_mass_ppSmr");
	int lowm_bin_pp = h_mass_ppSmr->FindBin(lowm);
	int highm_bin_pp = h_mass_ppSmr->FindBin(highm);
	TH1F* h_pt_ppSmr = (TH1F*)H_ppSmrQMPtvsM->ProjectionX("h_pt_ppSmr", lowm_bin_pp, highm_bin_pp);

	TFile *fpAuSmr = new TFile("../out_nophotos_3.065/Jpsi.toyMc_ia_66.root", "read");
	TH3F *H_pAuSmrQMPtvsM = (TH3F*)fpAuSmr->Get("hRcQM_PtMCent");
	TH1F* h_mass_pAuSmr = (TH1F*)H_pAuSmrQMPtvsM->ProjectionY("h_mass_pAuSmr");
	int lowm_bin_pAu = h_mass_pAuSmr->FindBin(lowm);
	int highm_bin_pAu = h_mass_pAuSmr->FindBin(highm);
	TH1F* h_pt_pAuSmr = (TH1F*)H_pAuSmrQMPtvsM->ProjectionX("h_pt_pAuSmr", lowm_bin_pAu, highm_bin_pAu);

	h_pt_pp->Rebin(5);
	h_pt_pau->Rebin(5);
	h_pt_pau->Scale(1300./900);
	h_pt_pp->SetMarkerStyle(20);
	h_pt_pp->SetMarkerSize(0.8);
	h_pt_pau->SetMarkerStyle(21);
	h_pt_pau->SetMarkerColor(2);
	h_pt_pau->SetMarkerSize(0.8);
	h_pt_pp->SetAxisRange(0, 2000, "y");
	h_pt_pp->Draw("pe");
	h_pt_pau->Draw("pesame");
	h_pt_ppSmr->SetMarkerStyle(20);
	h_pt_ppSmr->SetMarkerColor(4);
	h_pt_ppSmr->Sumw2();
	h_pt_ppSmr->Rebin(5);
	h_pt_ppSmr->Scale(1.2*h_pt_pp->Integral()/h_pt_ppSmr->Integral());
	h_pt_ppSmr->Draw("pesame");
	h_pt_pAuSmr->SetMarkerStyle(21);
	h_pt_pAuSmr->SetMarkerColor(5);
	h_pt_pAuSmr->Sumw2();
	h_pt_pAuSmr->Rebin(5);
	h_pt_pAuSmr->Scale(1.2*h_pt_pp->Integral()/h_pt_pAuSmr->Integral());
	h_pt_pAuSmr->Draw("pesame");


	TLegend *leg2 = new TLegend(0.6, 0.6, 0.88,0.88);
	leg2->AddEntry( h_pt_pp,    "J/#psi pt Run15 pp","lp");
	leg2->AddEntry( h_pt_pau,   "J/#psi pt Run15 pAu","lp");
	leg2->AddEntry( h_pt_ppSmr, "J/#psi pt Run15 ppSmr","lp");
	leg2->AddEntry( h_pt_pAuSmr, "J/#psi pt Run15 pAuSmr","lp");
	leg2->Draw("same");	

	c1->SaveAs("plots/comparPPPAuSmr_pt.gif");

}
