#include"TPaveStats.h"

const double ptlow = 4.0; 
const double pthigh = 10.;


void compSigDataSmr(TString sys = "pp"){
//void compSigDataSmr(TString sys = "pAu"){
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);
	//gStyle->UseCurrentStyle();

	TFile *fin_initSmr; 
	TFile *fin_data;
	TFile *fin_Smr;

	if(sys=="pp"){
		fin_initSmr = new TFile("../out_nophotos_3.068/Jpsi.toyMc_ia_0.root","read");	
		fin_data = new TFile("./outrootfile/BHT2_run15pp_out__pt_4_10_masssig.root","read");
		fin_Smr  = new TFile("../out_nophotos_3.065/Jpsi.toyMc_ia_52.root","read");	
	}
	else if(sys=="pAu")
	{
		fin_initSmr = new TFile("../out_nophotos_3.068/Jpsi.toyMc_ia_20.root","read");	
		fin_data = new TFile("./outrootfilepau/BHT2_run15pau_out__pt_4_10_masssig.root","read");
		fin_Smr  = new TFile("../out_nophotos_3.065/Jpsi.toyMc_ia_63.root","read");	
	}

	TH1F* H_dataJpsiM = (TH1F*)fin_data->Get("H_PMass_SIG_noBgk_2");
	TF1* funcPureSG = (TF1*)fin_data->Get("fpuresig_2");

	TH3F* H_SmrJpsiPtvsM_best = (TH3F*)fin_Smr->Get("hRcQM_PtMCent");
	TH1F* HSMR_PT = (TH1F*) H_SmrJpsiPtvsM_best->ProjectionX("HSMR_PT");
	int lowb =HSMR_PT->FindBin(ptlow);
	int higb = HSMR_PT->FindBin(pthigh);
	TH1F* H_SMRJpsiM_best = (TH1F*)H_SmrJpsiPtvsM_best->ProjectionY("H_SMRJpsiM_best", lowb, higb,1,1);
	
	//initial smear
	TH3F* H_SmrJpsiPtvsM_1 = (TH3F*)fin_initSmr->Get("hRcQM_PtMCent");
	TH1F* H_SMRJpsiM = (TH1F*)H_SmrJpsiPtvsM_1->ProjectionY("H_SMRJpsiM", lowb, higb, 1, 1);


	TCanvas *c1 = new TCanvas("c1");

	H_SMRJpsiM->Rebin(10);
	H_SMRJpsiM_best->Rebin(10);
	
	float scalefct = H_dataJpsiM->GetMaximum()/H_SMRJpsiM->GetMaximum();
	H_SMRJpsiM->Scale(scalefct);
	
	float scalefct2 = H_dataJpsiM->GetMaximum()/H_SMRJpsiM_best->GetMaximum();
	H_SMRJpsiM_best->Scale(scalefct2);

	H_dataJpsiM->SetAxisRange(2,4,"x");
	H_dataJpsiM->SetLineWidth(1.);
	H_dataJpsiM->SetLineColor(2);
	H_SMRJpsiM->SetLineColor(4);
	H_SMRJpsiM->SetLineWidth(2);
	H_SMRJpsiM_best->SetLineColor(8);
	H_SMRJpsiM_best->SetLineWidth(2);


	H_dataJpsiM->SetMarkerStyle(20);
	H_dataJpsiM->SetAxisRange(-20, H_dataJpsiM->GetMaximum()*1.6,"y");
	H_dataJpsiM->Draw("pe");
	funcPureSG->SetLineColor(2);
	//funcPureSG->Draw("lsame");
	H_SMRJpsiM->Draw("histsame");
	H_SMRJpsiM_best->Draw("histsame");

	TLatex tl;
	tl.SetNDC();
	tl.DrawLatex(0.6, 0.6, Form("%.1f<p_{T}<%.1f GeV/c", ptlow, pthigh));


	TLegend *leg2 = new TLegend(0.12, 0.6, 0.5,0.88);
	leg2->SetBorderSize(0);
	leg2->AddEntry( H_dataJpsiM,  "J/#psi from "+sys+" data","lp");
	//leg2->AddEntry( funcPureSG,  "J/#psi fitted from "+sys+" data","l");
	leg2->AddEntry( H_SMRJpsiM,  "J/#psi from embed ","lp");
	leg2->AddEntry( H_SMRJpsiM_best,  "J/#psi from Smr ","lp");
	leg2->Draw("same");	
	
	c1->SaveAs("plots/datajpsioverlay_"+sys+".png");

}
