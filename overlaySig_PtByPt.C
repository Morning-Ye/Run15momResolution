#include"TPaveStats.h"
#include"TROOT.h"

const int NPtBins = 7;
const int PTBIN[NPtBins+1]={3, 4,  5, 6, 7,  8, 9, 10};

TFile *fin_data[NPtBins];
TFile *fin_Smr;

TH1F* HSig_Data[NPtBins];
TH1F* HSig_Smr[NPtBins];

void readFiles(TString sys);
void draw(TString sys);


void overlaySig_PtByPt(TString sys="pAu"){
//void overlaySig_PtByPt(TString sys="pp"){
	
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);

	readFiles(sys);
	draw(sys);
}

void readFiles(TString sys){
	//read in Files
	if(sys=="pp"){
		fin_Smr  = new TFile("../out_nophotos_3.065/Jpsi.toyMc_ia_55.root","read");
		cout<<"read in fin_Smr: "<<fin_Smr->GetName()<<endl;
		for(int i=0; i<NPtBins; i++){
			fin_data[i] = new TFile(Form("./outrootfile/BHT2_run15pp_out__pt_%d_%d_masssig.root",PTBIN[i], PTBIN[i+1]),"read");
			cout<<"read in fin_data: "<<fin_data[i]->GetName()<<endl;
		}
	}
	else if(sys=="pAu")
	{
		fin_Smr  = new TFile("../out_nophotos_3.065/Jpsi.toyMc_ia_63.root","read");	
		cout<<"read in fin_Smr: "<<fin_Smr->GetName()<<endl;
		for(int i=0; i<NPtBins; i++){
			fin_data[i] = new TFile(Form("./outrootfile/BHT2_run15pp_out__pt_%d_%d_masssig.root",PTBIN[i], PTBIN[i+1]),"read");
			cout<<"read in fin_data: "<<fin_data[i]->GetName()<<endl;
		}
	}


	//get histograms
	TH3F* H_RcQM_PtMCent = (TH3F*) fin_Smr ->Get("hRcQM_PtMCent");
	TH1F* HtemPt = (TH1F*) H_RcQM_PtMCent->ProjectionX("HtemPt");

	for(int i=0; i<NPtBins; i++){

		int lowptbin = HtemPt->FindBin(PTBIN[i]);
		int highptbin = HtemPt->FindBin(PTBIN[i+1]);

		HSig_Smr[i] = (TH1F*)H_RcQM_PtMCent->ProjectionY( Form("HSig_Smr_%d",i), lowptbin, highptbin, 0, -1);
		HSig_Smr[i]->SetName( Form("HSig_Smr_%d",i) );
		HSig_Data[i] = (TH1F*)fin_data[i]->Get("H_PMass_SIG_noBgk_2");
		HSig_Data[i]->SetName( Form("HSig_Data_%d",i) );
	}
}


void draw(TString sys){

	TCanvas *c1 = new TCanvas("c1");

	for(int i=0; i<NPtBins; i++){
		//HSig_Data[i]->GetFunction("fitf")->SetBit(TF1::kNotDraw);

		HSig_Smr[i]->Rebin(10);
		HSig_Smr[i]->SetMarkerStyle(24);
		HSig_Smr[i]->SetMarkerColor(4);
		HSig_Smr[i]->Scale(HSig_Data[i]->GetMaximum()/HSig_Smr[i]->GetMaximum());
		HSig_Data[i]->SetAxisRange(0, 1.6*HSig_Data[i]->GetMaximum(), "y");
		HSig_Data[i]->Draw("pe");
		HSig_Smr[i]->Draw("pesame");
		
		TLatex tl;
		tl.SetNDC();
		tl.DrawLatex(0.15, 0.6, Form("%d <p_{T}< %d GeV/c", PTBIN[i], PTBIN[i+1]));

		TLegend *leg2 = new TLegend(0.13, 0.7, 0.45,0.88);
		leg2->AddEntry( HSig_Smr[i],  "J/#psi from Smr "+sys,"lp");
		leg2->AddEntry( HSig_Data[i],  "J/#psi from Data "+sys,"lp");
		leg2->Draw();


		c1->SaveAs( Form("./plots/jpsiSignal_SmrVsData_pt%d_%d_"+sys+".png", PTBIN[i], PTBIN[i+1]) );
	}
}


