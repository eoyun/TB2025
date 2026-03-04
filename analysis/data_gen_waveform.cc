#include "TBread.h"
#include "TButility.h"

#include <filesystem>
#include <chrono>
#include <numeric>
#include <vector>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TF1.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "function.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    
    int C_first = 240; // Module integration range
    int C_last = 450;  // Module integration range

    int S_first = 260; // Module integration range
    int S_last = 550;  // Module integration range

    int M5T3C_first = 240; // M5T3 integration range
    int M5T3C_last =  440; // M5T3 integration range

    int M5T3S_first = 245; // M5T3 integration range
    int M5T3S_last =  500; // M5T3 integration range
    
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

    // setup for prompt analysis
    // it could be like
    int fRunNum = std::stoi(argv[1]);
    int fMaxEvent = std::stoi(argv[2]);
    int fMaxFile = -1;
    
    FILE * fp;

    fs::path dir("./data_waveform");   
    if (!(fs::exists(dir))) fs::create_directory(dir);
        
    char filename[200];
    sprintf(filename,"./data_waveform/run_%d.csv",fRunNum);
    fp = fopen(filename,"wt");
    // initialize the utility class
    TButility util = TButility();
    util.LoadMapping("../mapping/mapping_TB2025_v1.root");
    
    TFile* f_DWC = TFile::Open((TString)("./DWC/DWC_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    TH2D* h_DWC1_pos   = (TH2D*) f_DWC->Get("dwc1_pos");
    TH2D* h_DWC2_pos   = (TH2D*) f_DWC->Get("dwc2_pos");
    std::vector<float> DWC1_offset = getDWCoffset(h_DWC1_pos); // DWC1_offset.at(0) == X, DWC1_offset.at(1) == Y
    std::vector<float> DWC2_offset = getDWCoffset(h_DWC2_pos);
    
    // Get Ntuple
    //TFile* fNtuple = TFile::Open((TString)("/Users/yhep/DRC/TB2025/analysis/SW/Prompt_ntuple/Prompt_ntuple_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    TFile* fNtuple = TFile::Open((TString)("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_PromptAnalysis/Prompt_ntuple_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    // Create TTreeReader
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

    // Set Maximum event
    Long64_t totalEntries = reader.GetEntries();
    if (fMaxEvent == -1 || fMaxEvent > totalEntries)
        fMaxEvent = totalEntries;
    
    for (int iEvt = 0; iEvt < fMaxEvent; iEvt++) {
        printProgress(iEvt, fMaxEvent);
        // Load event using TTreeReader
        reader.SetEntry(iEvt);
        
        float signal_CC1 = GetPeak(*wave_CC1, CC1_peak_first, CC1_peak_last); // PeakADC
        float signal_CC2 = GetPeak(*wave_CC2, CC2_peak_first, CC2_peak_last); // PeakADC
        
        float signal_PS = GetPeak(*wave_PS, PS_first, PS_last); // PeakADC
        float signal_MC = GetPeak(*wave_MC, MC_first, MC_last); // PeakADC
        float signal_TC = GetPeak(*wave_TC, TC_first, TC_last); // PeakADC
        
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
                
        hist_CC1->Fill(signal_CC1);
        hist_CC2->Fill(signal_CC2);
        
        hist_PS->Fill(signal_PS);
        hist_MC->Fill(signal_MC);
        hist_TC->Fill(signal_TC);
        
        hist_DWC1_pos_corrected  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_corrected  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1)); 
        hist_DWC_x_corr_corrected->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_corrected->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));
        
        // Event selection here!!
        // Require C1, C2 to have intADC larger than cut
        // if ( !((cut_CC1 < signal_CC1) && (cut_CC2 < signal_CC2)) ) continue;
        if ( !(dwcCorrelationCut(DWC1_corrected_pos, DWC2_corrected_pos, cut_DWC)) ) continue;
        if ( signal_PS < cut_PS1 || signal_PS > cut_PS2 ) continue; // Select above 3 mip peak
        if ( signal_MC < cut_MC ) continue; // Select only pedestals
        
	for (int i=1;i<1000;i++){
	    fprintf(fp,"%d,",(*wave_M5_T2_C).at(i));	
	    
	}
	fprintf(fp,"%d\n",(*wave_M5_T2_C).at(1000));


        hist_CC1_after->Fill(signal_CC1);
        hist_CC2_after->Fill(signal_CC2);
        
        hist_PS_after->Fill(signal_PS);
        hist_MC_after->Fill(signal_MC);
        hist_TC_after->Fill(signal_TC);
        
        hist_DWC1_pos_after  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_after  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1));
        hist_DWC_x_corr_after->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_after->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));
    }
    
    // Output file
    std::string outFile = "./data_waveform/DRC_Run_" + std::to_string(fRunNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
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
    
    // Hist after PID
    hist_CC1_after->Write();
    hist_CC2_after->Write();
    
    hist_PS_after->Write();
    hist_MC_after->Write();
    hist_TC_after->Write();
    
    hist_DWC1_pos_after  ->Write();
    hist_DWC2_pos_after  ->Write();
    hist_DWC_x_corr_after->Write();
    hist_DWC_y_corr_after->Write();
    outputRoot->Close();
    
    fclose(fp);
    return 0;
}
