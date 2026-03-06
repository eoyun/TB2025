#include "TBread.h"
#include "TButility.h"

#include <filesystem>
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "function.h"

namespace fs = std::filesystem;


int main(int argc, char** argv) {

    int CC1_peak_first = 650; // Peak search range
    int CC1_peak_last  = 750; // Peak search range
    
    int CC2_peak_first = 620; // Peak search range
    int CC2_peak_last  = 850; // Peak search range
    
    int PS_first = 200; // PS integration range
    int PS_last  = 320; // PS integration range
    
    int MC_first = 650; // MC integration range
    int MC_last  = 850; // MC integration range
    
    int TC_first = 300; // TC peak search range
    int TC_last  = 450; // TC peak search range
    
    int LC_first = 400;    // LC integration range  
    int LC_last  = 600; // LC integration range
    
    // cuts
    float cut_CC1  = 60.;   // PID cut for CC1 (PeakADC)
    float cut_CC2  = 100.;  // PID cut for CC2 (PeakADC)
    
    float cut_PS1 = 50.; // PID cut for PS (PeakADC)
    float cut_PS2 = 300.; // PID cut for PS (PeakADC)
    float cut_MC = 38.;   // PID cut for MC (PeakADC)
    
    float cut_DWC = 4; // Beam geometry cut for DWC
    
    int fRunNum = std::stoi(argv[1]);
    int fMaxEvent = std::stoi(argv[2]);
    int fMaxFile = -1;

    
    TFile* f_DWC = TFile::Open((TString)("./DWC/DWC_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    TH2D* h_DWC1_pos   = (TH2D*) f_DWC->Get("dwc1_pos");
    TH2D* h_DWC2_pos   = (TH2D*) f_DWC->Get("dwc2_pos");
    std::vector<float> DWC1_offset = getDWCoffset(h_DWC1_pos); // DWC1_offset.at(0) == X, DWC1_offset.at(1) == Y
    std::vector<float> DWC2_offset = getDWCoffset(h_DWC2_pos);
    f_DWC->Close();
    
    fs::path dir("./wave_Cor");   
    if (!(fs::exists(dir))) fs::create_directory(dir);
    std::string outFile = "./wave_Cor/wave_Cor_Run_" + std::to_string(fRunNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");

    TH1F* hist_CC1 = new TH1F("CC1", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_CC2 = new TH1F("CC2", ";peakADC;Events", 1024, 0, 4096);
    
    TH1F* hist_PS = new TH1F("PS" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_MC = new TH1F("MC" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_TC = new TH1F("TC" , ";peakADC;nEvents", 1024, 0, 4096);

    TH2D* hist_DWC1_pos_corrected   = new TH2D("DWC1_pos_corrected",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_corrected   = new TH2D("DWC2_pos_corrected",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_corrected = new TH2D("DWC_x_corr_corrected", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_corrected = new TH2D("DWC_y_corr_corrected", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);
    
    TH1F* hist_PS_after = new TH1F("PS_after" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_MC_after = new TH1F("MC_after" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_TC_after = new TH1F("TC_after" , ";peakADC;nEvents", 1024, 0, 4096);
    
    TH1F* hist_CC1_after = new TH1F("CC1_after", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_CC2_after = new TH1F("CC2_after", ";peakADC;Events", 1024, 0, 4096);
    
    TH2D* hist_DWC1_pos_after   = new TH2D("DWC1_pos_after",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_after   = new TH2D("DWC2_pos_after",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_after = new TH2D("DWC_x_corr_after", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_after = new TH2D("DWC_y_corr_after", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);

    // initialize the utility class
    TButility util = TButility();
    util.LoadMapping("../mapping/mapping_TB2025_v1.root");
    TBcid cid_CC1 = util.GetCID("CC1");
    TBcid cid_CC2 = util.GetCID("CC2");
    TBcid cid_PS = util.GetCID("PS");
    TBcid cid_MC = util.GetCID("MC");
    TBcid cid_TC = util.GetCID("TC");

    TBcid cid_DWC1_L = util.GetCID("DWC1L");
    TBcid cid_DWC1_R = util.GetCID("DWC1R");
    TBcid cid_DWC1_U = util.GetCID("DWC1U");
    TBcid cid_DWC1_D = util.GetCID("DWC1D");
    TBcid cid_DWC2_L = util.GetCID("DWC2L");
    TBcid cid_DWC2_R = util.GetCID("DWC2R");
    TBcid cid_DWC2_U = util.GetCID("DWC2U");
    TBcid cid_DWC2_D = util.GetCID("DWC2D");

    TBcid cid_M4_T2_S = util.GetCID("M5-T2-S");

    TH2D* wave_uncor = new TH2D("wave_uncor","",1000,0,1000,200,-100,100);
    TH2D* wave_cor = new TH2D("wave_cor","",1000,0,1000,200,-100,100);


    // MID: 3-7: PMT modules, MID 9: LC, MID 10: Aux(CC1, CC2, PS, TC, MC), MID 12: Triggers (T1, T2, T1NIM, T2NIM, Coin), MID 14-17: MCP micro, MID 18: DWC
    // TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, "/Volumes/Macintosh HD-1/Users/yhep/scratch/YUdaq", {3, 4, 5, 6, 7, 9, 10, 12, 18});
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_Data/", {3, 4, 5, 6, 7, 10, 12, 18});
    // Set Maximum event
    if (fMaxEvent == -1)
      fMaxEvent = readerWave.GetMaxEvent();
  
    if (fMaxEvent > readerWave.GetMaxEvent())
      fMaxEvent = readerWave.GetMaxEvent();
  

    // Evt Loop

    for (int iEvt = 0; iEvt < fMaxEvent; iEvt++) {
	printProgress(iEvt, fMaxEvent);
	TBevt<TBwaveform> aEvent = readerWave.GetAnEvent();
	TBwaveform PS_wave = aEvent.GetData(cid_PS);
	TBwaveform MC_wave = aEvent.GetData(cid_MC);
	TBwaveform TC_wave = aEvent.GetData(cid_TC);
	TBwaveform CC1_wave = aEvent.GetData(cid_CC1);
	TBwaveform CC2_wave = aEvent.GetData(cid_CC2);

	TBwaveform DWC1L_wave = aEvent.GetData(cid_DWC1_L);
	TBwaveform DWC1R_wave = aEvent.GetData(cid_DWC1_R);
	TBwaveform DWC1U_wave = aEvent.GetData(cid_DWC1_U);
	TBwaveform DWC1D_wave = aEvent.GetData(cid_DWC1_D);
	TBwaveform DWC2L_wave = aEvent.GetData(cid_DWC2_L);
	TBwaveform DWC2R_wave = aEvent.GetData(cid_DWC2_R);
	TBwaveform DWC2U_wave = aEvent.GetData(cid_DWC2_U);
	TBwaveform DWC2D_wave = aEvent.GetData(cid_DWC2_D);
	
	std::vector<short> waveform_PS = PS_wave.waveform();	
	std::vector<short> waveform_MC = MC_wave.waveform();	
	std::vector<short> waveform_TC = TC_wave.waveform();	
	std::vector<short> waveform_CC1 = CC1_wave.waveform();	
	std::vector<short> waveform_CC2 = CC2_wave.waveform();	

	std::vector<short> waveform_DWC1L = DWC1L_wave.waveform();	
	std::vector<short> waveform_DWC1R = DWC1R_wave.waveform();	
	std::vector<short> waveform_DWC1U = DWC1U_wave.waveform();	
	std::vector<short> waveform_DWC1D = DWC1D_wave.waveform();	
	std::vector<short> waveform_DWC2L = DWC2L_wave.waveform();	
	std::vector<short> waveform_DWC2R = DWC2R_wave.waveform();	
	std::vector<short> waveform_DWC2U = DWC2U_wave.waveform();	
	std::vector<short> waveform_DWC2D = DWC2D_wave.waveform();	

	std::vector<float> DWC1_time;
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC1R, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC1L, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC1U, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC1D, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        
        std::vector<float> DWC2_time;
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC2R, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC2L, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC2U, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC2D, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2

	std::vector<float> DWC1_corrected_pos = getDWC1position(DWC1_time, DWC1_offset); // DWC1 X, Y
        std::vector<float> DWC2_corrected_pos = getDWC2position(DWC2_time, DWC2_offset); // DWC2 X, Y

	double signal_PS = GetPeak(waveform_PS, PS_first, PS_last); // PeakADC
        double signal_MC = GetPeak(waveform_MC, MC_first, MC_last); // PeakADC
        double signal_TC = GetPeak(waveform_TC, TC_first, TC_last); // PeakADC
        double signal_CC1 = GetPeak(waveform_CC1, CC1_peak_first, CC1_peak_last); // PeakADC
        double signal_CC2 = GetPeak(waveform_CC2, CC2_peak_first, CC2_peak_last); // PeakADC

        hist_CC1->Fill(signal_CC1);
        hist_CC2->Fill(signal_CC2);
        
        hist_PS->Fill(signal_PS);
        hist_MC->Fill(signal_MC);
        hist_TC->Fill(signal_TC);

	hist_DWC1_pos_corrected  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_corrected  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1));
        hist_DWC_x_corr_corrected->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_corrected->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));
        if ( !(dwcCorrelationCut(DWC1_corrected_pos, DWC2_corrected_pos, cut_DWC)) ) continue;
	//if ( std::abs(DWC1_corrected_pos.at(0))>5 || std::abs(DWC1_corrected_pos.at(1))>5) continue;
        //if ( std::abs(DWC2_corrected_pos.at(0))>5 || std::abs(DWC2_corrected_pos.at(1))>5) continue;
        //if ( signal_PS < cut_PS1 || signal_PS > cut_PS2 ) continue; // Select above 3 mip peak
        //if ( signal_MC < cut_MC ) continue; // Select only pedestals
        if ( signal_MC > cut_MC ) continue; // Select only pedestals
        if ( signal_PS > cut_PS2 ) continue; // Select only pedestals
        
	hist_CC1_after->Fill(signal_CC1);
        hist_CC2_after->Fill(signal_CC2);
        
        hist_PS_after->Fill(signal_PS);
        hist_MC_after->Fill(signal_MC);
        hist_TC_after->Fill(signal_TC);

        hist_DWC1_pos_after  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_after  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1));
        hist_DWC_x_corr_after->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_after->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));

	TBwaveform M4_T2_S_wave = aEvent.GetData(cid_M4_T2_S);

	std::vector<float> waveuncor = M4_T2_S_wave.pedcorrectedWaveform();	
	std::vector<float> wavecor = M4_T2_S_wave.ADCpedcorrectedWaveform();	

	for (int i =1; i < 1001; i++){
	  //std::cout<<" wave value : " <<waveuncor.at(i)<<" | "<<wavecor.at(i)<<std::endl;
	  wave_uncor->Fill(i-1,waveuncor.at(i));
	  wave_cor->Fill(i-1,wavecor.at(i));
	}


    }
    outputRoot->cd();
    hist_CC1->Write();
    hist_CC2->Write();
    
    hist_PS->Write();
    hist_MC->Write();
    hist_TC->Write();
    
    hist_DWC1_pos_corrected  ->Write();
    hist_DWC2_pos_corrected  ->Write();
    hist_DWC_x_corr_corrected->Write();
    hist_DWC_y_corr_corrected->Write();

    hist_CC1_after->Write();
    hist_CC2_after->Write();
    
    hist_PS_after->Write();
    hist_MC_after->Write();
    hist_TC_after->Write();
    
    hist_DWC1_pos_after  ->Write();
    hist_DWC2_pos_after  ->Write();
    hist_DWC_x_corr_after->Write();
    hist_DWC_y_corr_after->Write();
    
    wave_uncor->Write();
    wave_cor->Write();

    outputRoot->Close();

}

