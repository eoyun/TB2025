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

    int fRunNum = std::stoi(argv[1]);
    int fMaxEvent = std::stoi(argv[2]);
    int fMaxFile = -1;

    fs::path dir("./Time");   
    if (!(fs::exists(dir))) fs::create_directory(dir);

    int PS_first = 200; // PS integration range
    int PS_last  = 320; // PS integration range
 
    int MC_first = 650; // MC integration range
    int MC_last  = 850; // MC integration range
 
    TFile* f_DWC = TFile::Open((TString)("./DWC/DWC_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    TH2D* h_DWC1_pos   = (TH2D*) f_DWC->Get("dwc1_pos");
    TH2D* h_DWC2_pos   = (TH2D*) f_DWC->Get("dwc2_pos");
    std::vector<float> DWC1_offset = getDWCoffset(h_DWC1_pos); // DWC1_offset.at(0) == X, DWC1_offset.at(1) == Y
    std::vector<float> DWC2_offset = getDWCoffset(h_DWC2_pos);

    TH2D* hist_DWC1_pos_corrected   = new TH2D("DWC1_pos_corrected",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_corrected   = new TH2D("DWC2_pos_corrected",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_corrected = new TH2D("DWC_x_corr_corrected", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_corrected = new TH2D("DWC_y_corr_corrected", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);

    TH2D* hist_DWC1_pos_after   = new TH2D("DWC1_pos_after",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_after   = new TH2D("DWC2_pos_after",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_after = new TH2D("DWC_x_corr_after", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_after = new TH2D("DWC_y_corr_after", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);

    float cut_DWC = 2; // Beam geometry cut for DWC
    // initialize the utility class
    TButility util = TButility();
    util.LoadMapping("../mapping/mapping_TB2025_v1.root");
    
    TFile* fNtuple = TFile::Open((TString)("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_PromptAnalysis/Prompt_ntuple_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    TTreeReader reader("evt", fNtuple);

    // Create TTreeReaderValue for all waveform branches
    TTreeReaderValue<std::vector<short>> wave_M1_T1_C(reader, "wave_M1_T1_C"); TTreeReaderValue<std::vector<short>> wave_M1_T1_S(reader, "wave_M1_T1_S");
    TTreeReaderValue<std::vector<short>> wave_M1_T2_C(reader, "wave_M1_T2_C"); TTreeReaderValue<std::vector<short>> wave_M1_T2_S(reader, "wave_M1_T2_S");
    TTreeReaderValue<std::vector<short>> wave_M1_T3_C(reader, "wave_M1_T3_C"); TTreeReaderValue<std::vector<short>> wave_M1_T3_S(reader, "wave_M1_T3_S");
    TTreeReaderValue<std::vector<short>> wave_M1_T4_C(reader, "wave_M1_T4_C"); TTreeReaderValue<std::vector<short>> wave_M1_T4_S(reader, "wave_M1_T4_S");

    TTreeReaderValue<std::vector<short>> wave_M2_T1_C(reader, "wave_M2_T1_C"); TTreeReaderValue<std::vector<short>> wave_M2_T1_S(reader, "wave_M2_T1_S");
    TTreeReaderValue<std::vector<short>> wave_M2_T2_C(reader, "wave_M2_T2_C"); TTreeReaderValue<std::vector<short>> wave_M2_T2_S(reader, "wave_M2_T2_S");
    TTreeReaderValue<std::vector<short>> wave_M2_T3_C(reader, "wave_M2_T3_C"); TTreeReaderValue<std::vector<short>> wave_M2_T3_S(reader, "wave_M2_T3_S");
    TTreeReaderValue<std::vector<short>> wave_M2_T4_C(reader, "wave_M2_T4_C"); TTreeReaderValue<std::vector<short>> wave_M2_T4_S(reader, "wave_M2_T4_S");

    TTreeReaderValue<std::vector<short>> wave_M3_T1_C(reader, "wave_M3_T1_C"); TTreeReaderValue<std::vector<short>> wave_M3_T1_S(reader, "wave_M3_T1_S");
    TTreeReaderValue<std::vector<short>> wave_M3_T2_C(reader, "wave_M3_T2_C"); TTreeReaderValue<std::vector<short>> wave_M3_T2_S(reader, "wave_M3_T2_S");
    TTreeReaderValue<std::vector<short>> wave_M3_T3_C(reader, "wave_M3_T3_C"); TTreeReaderValue<std::vector<short>> wave_M3_T3_S(reader, "wave_M3_T3_S");
    TTreeReaderValue<std::vector<short>> wave_M3_T4_C(reader, "wave_M3_T4_C"); TTreeReaderValue<std::vector<short>> wave_M3_T4_S(reader, "wave_M3_T4_S");

    TTreeReaderValue<std::vector<short>> wave_M4_T1_C(reader, "wave_M4_T1_C"); TTreeReaderValue<std::vector<short>> wave_M4_T1_S(reader, "wave_M4_T1_S");
    TTreeReaderValue<std::vector<short>> wave_M4_T2_C(reader, "wave_M4_T2_C"); TTreeReaderValue<std::vector<short>> wave_M4_T2_S(reader, "wave_M4_T2_S");
    TTreeReaderValue<std::vector<short>> wave_M4_T3_C(reader, "wave_M4_T3_C"); TTreeReaderValue<std::vector<short>> wave_M4_T3_S(reader, "wave_M4_T3_S");
    TTreeReaderValue<std::vector<short>> wave_M4_T4_C(reader, "wave_M4_T4_C"); TTreeReaderValue<std::vector<short>> wave_M4_T4_S(reader, "wave_M4_T4_S");

    TTreeReaderValue<std::vector<short>> wave_M5_T1_C(reader, "wave_M5_T1_C"); TTreeReaderValue<std::vector<short>> wave_M5_T1_S(reader, "wave_M5_T1_S");
    TTreeReaderValue<std::vector<short>> wave_M5_T2_C(reader, "wave_M5_T2_C"); TTreeReaderValue<std::vector<short>> wave_M5_T2_S(reader, "wave_M5_T2_S");
    TTreeReaderValue<std::vector<short>> wave_M5_T3_C(reader, "wave_M5_T3_C"); TTreeReaderValue<std::vector<short>> wave_M5_T3_S(reader, "wave_M5_T3_S");
    TTreeReaderValue<std::vector<short>> wave_M5_T4_C(reader, "wave_M5_T4_C"); TTreeReaderValue<std::vector<short>> wave_M5_T4_S(reader, "wave_M5_T4_S");

    TTreeReaderValue<std::vector<short>> wave_M6_T1_C(reader, "wave_M6_T1_C"); TTreeReaderValue<std::vector<short>> wave_M6_T1_S(reader, "wave_M6_T1_S");
    TTreeReaderValue<std::vector<short>> wave_M6_T2_C(reader, "wave_M6_T2_C"); TTreeReaderValue<std::vector<short>> wave_M6_T2_S(reader, "wave_M6_T2_S");
    TTreeReaderValue<std::vector<short>> wave_M6_T3_C(reader, "wave_M6_T3_C"); TTreeReaderValue<std::vector<short>> wave_M6_T3_S(reader, "wave_M6_T3_S");
    TTreeReaderValue<std::vector<short>> wave_M6_T4_C(reader, "wave_M6_T4_C"); TTreeReaderValue<std::vector<short>> wave_M6_T4_S(reader, "wave_M6_T4_S");

    TTreeReaderValue<std::vector<short>> wave_M7_T1_C(reader, "wave_M7_T1_C"); TTreeReaderValue<std::vector<short>> wave_M7_T1_S(reader, "wave_M7_T1_S");
    TTreeReaderValue<std::vector<short>> wave_M7_T2_C(reader, "wave_M7_T2_C"); TTreeReaderValue<std::vector<short>> wave_M7_T2_S(reader, "wave_M7_T2_S");
    TTreeReaderValue<std::vector<short>> wave_M7_T3_C(reader, "wave_M7_T3_C"); TTreeReaderValue<std::vector<short>> wave_M7_T3_S(reader, "wave_M7_T3_S");
    TTreeReaderValue<std::vector<short>> wave_M7_T4_C(reader, "wave_M7_T4_C"); TTreeReaderValue<std::vector<short>> wave_M7_T4_S(reader, "wave_M7_T4_S");

    TTreeReaderValue<std::vector<short>> wave_M8_T1_C(reader, "wave_M8_T1_C"); TTreeReaderValue<std::vector<short>> wave_M8_T1_S(reader, "wave_M8_T1_S");
    TTreeReaderValue<std::vector<short>> wave_M8_T2_C(reader, "wave_M8_T2_C"); TTreeReaderValue<std::vector<short>> wave_M8_T2_S(reader, "wave_M8_T2_S");
    TTreeReaderValue<std::vector<short>> wave_M8_T3_C(reader, "wave_M8_T3_C"); TTreeReaderValue<std::vector<short>> wave_M8_T3_S(reader, "wave_M8_T3_S");
    TTreeReaderValue<std::vector<short>> wave_M8_T4_C(reader, "wave_M8_T4_C"); TTreeReaderValue<std::vector<short>> wave_M8_T4_S(reader, "wave_M8_T4_S");

    TTreeReaderValue<std::vector<short>> wave_M9_T1_C(reader, "wave_M9_T1_C"); TTreeReaderValue<std::vector<short>> wave_M9_T1_S(reader, "wave_M9_T1_S");
    TTreeReaderValue<std::vector<short>> wave_M9_T2_C(reader, "wave_M9_T2_C"); TTreeReaderValue<std::vector<short>> wave_M9_T2_S(reader, "wave_M9_T2_S");
    TTreeReaderValue<std::vector<short>> wave_M9_T3_C(reader, "wave_M9_T3_C"); TTreeReaderValue<std::vector<short>> wave_M9_T3_S(reader, "wave_M9_T3_S");
    TTreeReaderValue<std::vector<short>> wave_M9_T4_C(reader, "wave_M9_T4_C"); TTreeReaderValue<std::vector<short>> wave_M9_T4_S(reader, "wave_M9_T4_S");
    
    TTreeReaderValue<std::vector<short>> wave_CC1(reader, "wave_CC1"); TTreeReaderValue<std::vector<short>> wave_CC2(reader, "wave_CC2");
    TTreeReaderValue<std::vector<short>> wave_PS(reader, "wave_PS"); TTreeReaderValue<std::vector<short>> wave_MC(reader, "wave_MC"); TTreeReaderValue<std::vector<short>> wave_TC(reader, "wave_TC");

    TTreeReaderValue<std::vector<short>> wave_DWC1_R(reader, "wave_DWC1_R"); TTreeReaderValue<std::vector<short>> wave_DWC1_L(reader, "wave_DWC1_L"); TTreeReaderValue<std::vector<short>> wave_DWC1_U(reader, "wave_DWC1_U"); TTreeReaderValue<std::vector<short>> wave_DWC1_D(reader, "wave_DWC1_D");
    TTreeReaderValue<std::vector<short>> wave_DWC2_R(reader, "wave_DWC2_R"); TTreeReaderValue<std::vector<short>> wave_DWC2_L(reader, "wave_DWC2_L"); TTreeReaderValue<std::vector<short>> wave_DWC2_U(reader, "wave_DWC2_U"); TTreeReaderValue<std::vector<short>> wave_DWC2_D(reader, "wave_DWC2_D");

    TTreeReaderValue<std::vector<short>> wave_LC2(reader, "wave_LC2"); TTreeReaderValue<std::vector<short>> wave_LC3(reader, "wave_LC3"); TTreeReaderValue<std::vector<short>> wave_LC4(reader, "wave_LC4"); 
    TTreeReaderValue<std::vector<short>> wave_LC5(reader, "wave_LC5"); TTreeReaderValue<std::vector<short>> wave_LC7(reader, "wave_LC7"); TTreeReaderValue<std::vector<short>> wave_LC8(reader, "wave_LC8"); 
    TTreeReaderValue<std::vector<short>> wave_LC9(reader, "wave_LC9"); TTreeReaderValue<std::vector<short>> wave_LC10(reader, "wave_LC10"); TTreeReaderValue<std::vector<short>> wave_LC11(reader, "wave_LC11"); 
    TTreeReaderValue<std::vector<short>> wave_LC12(reader, "wave_LC12"); TTreeReaderValue<std::vector<short>> wave_LC13(reader, "wave_LC13"); TTreeReaderValue<std::vector<short>> wave_LC14(reader, "wave_LC14");
    TTreeReaderValue<std::vector<short>> wave_LC15(reader, "wave_LC15"); TTreeReaderValue<std::vector<short>> wave_LC16(reader, "wave_LC16"); TTreeReaderValue<std::vector<short>> wave_LC19(reader, "wave_LC19"); TTreeReaderValue<std::vector<short>> wave_LC20(reader, "wave_LC20");
    TTreeReaderValue<std::vector<short>> wave_T1(reader, "wave_T1"); TTreeReaderValue<std::vector<short>> wave_T2(reader, "wave_T2"); TTreeReaderValue<std::vector<short>> wave_T1NIM(reader, "wave_T1NIM"); TTreeReaderValue<std::vector<short>> wave_T2NIM(reader, "wave_T2NIM"); TTreeReaderValue<std::vector<short>> wave_Coin(reader, "wave_Coin");
    TTreeReaderValue<std::vector<short>> wave_Coin_ref_1(reader, "wave_Coin_ref_1"); TTreeReaderValue<std::vector<short>> wave_Coin_ref_2(reader, "wave_Coin_ref_2"); TTreeReaderValue<std::vector<short>> wave_Coin_ref_3(reader, "wave_Coin_ref_3"); TTreeReaderValue<std::vector<short>> wave_Coin_ref_4(reader, "wave_Coin_ref_4");
    
    std::string outFile = "./Time/Time_Run_" + std::to_string(fRunNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();

    TTree * timeTree = new TTree("time","");

    double time_M5_T1_S, time_M5_T1_C;
    double time_M5_T2_S, time_M5_T2_C;
    double time_M5_T3_S, time_M5_T3_C;
    double time_M5_T4_S, time_M5_T4_C;

    double peak_PS, peak_MC;
    double peak_T1, peak_T2;
    double time_T1, time_T2, time_Coin_trig, time_Coin_M5_S, time_Coin_M5_C, time_Coin_inv;

    timeTree->Branch("time_M5_T1_S",&time_M5_T1_S);
    timeTree->Branch("time_M5_T1_C",&time_M5_T1_C);
    timeTree->Branch("time_M5_T2_S",&time_M5_T2_S);
    timeTree->Branch("time_M5_T2_C",&time_M5_T2_C);
    timeTree->Branch("time_M5_T3_S",&time_M5_T3_S);
    timeTree->Branch("time_M5_T3_C",&time_M5_T3_C);
    timeTree->Branch("time_M5_T4_S",&time_M5_T4_S);
    timeTree->Branch("time_M5_T4_C",&time_M5_T4_C);

    timeTree->Branch("peak_PS",&peak_PS);
    timeTree->Branch("peak_MC",&peak_MC);
    
    timeTree->Branch("peak_T1",&peak_T1);
    timeTree->Branch("peak_T2",&peak_T2);
    
    timeTree->Branch("time_T1",&time_T1);
    timeTree->Branch("time_T2",&time_T2);
    timeTree->Branch("time_Coin_trig",&time_Coin_trig);
    timeTree->Branch("time_Coin_M5_S",&time_Coin_M5_S);
    timeTree->Branch("time_Coin_M5_C",&time_Coin_M5_C);
    timeTree->Branch("time_Coin_inv",&time_Coin_inv);

    // MID: 3-7: PMT modules, MID 9: LC, MID 10: Aux(CC1, CC2, PS, TC, MC), MID 12: Triggers (T1, T2, T1NIM, T2NIM, Coin), MID 14-17: MCP micro, MID 18: DWC
    // TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, "/Volumes/Macintosh HD-1/Users/yhep/scratch/YUdaq", {3, 4, 5, 6, 7, 9, 10, 12, 18});

    // Set Maximum event
    Long64_t totalEntries = reader.GetEntries();
    if (fMaxEvent == -1 || fMaxEvent > totalEntries)
        fMaxEvent = totalEntries;

    // Evt Loop
    for (int iEvt = 0; iEvt < fMaxEvent; iEvt++) {
        printProgress(iEvt, fMaxEvent);
        reader.SetEntry(iEvt);
        std::vector<float> DWC1_time;
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(*wave_DWC1_R, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(*wave_DWC1_L, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(*wave_DWC1_U, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(*wave_DWC1_D, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        
        std::vector<float> DWC2_time;
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(*wave_DWC2_R, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(*wave_DWC2_L, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(*wave_DWC2_U, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(*wave_DWC2_D, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2
        
        std::vector<float> DWC1_corrected_pos = getDWC1position(DWC1_time, DWC1_offset); // DWC1 X, Y
        std::vector<float> DWC2_corrected_pos = getDWC2position(DWC2_time, DWC2_offset); // DWC2 X, Y
                
	std::vector<double> wave_pedcor_M5_T1_S = pedcorwave(*wave_M5_T1_S,100);
	std::vector<double> wave_pedcor_M5_T1_C = pedcorwave(*wave_M5_T1_C,100);
	std::vector<double> wave_pedcor_M5_T2_S = pedcorwave(*wave_M5_T2_S,100);
	std::vector<double> wave_pedcor_M5_T2_C = pedcorwave(*wave_M5_T2_C,100);
	std::vector<double> wave_pedcor_M5_T3_S = pedcorwave(*wave_M5_T3_S,100);
	std::vector<double> wave_pedcor_M5_T3_C = pedcorwave(*wave_M5_T3_C,100);
	std::vector<double> wave_pedcor_M5_T4_S = pedcorwave(*wave_M5_T4_S,100);
	std::vector<double> wave_pedcor_M5_T4_C = pedcorwave(*wave_M5_T4_C,100);
	

	std::vector<double> wave_pedcor_T1 = pedcorwave(*wave_T1,100);
	std::vector<double> wave_pedcor_T2 = pedcorwave(*wave_T2,100);
	std::vector<double> wave_pedcor_Coin_trig = pedcorwave(*wave_Coin_ref_4,100);
	std::vector<double> wave_pedcor_Coin_M5_S = pedcorwave(*wave_Coin_ref_1,100);
	std::vector<double> wave_pedcor_Coin_M5_C = pedcorwave(*wave_Coin_ref_2,100);
	std::vector<double> wave_pedcor_Coin_inv = pedcorwave(*wave_Coin_ref_3,100);

	time_M5_T1_S = getTime_frompeak(wave_pedcor_M5_T1_S,0.4);
	time_M5_T1_C = getTime_frompeak(wave_pedcor_M5_T1_C,0.4);
	time_M5_T2_S = getTime_frompeak(wave_pedcor_M5_T2_S,0.4);
	time_M5_T2_C = getTime_frompeak(wave_pedcor_M5_T2_C,0.4);
	time_M5_T3_S = getTime_frompeak(wave_pedcor_M5_T3_S,0.4);
	time_M5_T3_C = getTime_frompeak(wave_pedcor_M5_T3_C,0.4);
	time_M5_T4_S = getTime_frompeak(wave_pedcor_M5_T4_S,0.4);
	time_M5_T4_C = getTime_frompeak(wave_pedcor_M5_T4_C,0.4);
	
	time_T1 = getTime_frompeak(wave_pedcor_T1,0.4);
	time_T2 = getTime_frompeak(wave_pedcor_T2,0.4);
	time_Coin_trig = getTime_frompeak(wave_pedcor_Coin_trig,0.4);
	time_Coin_M5_S = getTime_frompeak(wave_pedcor_Coin_M5_S,0.4);
	time_Coin_M5_C = getTime_frompeak(wave_pedcor_Coin_M5_C,0.4);
	time_Coin_inv = getTime_frompeak(wave_pedcor_Coin_inv,0.4);

        peak_PS = GetPeak(*wave_PS, PS_first, PS_last); // PeakADC
        peak_MC = GetPeak(*wave_MC, MC_first, MC_last); // PeakADC
	///std::cout<<time_T2<<" | "<<time_M5_T2_S<<" | "<<time_M5_T2_C<<std::endl;
	peak_T1 = GetPeak(*wave_T1,1,1000);
	peak_T2 = GetPeak(*wave_T2,1,1000);
        hist_DWC1_pos_corrected  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_corrected  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1)); 
        hist_DWC_x_corr_corrected->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_corrected->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));
        if ( !(dwcCorrelationCut(DWC1_corrected_pos, DWC2_corrected_pos, cut_DWC)) ) continue;
	//if ( std::abs(DWC1_corrected_pos.at(0))>5 || std::abs(DWC1_corrected_pos.at(1))>5) continue;
        //if ( std::abs(DWC2_corrected_pos.at(0))>5 || std::abs(DWC2_corrected_pos.at(1))>5) continue;

        hist_DWC1_pos_after  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_after  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1));
        hist_DWC_x_corr_after->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_after->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));
	timeTree->Fill();
    }

    timeTree->Write();
    hist_DWC1_pos_corrected  ->Write();
    hist_DWC2_pos_corrected  ->Write();
    hist_DWC_x_corr_corrected->Write();
    hist_DWC_y_corr_corrected->Write();
    
    hist_DWC1_pos_after  ->Write();
    hist_DWC2_pos_after  ->Write();
    hist_DWC_x_corr_after->Write();
    hist_DWC_y_corr_after->Write();
    outputRoot->Close();
}

