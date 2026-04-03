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

#include "function.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    
    int C_first = 250; // Module integration range
    int C_last = 450;  // Module integration range

    int S_first = 260; // Module integration range
    int S_last = 500;  // Module integration range

    int M5T3C_first = 230; // M5T3 integration range
    int M5T3C_last =  440; // M5T3 integration range

    int M5T3S_first = 240; // M5T3 integration range
    int M5T3S_last =  485; // M5T3 integration range
    
    int CC1_peak_first = 650; // Peak search range
    int CC1_peak_last  = 750; // Peak search range
    
    int CC2_peak_first = 620; // Peak search range
    int CC2_peak_last  = 850; // Peak search range
    
    int PS_first = 200; // PS integration range
    int PS_last  = 350; // PS integration range
    
    int MC_first = 650; // MC integration range
    int MC_last  = 850; // MC integration range
    
    int TC_first = 300; // TC peak search range
    int TC_last  = 450; // TC peak search range
    
    int LC_first = 400;    // LC integration range  
    int LC_last  = 600; // LC integration range
    
    // cuts
    // float cut_CC1  = 100.;  // PID cut for CC1 (PeakADC)
    // float cut_CC2  = 200.;  // PID cut for CC2 (PeakADC)
    float cut_CC1  = 60.;   // PID cut for CC1 (PeakADC)
    float cut_CC2  = 100.;  // PID cut for CC2 (PeakADC)
    
    // float cut_PS = 20000.; // PID cut for PS (IntADC)
    // float cut_PS = 10000.; // PID cut for PS (IntADC)
    float cut_PS = 600.; // PID cut for PS (PeakADC)
    float cut_MC = 35.;   // PID cut for MC (PeakADC)
    
    float cut_DWC = 4; // Beam geometry cut for DWC
  
    // setup for prompt analysis
    // it could be like
    int fRunNum = std::stoi(argv[1]);
    int fMaxEvent = std::stoi(argv[2]);
    int fMaxFile = -1;

    // Dataset switch (default: TB2025 mapping)
    // usage example: ./calib_DRC_ADC_mode_example <run> <maxEvent> [isKEK] [correctionMode] [correctionCSV]
    bool isKEK = false;
    if (argc > 3)
        isKEK = (std::stoi(argv[3]) != 0);

    const std::string mappingPath = isKEK ? "../mapping/mapping_KEK_v1.root" : "../mapping/mapping_TB2025_v1.root";
    const std::string correctionMode = (argc > 4) ? argv[4] : "PatchBased";
    const std::string correctionCSVPath = (argc > 5)
                                              ? argv[5]
                                              : (isKEK ? "../kek_mean.csv" : "../tb2025_updated.csv");
    const std::string dataPath = isKEK ? "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/KEK_DRC_TB_Data/"
                                       : "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_Data/";
    
    fs::path dir("./Closure");   
    if (!(fs::exists(dir))) fs::create_directory(dir);
        
    // initialize the utility class
    TButility util = TButility();
    util.LoadMapping(mappingPath);
    TBwaveform::SetCorrectionMode(correctionMode);
    if (correctionMode == "PatchBased" || correctionMode == "ADCcorrection")
    {
        TBwaveform::SetCorrectionCSVPath(correctionCSVPath);
    }
    else
    {
        // Entire mode expects keys like Mx_Tx_S/C_mean,
        // woAVG mode expects keys like Mx_Tx_S/C.
        // Both are loaded from the same CSV file in this example.
        TBwaveform::SetCorrectionCSVPathForMode("ADCcorrectionEntire", correctionCSVPath);
        TBwaveform::SetCorrectionCSVPathForMode("ADCcorrectionwoAVG", correctionCSVPath);
    }
    
    TFile* f_DWC = TFile::Open((TString)("./DWC/DWC_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    TH2D* h_DWC1_pos   = (TH2D*) f_DWC->Get("dwc1_pos");
    TH2D* h_DWC2_pos   = (TH2D*) f_DWC->Get("dwc2_pos");
    std::vector<float> DWC1_offset = getDWCoffset(h_DWC1_pos); // DWC1_offset.at(0) == X, DWC1_offset.at(1) == Y
    std::vector<float> DWC2_offset = getDWCoffset(h_DWC2_pos);
    
    // prepare CIDs that we want to use (CID = Channel ID)
    // 3x3 module CIDs
    std::vector<TBcid> cid_M1_C; std::vector<TBcid> cid_M1_S;
    std::vector<TBcid> cid_M2_C; std::vector<TBcid> cid_M2_S;
    std::vector<TBcid> cid_M3_C; std::vector<TBcid> cid_M3_S;
    std::vector<TBcid> cid_M4_C; std::vector<TBcid> cid_M4_S;
    std::vector<TBcid> cid_M5_C; std::vector<TBcid> cid_M5_S;
    std::vector<TBcid> cid_M6_C; std::vector<TBcid> cid_M6_S;
    std::vector<TBcid> cid_M7_C; std::vector<TBcid> cid_M7_S;
    std::vector<TBcid> cid_M8_C; std::vector<TBcid> cid_M8_S;
    std::vector<TBcid> cid_M9_C; std::vector<TBcid> cid_M9_S;
    
    auto getDRCName = [&](int module, int tower, const char* type) -> TString {
        if (isKEK)
            return Form("T%d-%s", tower, type);
        return Form("M%d-T%d-%s", module, tower, type);
    };

    for(int tower = 1; tower <= 4; tower++) {
        cid_M1_C.emplace_back(util.GetCID(getDRCName(1, tower, "C")));
        cid_M2_C.emplace_back(util.GetCID(getDRCName(2, tower, "C")));
        cid_M3_C.emplace_back(util.GetCID(getDRCName(3, tower, "C")));
        cid_M4_C.emplace_back(util.GetCID(getDRCName(4, tower, "C")));
        cid_M5_C.emplace_back(util.GetCID(getDRCName(5, tower, "C")));
        cid_M6_C.emplace_back(util.GetCID(getDRCName(6, tower, "C")));
        cid_M7_C.emplace_back(util.GetCID(getDRCName(7, tower, "C")));
        cid_M8_C.emplace_back(util.GetCID(getDRCName(8, tower, "C")));
        cid_M9_C.emplace_back(util.GetCID(getDRCName(9, tower, "C")));

        cid_M1_S.emplace_back(util.GetCID(getDRCName(1, tower, "S")));
        cid_M2_S.emplace_back(util.GetCID(getDRCName(2, tower, "S")));
        cid_M3_S.emplace_back(util.GetCID(getDRCName(3, tower, "S")));
        cid_M4_S.emplace_back(util.GetCID(getDRCName(4, tower, "S")));
        cid_M5_S.emplace_back(util.GetCID(getDRCName(5, tower, "S")));
        cid_M6_S.emplace_back(util.GetCID(getDRCName(6, tower, "S")));
        cid_M7_S.emplace_back(util.GetCID(getDRCName(7, tower, "S")));
        cid_M8_S.emplace_back(util.GetCID(getDRCName(8, tower, "S")));
        cid_M9_S.emplace_back(util.GetCID(getDRCName(9, tower, "S")));
    }
    
    // Aux. detectors
    
    TBcid cid_PS = util.GetCID("PS"); // Preshower
    TBcid cid_MC = util.GetCID("MC"); // Muon counter
    TBcid cid_TC = util.GetCID("TC"); // Tail catcher
    
    TBcid cid_DWC1_R = util.GetCID("DWC1R"); // DWC 1
    TBcid cid_DWC1_L = util.GetCID("DWC1L"); // DWC 1
    TBcid cid_DWC1_U = util.GetCID("DWC1U"); // DWC 1
    TBcid cid_DWC1_D = util.GetCID("DWC1D"); // DWC 1
    
    TBcid cid_DWC2_R = util.GetCID("DWC2R"); // DWC 2
    TBcid cid_DWC2_L = util.GetCID("DWC2L"); // DWC 2
    TBcid cid_DWC2_U = util.GetCID("DWC2U"); // DWC 2
    TBcid cid_DWC2_D = util.GetCID("DWC2D"); // DWC 2
    
    TH2D* M1_T1_S_hist = new TH2D("M1_T1_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M1_T2_S_hist = new TH2D("M1_T2_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M1_T3_S_hist = new TH2D("M1_T3_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M1_T4_S_hist = new TH2D("M1_T4_S_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M2_T1_S_hist = new TH2D("M2_T1_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M2_T2_S_hist = new TH2D("M2_T2_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M2_T3_S_hist = new TH2D("M2_T3_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M2_T4_S_hist = new TH2D("M2_T4_S_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M3_T1_S_hist = new TH2D("M3_T1_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M3_T2_S_hist = new TH2D("M3_T2_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M3_T3_S_hist = new TH2D("M3_T3_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M3_T4_S_hist = new TH2D("M3_T4_S_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M4_T1_S_hist = new TH2D("M4_T1_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M4_T2_S_hist = new TH2D("M4_T2_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M4_T3_S_hist = new TH2D("M4_T3_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M4_T4_S_hist = new TH2D("M4_T4_S_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M5_T1_S_hist = new TH2D("M5_T1_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M5_T2_S_hist = new TH2D("M5_T2_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M5_T3_S_hist = new TH2D("M5_T3_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M5_T4_S_hist = new TH2D("M5_T4_S_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M6_T1_S_hist = new TH2D("M6_T1_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M6_T2_S_hist = new TH2D("M6_T2_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M6_T3_S_hist = new TH2D("M6_T3_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M6_T4_S_hist = new TH2D("M6_T4_S_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M7_T1_S_hist = new TH2D("M7_T1_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M7_T2_S_hist = new TH2D("M7_T2_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M7_T3_S_hist = new TH2D("M7_T3_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M7_T4_S_hist = new TH2D("M7_T4_S_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M8_T1_S_hist = new TH2D("M8_T1_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M8_T2_S_hist = new TH2D("M8_T2_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M8_T3_S_hist = new TH2D("M8_T3_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M8_T4_S_hist = new TH2D("M8_T4_S_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M9_T1_S_hist = new TH2D("M9_T1_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M9_T2_S_hist = new TH2D("M9_T2_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M9_T3_S_hist = new TH2D("M9_T3_S_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M9_T4_S_hist = new TH2D("M9_T4_S_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M1_T1_C_hist = new TH2D("M1_T1_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M1_T2_C_hist = new TH2D("M1_T2_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M1_T3_C_hist = new TH2D("M1_T3_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M1_T4_C_hist = new TH2D("M1_T4_C_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M2_T1_C_hist = new TH2D("M2_T1_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M2_T2_C_hist = new TH2D("M2_T2_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M2_T3_C_hist = new TH2D("M2_T3_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M2_T4_C_hist = new TH2D("M2_T4_C_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M3_T1_C_hist = new TH2D("M3_T1_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M3_T2_C_hist = new TH2D("M3_T2_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M3_T3_C_hist = new TH2D("M3_T3_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M3_T4_C_hist = new TH2D("M3_T4_C_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M4_T1_C_hist = new TH2D("M4_T1_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M4_T2_C_hist = new TH2D("M4_T2_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M4_T3_C_hist = new TH2D("M4_T3_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M4_T4_C_hist = new TH2D("M4_T4_C_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M5_T1_C_hist = new TH2D("M5_T1_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M5_T2_C_hist = new TH2D("M5_T2_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M5_T3_C_hist = new TH2D("M5_T3_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M5_T4_C_hist = new TH2D("M5_T4_C_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M6_T1_C_hist = new TH2D("M6_T1_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M6_T2_C_hist = new TH2D("M6_T2_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M6_T3_C_hist = new TH2D("M6_T3_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M6_T4_C_hist = new TH2D("M6_T4_C_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M7_T1_C_hist = new TH2D("M7_T1_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M7_T2_C_hist = new TH2D("M7_T2_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M7_T3_C_hist = new TH2D("M7_T3_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M7_T4_C_hist = new TH2D("M7_T4_C_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M8_T1_C_hist = new TH2D("M8_T1_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M8_T2_C_hist = new TH2D("M8_T2_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M8_T3_C_hist = new TH2D("M8_T3_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M8_T4_C_hist = new TH2D("M8_T4_C_hist","",1024,0,1024,3600,-100,3500);
    
    TH2D* M9_T1_C_hist = new TH2D("M9_T1_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M9_T2_C_hist = new TH2D("M9_T2_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M9_T3_C_hist = new TH2D("M9_T3_C_hist","",1024,0,1024,3600,-100,3500);
    TH2D* M9_T4_C_hist = new TH2D("M9_T4_C_hist","",1024,0,1024,3600,-100,3500);
    
    // TH1F* hist_PS = new TH1F("PS" , ";intADC;nEvents", 320, -20000, 300000);
    // TH1F* hist_MC = new TH1F("MC" , ";intADC;nEvents", 320, -20000, 300000);
    // TH1F* hist_TC = new TH1F("TC" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH2D* hist_DWC1_pos_corrected   = new TH2D("DWC1_pos_corrected",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_corrected   = new TH2D("DWC2_pos_corrected",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_corrected = new TH2D("DWC_x_corr_corrected", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_corrected = new TH2D("DWC_y_corr_corrected", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);
    
    // Histograms after PID
    
    TH2D* hist_DWC1_pos_after   = new TH2D("DWC1_pos_after",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_after   = new TH2D("DWC2_pos_after",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_after = new TH2D("DWC_x_corr_after", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_after = new TH2D("DWC_y_corr_after", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);
    
    // MID: 3-7: PMT modules, MID 9: LC, MID 10: Aux(CC1, CC2, PS, TC, MC), MID 12: Triggers (T1, T2, T1NIM, T2NIM, Coin), MID 14-17: MCP micro, MID 18: DWC
    // TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, "/Volumes/Macintosh HD-1/Users/yhep/scratch/YUdaq", {3, 4, 5, 6, 7, 9, 10, 12, 18});
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, dataPath, {3, 4, 5, 6, 7, 9, 10, 12, 18});
    
    // Set Maximum event
    if (fMaxEvent == -1 || fMaxEvent > readerWave.GetMaxEvent())
        fMaxEvent = readerWave.GetMaxEvent();
    
    for (int iEvt = 0; iEvt < fMaxEvent; iEvt++) {
        printProgress(iEvt, fMaxEvent);
        // Load event
        TBevt<TBwaveform> anEvt = readerWave.GetAnEvent();
        
        // Get data of certain channel we want to use
        // Get pedestal corrected waveform
        // TBwaveform::pedcorrectedWaveform(float ped): when using external (e.g. run pedestal)
        // TBwaveform::pedcorrectedWaveform(): when using event-by-event pedestal (ped = average of first 100 bin except 0th bin)
        std::vector< std::vector<float> > wave_M1_C; std::vector< std::vector<float> > wave_M1_S;
        std::vector< std::vector<float> > wave_M2_C; std::vector< std::vector<float> > wave_M2_S;
        std::vector< std::vector<float> > wave_M3_C; std::vector< std::vector<float> > wave_M3_S;
        std::vector< std::vector<float> > wave_M4_C; std::vector< std::vector<float> > wave_M4_S;
        std::vector< std::vector<float> > wave_M5_C; std::vector< std::vector<float> > wave_M5_S;
        std::vector< std::vector<float> > wave_M6_C; std::vector< std::vector<float> > wave_M6_S;
        std::vector< std::vector<float> > wave_M7_C; std::vector< std::vector<float> > wave_M7_S;
        std::vector< std::vector<float> > wave_M8_C; std::vector< std::vector<float> > wave_M8_S;
        std::vector< std::vector<float> > wave_M9_C; std::vector< std::vector<float> > wave_M9_S;
        for(int tower = 0; tower < 4; tower++) {
            wave_M1_C.emplace_back( anEvt.GetData(cid_M1_C.at(tower)).pedcorrectedWaveform() ); 
            wave_M2_C.emplace_back( anEvt.GetData(cid_M2_C.at(tower)).pedcorrectedWaveform() ); 
            wave_M3_C.emplace_back( anEvt.GetData(cid_M3_C.at(tower)).pedcorrectedWaveform() ); 
            wave_M4_C.emplace_back( anEvt.GetData(cid_M4_C.at(tower)).pedcorrectedWaveform() ); 
            wave_M5_C.emplace_back( anEvt.GetData(cid_M5_C.at(tower)).pedcorrectedWaveform() ); 
            wave_M6_C.emplace_back( anEvt.GetData(cid_M6_C.at(tower)).pedcorrectedWaveform() ); 
            wave_M7_C.emplace_back( anEvt.GetData(cid_M7_C.at(tower)).pedcorrectedWaveform() ); 
            wave_M8_C.emplace_back( anEvt.GetData(cid_M8_C.at(tower)).pedcorrectedWaveform() ); 
            wave_M9_C.emplace_back( anEvt.GetData(cid_M9_C.at(tower)).pedcorrectedWaveform() ); 
            
            wave_M1_S.emplace_back( anEvt.GetData(cid_M1_S.at(tower)).pedcorrectedWaveform() ); 
            wave_M2_S.emplace_back( anEvt.GetData(cid_M2_S.at(tower)).pedcorrectedWaveform() ); 
            wave_M3_S.emplace_back( anEvt.GetData(cid_M3_S.at(tower)).pedcorrectedWaveform() ); 
            wave_M4_S.emplace_back( anEvt.GetData(cid_M4_S.at(tower)).pedcorrectedWaveform() ); 
            wave_M5_S.emplace_back( anEvt.GetData(cid_M5_S.at(tower)).pedcorrectedWaveform() ); 
            wave_M6_S.emplace_back( anEvt.GetData(cid_M6_S.at(tower)).pedcorrectedWaveform() ); 
            wave_M7_S.emplace_back( anEvt.GetData(cid_M7_S.at(tower)).pedcorrectedWaveform() ); 
            wave_M8_S.emplace_back( anEvt.GetData(cid_M8_S.at(tower)).pedcorrectedWaveform() ); 
            wave_M9_S.emplace_back( anEvt.GetData(cid_M9_S.at(tower)).pedcorrectedWaveform() ); 
        } 
        
        // Get waveform for DWCs
        std::vector<short> wave_DWC1_R = (anEvt.GetData(cid_DWC1_R)).waveform();
        std::vector<short> wave_DWC1_L = (anEvt.GetData(cid_DWC1_L)).waveform();
        std::vector<short> wave_DWC1_U = (anEvt.GetData(cid_DWC1_U)).waveform();
        std::vector<short> wave_DWC1_D = (anEvt.GetData(cid_DWC1_D)).waveform();
        std::vector<short> wave_DWC2_R = (anEvt.GetData(cid_DWC2_R)).waveform();
        std::vector<short> wave_DWC2_L = (anEvt.GetData(cid_DWC2_L)).waveform();
        std::vector<short> wave_DWC2_U = (anEvt.GetData(cid_DWC2_U)).waveform();
        std::vector<short> wave_DWC2_D = (anEvt.GetData(cid_DWC2_D)).waveform();
        
        std::vector<float> DWC1_time;
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(wave_DWC1_R, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(wave_DWC1_L, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(wave_DWC1_U, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(wave_DWC1_D, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 1
        
        std::vector<float> DWC2_time;
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(wave_DWC2_R, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(wave_DWC2_L, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(wave_DWC2_U, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(wave_DWC2_D, 0.4, 1, 1000)); // Get 40% leading Edge Time for DWC 2
        
        std::vector<float> DWC1_corrected_pos = getDWC1position(DWC1_time, DWC1_offset); // DWC1 X, Y
        std::vector<float> DWC2_corrected_pos = getDWC2position(DWC2_time, DWC2_offset); // DWC2 X, Y
        
        
        hist_DWC1_pos_corrected  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_corrected  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1)); 
        hist_DWC_x_corr_corrected->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_corrected->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));
        
        // Event selection here!!
        // Require C1, C2 to have intADC larger than cut
        // if ( !((cut_CC1 < signal_CC1) && (cut_CC2 < signal_CC2)) ) continue;
        if ( !(dwcCorrelationCut(DWC1_corrected_pos, DWC2_corrected_pos, cut_DWC)) ) continue;
        
        
        hist_DWC1_pos_after  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_after  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1));
        hist_DWC_x_corr_after->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_after->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));

        for (int i = 1; i<951; i++){
          M1_T1_S_hist->Fill(i,wave_M1_S.at(0).at(i));
          M1_T2_S_hist->Fill(i,wave_M1_S.at(1).at(i));
          M1_T3_S_hist->Fill(i,wave_M1_S.at(2).at(i));
          M1_T4_S_hist->Fill(i,wave_M1_S.at(3).at(i));
          
          M2_T1_S_hist->Fill(i,wave_M2_S.at(0).at(i));
          M2_T2_S_hist->Fill(i,wave_M2_S.at(1).at(i));
          M2_T3_S_hist->Fill(i,wave_M2_S.at(2).at(i));
          M2_T4_S_hist->Fill(i,wave_M2_S.at(3).at(i));
          
          M3_T1_S_hist->Fill(i,wave_M3_S.at(0).at(i));
          M3_T2_S_hist->Fill(i,wave_M3_S.at(1).at(i));
          M3_T3_S_hist->Fill(i,wave_M3_S.at(2).at(i));
          M3_T4_S_hist->Fill(i,wave_M3_S.at(3).at(i));
          
          M4_T1_S_hist->Fill(i,wave_M4_S.at(0).at(i));
          M4_T2_S_hist->Fill(i,wave_M4_S.at(1).at(i));
          M4_T3_S_hist->Fill(i,wave_M4_S.at(2).at(i));
          M4_T4_S_hist->Fill(i,wave_M4_S.at(3).at(i));
          
          M5_T1_S_hist->Fill(i,wave_M5_S.at(0).at(i));
          M5_T2_S_hist->Fill(i,wave_M5_S.at(1).at(i));
          M5_T3_S_hist->Fill(i,wave_M5_S.at(2).at(i));
          M5_T4_S_hist->Fill(i,wave_M5_S.at(3).at(i));
          
          M6_T1_S_hist->Fill(i,wave_M6_S.at(0).at(i));
          M6_T2_S_hist->Fill(i,wave_M6_S.at(1).at(i));
          M6_T3_S_hist->Fill(i,wave_M6_S.at(2).at(i));
          M6_T4_S_hist->Fill(i,wave_M6_S.at(3).at(i));
          
          M7_T1_S_hist->Fill(i,wave_M7_S.at(0).at(i));
          M7_T2_S_hist->Fill(i,wave_M7_S.at(1).at(i));
          M7_T3_S_hist->Fill(i,wave_M7_S.at(2).at(i));
          M7_T4_S_hist->Fill(i,wave_M7_S.at(3).at(i));
          
          M8_T1_S_hist->Fill(i,wave_M8_S.at(0).at(i));
          M8_T2_S_hist->Fill(i,wave_M8_S.at(1).at(i));
          M8_T3_S_hist->Fill(i,wave_M8_S.at(2).at(i));
          M8_T4_S_hist->Fill(i,wave_M8_S.at(3).at(i));
          
          M9_T1_S_hist->Fill(i,wave_M9_S.at(0).at(i));
          M9_T2_S_hist->Fill(i,wave_M9_S.at(1).at(i));
          M9_T3_S_hist->Fill(i,wave_M9_S.at(2).at(i));
          M9_T4_S_hist->Fill(i,wave_M9_S.at(3).at(i));
          
          M1_T1_C_hist->Fill(i,wave_M1_C.at(0).at(i));
          M1_T2_C_hist->Fill(i,wave_M1_C.at(1).at(i));
          M1_T3_C_hist->Fill(i,wave_M1_C.at(2).at(i));
          M1_T4_C_hist->Fill(i,wave_M1_C.at(3).at(i));
          
          M2_T1_C_hist->Fill(i,wave_M2_C.at(0).at(i));
          M2_T2_C_hist->Fill(i,wave_M2_C.at(1).at(i));
          M2_T3_C_hist->Fill(i,wave_M2_C.at(2).at(i));
          M2_T4_C_hist->Fill(i,wave_M2_C.at(3).at(i));
          
          M3_T1_C_hist->Fill(i,wave_M3_C.at(0).at(i));
          M3_T2_C_hist->Fill(i,wave_M3_C.at(1).at(i));
          M3_T3_C_hist->Fill(i,wave_M3_C.at(2).at(i));
          M3_T4_C_hist->Fill(i,wave_M3_C.at(3).at(i));
          
          M4_T1_C_hist->Fill(i,wave_M4_C.at(0).at(i));
          M4_T2_C_hist->Fill(i,wave_M4_C.at(1).at(i));
          M4_T3_C_hist->Fill(i,wave_M4_C.at(2).at(i));
          M4_T4_C_hist->Fill(i,wave_M4_C.at(3).at(i));
          
          M5_T1_C_hist->Fill(i,wave_M5_C.at(0).at(i));
          M5_T2_C_hist->Fill(i,wave_M5_C.at(1).at(i));
          M5_T3_C_hist->Fill(i,wave_M5_C.at(2).at(i));
          M5_T4_C_hist->Fill(i,wave_M5_C.at(3).at(i));
          
          M6_T1_C_hist->Fill(i,wave_M6_C.at(0).at(i));
          M6_T2_C_hist->Fill(i,wave_M6_C.at(1).at(i));
          M6_T3_C_hist->Fill(i,wave_M6_C.at(2).at(i));
          M6_T4_C_hist->Fill(i,wave_M6_C.at(3).at(i));
          
          M7_T1_C_hist->Fill(i,wave_M7_C.at(0).at(i));
          M7_T2_C_hist->Fill(i,wave_M7_C.at(1).at(i));
          M7_T3_C_hist->Fill(i,wave_M7_C.at(2).at(i));
          M7_T4_C_hist->Fill(i,wave_M7_C.at(3).at(i));
          
          M8_T1_C_hist->Fill(i,wave_M8_C.at(0).at(i));
          M8_T2_C_hist->Fill(i,wave_M8_C.at(1).at(i));
          M8_T3_C_hist->Fill(i,wave_M8_C.at(2).at(i));
          M8_T4_C_hist->Fill(i,wave_M8_C.at(3).at(i));
          
          M9_T1_C_hist->Fill(i,wave_M9_C.at(0).at(i));
          M9_T2_C_hist->Fill(i,wave_M9_C.at(1).at(i));
          M9_T3_C_hist->Fill(i,wave_M9_C.at(2).at(i));
          M9_T4_C_hist->Fill(i,wave_M9_C.at(3).at(i));
	
	}

    }
    
    // Output file
    std::string outFile = "./Closure/NoClosure_Run_" + std::to_string(fRunNum) +"_" +correctionMode +".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();
    
    hist_DWC1_pos_corrected  ->Write();
    hist_DWC2_pos_corrected  ->Write();
    hist_DWC_x_corr_corrected->Write();
    hist_DWC_y_corr_corrected->Write();
    
    hist_DWC1_pos_after  ->Write();
    hist_DWC2_pos_after  ->Write();
    hist_DWC_x_corr_after->Write();
    hist_DWC_y_corr_after->Write();
    
    M1_T1_S_hist->Write();
    M1_T2_S_hist->Write();
    M1_T3_S_hist->Write();
    M1_T4_S_hist->Write();
    
    M2_T1_S_hist->Write();
    M2_T2_S_hist->Write();
    M2_T3_S_hist->Write();
    M2_T4_S_hist->Write();
    
    M3_T1_S_hist->Write();
    M3_T2_S_hist->Write();
    M3_T3_S_hist->Write();
    M3_T4_S_hist->Write();
    
    M4_T1_S_hist->Write();
    M4_T2_S_hist->Write();
    M4_T3_S_hist->Write();
    M4_T4_S_hist->Write();
    
    M5_T1_S_hist->Write();
    M5_T2_S_hist->Write();
    M5_T3_S_hist->Write();
    M5_T4_S_hist->Write();
    
    M6_T1_S_hist->Write();
    M6_T2_S_hist->Write();
    M6_T3_S_hist->Write();
    M6_T4_S_hist->Write();
    
    M7_T1_S_hist->Write();
    M7_T2_S_hist->Write();
    M7_T3_S_hist->Write();
    M7_T4_S_hist->Write();
    
    M8_T1_S_hist->Write();
    M8_T2_S_hist->Write();
    M8_T3_S_hist->Write();
    M8_T4_S_hist->Write();
    
    M9_T1_S_hist->Write();
    M9_T2_S_hist->Write();
    M9_T3_S_hist->Write();
    M9_T4_S_hist->Write();
    
    M1_T1_C_hist->Write();
    M1_T2_C_hist->Write();
    M1_T3_C_hist->Write();
    M1_T4_C_hist->Write();
    
    M2_T1_C_hist->Write();
    M2_T2_C_hist->Write();
    M2_T3_C_hist->Write();
    M2_T4_C_hist->Write();
    
    M3_T1_C_hist->Write();
    M3_T2_C_hist->Write();
    M3_T3_C_hist->Write();
    M3_T4_C_hist->Write();
    
    M4_T1_C_hist->Write();
    M4_T2_C_hist->Write();
    M4_T3_C_hist->Write();
    M4_T4_C_hist->Write();
    
    M5_T1_C_hist->Write();
    M5_T2_C_hist->Write();
    M5_T3_C_hist->Write();
    M5_T4_C_hist->Write();
    
    M6_T1_C_hist->Write();
    M6_T2_C_hist->Write();
    M6_T3_C_hist->Write();
    M6_T4_C_hist->Write();
    
    M7_T1_C_hist->Write();
    M7_T2_C_hist->Write();
    M7_T3_C_hist->Write();
    M7_T4_C_hist->Write();
    
    M8_T1_C_hist->Write();
    M8_T2_C_hist->Write();
    M8_T3_C_hist->Write();
    M8_T4_C_hist->Write();
    
    M9_T1_C_hist->Write();
    M9_T2_C_hist->Write();
    M9_T3_C_hist->Write();
    M9_T4_C_hist->Write();
    outputRoot->Close();
    
    return 0;
}
