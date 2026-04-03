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
    
    // Scaled factor for each channel
    float scaleFactor_C = 0.778861; // Using M5T1 C ch with fitting result (99.2%, 59.52 GeV)
    float scaleFactor_S = 0.791273; // Using M5T1 S ch with fitting result (99.2%, 59.52 GeV)

    // float scaleFactor_C = 0.679374500627782; // Using M5T3 C ch with fitting result (99.2%, 59.52 GeV)
    // float scaleFactor_S = 0.702218027371402; // Using M5T3 S ch with fitting result (99.2%, 59.52 GeV)
    
    // float scaleFactor_C = 0.760347470618293; // Using M8T2 C ch with fitting result (99.2%, 59.52 GeV)
    // float scaleFactor_S = 0.764743672105872; // Using M8T2 S ch with fitting result (99.2%, 59.52 GeV)
    
    // float scaleFactor_C = 0.757059272449758; // Using M9T1 C ch with fitting result (99.2%, 59.52 GeV)
    // float scaleFactor_S = 0.745677774993736; // Using M9T1 S ch with fitting result (99.2%, 59.52 GeV)
    
    float calib_M1_T1_C = 0.001475; float calib_M1_T1_S = 0.000734;
    float calib_M1_T2_C = 0.001457; float calib_M1_T2_S = 0.000762;
    float calib_M1_T3_C = 0.001585; float calib_M1_T3_S = 0.000790;
    float calib_M1_T4_C = 0.001503; float calib_M1_T4_S = 0.000795;
    
    float calib_M2_T1_C = 0.001362; float calib_M2_T1_S = 0.000757;
    float calib_M2_T2_C = 0.001470; float calib_M2_T2_S = 0.000758;
    float calib_M2_T3_C = 0.001380; float calib_M2_T3_S = 0.000786;
    float calib_M2_T4_C = 0.001465; float calib_M2_T4_S = 0.000821;
    
    float calib_M3_T1_C = 0.001433; float calib_M3_T1_S = 0.000744;
    float calib_M3_T2_C = 0.001596; float calib_M3_T2_S = 0.000800;
    float calib_M3_T3_C = 0.001540; float calib_M3_T3_S = 0.000784;
    float calib_M3_T4_C = 0.001502; float calib_M3_T4_S = 0.000803;
    
    float calib_M4_T1_C = 0.001459; float calib_M4_T1_S = 0.000806;
    float calib_M4_T2_C = 0.001199; float calib_M4_T2_S = 0.000757;
    float calib_M4_T3_C = 0.001487; float calib_M4_T3_S = 0.000790;
    float calib_M4_T4_C = 0.001445; float calib_M4_T4_S = 0.000806;
    
    float calib_M5_T1_C = 0.001643; float calib_M5_T1_S = 0.000781;
    float calib_M5_T2_C = 0.001505; float calib_M5_T2_S = 0.000719;
    float calib_M5_T3_C = 0.001940; float calib_M5_T3_S = 0.000999;
    float calib_M5_T4_C = 0.001402; float calib_M5_T4_S = 0.000840;
    
    float calib_M6_T1_C = 0.001513; float calib_M6_T1_S = 0.000815;
    float calib_M6_T2_C = 0.001394; float calib_M6_T2_S = 0.000819;
    float calib_M6_T3_C = 0.001559; float calib_M6_T3_S = 0.000810;
    float calib_M6_T4_C = 0.001659; float calib_M6_T4_S = 0.000836;
    
    float calib_M7_T1_C = 0.001533; float calib_M7_T1_S = 0.000828;
    float calib_M7_T2_C = 0.001491; float calib_M7_T2_S = 0.000766;
    float calib_M7_T3_C = 0.001551; float calib_M7_T3_S = 0.000744;
    float calib_M7_T4_C = 0.001524; float calib_M7_T4_S = 0.000804;
    
    float calib_M8_T1_C = 0.001579; float calib_M8_T1_S = 0.000809;
    float calib_M8_T2_C = 0.001506; float calib_M8_T2_S = 0.000776;
    float calib_M8_T3_C = 0.001484; float calib_M8_T3_S = 0.000772;
    float calib_M8_T4_C = 0.001493; float calib_M8_T4_S = 0.000805;
    
    float calib_M9_T1_C = 0.001521; float calib_M9_T1_S = 0.000794;
    float calib_M9_T2_C = 0.001506; float calib_M9_T2_S = 0.000777;
    float calib_M9_T3_C = 0.001498; float calib_M9_T3_S = 0.000807;
    float calib_M9_T4_C = 0.001456; float calib_M9_T4_S = 0.000848;

    float calib_LC2 = 1.; // Leakage Counter 2
    float calib_LC3 = 1.; // Leakage Counter 3
    float calib_LC4 = 1.; // Leakage Counter 4
    float calib_LC5 = 1.; // Leakage Counter 5
    float calib_LC7 = 1.; // Leakage Counter 7
    float calib_LC8 = 1.; // Leakage Counter 8
    float calib_LC9 = 1.; // Leakage Counter 9
    float calib_LC10 = 1.; // Leakage Counter 10
    float calib_LC11 = 1.; // Leakage Counter 11
    float calib_LC12 = 1.; // Leakage Counter 12
    float calib_LC13 = 1.; // Leakage Counter 13
    float calib_LC14 = 1.; // Leakage Counter 14
    float calib_LC15 = 1.; // Leakage Counter 15
    float calib_LC16 = 1.; // Leakage Counter 16
    float calib_LC19 = 1.; // Leakage Counter 19
    float calib_LC20 = 1.; // Leakage Counter 20
    
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
    
    fs::path dir("./Calib_ADC");   
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
    TBcid cid_CC1 = util.GetCID("CC1"); // Cherenkov counter 1
    TBcid cid_CC2 = util.GetCID("CC2"); // Cherenkov counter 2
    
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
    
    TBcid cid_LC2  = util.GetCID("LC2");  // Leakage Counter
    TBcid cid_LC3  = util.GetCID("LC3");  // Leakage Counter
    TBcid cid_LC4  = util.GetCID("LC4");  // Leakage Counter
    TBcid cid_LC5  = util.GetCID("LC5");  // Leakage Counter
    TBcid cid_LC7  = util.GetCID("LC7");  // Leakage Counter
    TBcid cid_LC8  = util.GetCID("LC8");  // Leakage Counter
    TBcid cid_LC9  = util.GetCID("LC9");  // Leakage Counter
    TBcid cid_LC10 = util.GetCID("LC10"); // Leakage Counter
    TBcid cid_LC11 = util.GetCID("LC11"); // Leakage Counter
    TBcid cid_LC12 = util.GetCID("LC12"); // Leakage Counter
    TBcid cid_LC13 = util.GetCID("LC13"); // Leakage Counter
    TBcid cid_LC14 = util.GetCID("LC14"); // Leakage Counter
    TBcid cid_LC15 = util.GetCID("LC15"); // Leakage Counter
    TBcid cid_LC16 = util.GetCID("LC16"); // Leakage Counter
    TBcid cid_LC19 = util.GetCID("LC19"); // Leakage Counter
    TBcid cid_LC20 = util.GetCID("LC20"); // Leakage Counter
    
    // prepare the histograms wa want to draw
    TH1F* hist_M1_T1_C = new TH1F("M1_T1_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T2_C = new TH1F("M1_T2_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T3_C = new TH1F("M1_T3_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T4_C = new TH1F("M1_T4_C" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M2_T1_C = new TH1F("M2_T1_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T2_C = new TH1F("M2_T2_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T3_C = new TH1F("M2_T3_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T4_C = new TH1F("M2_T4_C" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M3_T1_C = new TH1F("M3_T1_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T2_C = new TH1F("M3_T2_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T3_C = new TH1F("M3_T3_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T4_C = new TH1F("M3_T4_C" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M4_T1_C = new TH1F("M4_T1_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T2_C = new TH1F("M4_T2_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T3_C = new TH1F("M4_T3_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T4_C = new TH1F("M4_T4_C" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M5_T1_C = new TH1F("M5_T1_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T2_C = new TH1F("M5_T2_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T3_C = new TH1F("M5_T3_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T4_C = new TH1F("M5_T4_C" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M6_T1_C = new TH1F("M6_T1_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T2_C = new TH1F("M6_T2_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T3_C = new TH1F("M6_T3_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T4_C = new TH1F("M6_T4_C" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M7_T1_C = new TH1F("M7_T1_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T2_C = new TH1F("M7_T2_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T3_C = new TH1F("M7_T3_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T4_C = new TH1F("M7_T4_C" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M8_T1_C = new TH1F("M8_T1_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T2_C = new TH1F("M8_T2_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T3_C = new TH1F("M8_T3_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T4_C = new TH1F("M8_T4_C" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M9_T1_C = new TH1F("M9_T1_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T2_C = new TH1F("M9_T2_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T3_C = new TH1F("M9_T3_C" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T4_C = new TH1F("M9_T4_C" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M1_T1_S = new TH1F("M1_T1_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T2_S = new TH1F("M1_T2_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T3_S = new TH1F("M1_T3_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T4_S = new TH1F("M1_T4_S" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M2_T1_S = new TH1F("M2_T1_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T2_S = new TH1F("M2_T2_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T3_S = new TH1F("M2_T3_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T4_S = new TH1F("M2_T4_S" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M3_T1_S = new TH1F("M3_T1_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T2_S = new TH1F("M3_T2_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T3_S = new TH1F("M3_T3_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T4_S = new TH1F("M3_T4_S" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M4_T1_S = new TH1F("M4_T1_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T2_S = new TH1F("M4_T2_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T3_S = new TH1F("M4_T3_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T4_S = new TH1F("M4_T4_S" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M5_T1_S = new TH1F("M5_T1_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T2_S = new TH1F("M5_T2_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T3_S = new TH1F("M5_T3_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T4_S = new TH1F("M5_T4_S" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M6_T1_S = new TH1F("M6_T1_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T2_S = new TH1F("M6_T2_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T3_S = new TH1F("M6_T3_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T4_S = new TH1F("M6_T4_S" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M7_T1_S = new TH1F("M7_T1_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T2_S = new TH1F("M7_T2_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T3_S = new TH1F("M7_T3_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T4_S = new TH1F("M7_T4_S" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M8_T1_S = new TH1F("M8_T1_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T2_S = new TH1F("M8_T2_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T3_S = new TH1F("M8_T3_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T4_S = new TH1F("M8_T4_S" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M9_T1_S = new TH1F("M9_T1_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T2_S = new TH1F("M9_T2_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T3_S = new TH1F("M9_T3_S" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T4_S = new TH1F("M9_T4_S" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_Edep_M1_T1_C = new TH1F("Edep_M1_T1_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T2_C = new TH1F("Edep_M1_T2_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T3_C = new TH1F("Edep_M1_T3_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T4_C = new TH1F("Edep_M1_T4_C" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M2_T1_C = new TH1F("Edep_M2_T1_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T2_C = new TH1F("Edep_M2_T2_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T3_C = new TH1F("Edep_M2_T3_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T4_C = new TH1F("Edep_M2_T4_C" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M3_T1_C = new TH1F("Edep_M3_T1_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T2_C = new TH1F("Edep_M3_T2_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T3_C = new TH1F("Edep_M3_T3_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T4_C = new TH1F("Edep_M3_T4_C" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M4_T1_C = new TH1F("Edep_M4_T1_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T2_C = new TH1F("Edep_M4_T2_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T3_C = new TH1F("Edep_M4_T3_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T4_C = new TH1F("Edep_M4_T4_C" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M5_T1_C = new TH1F("Edep_M5_T1_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T2_C = new TH1F("Edep_M5_T2_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T3_C = new TH1F("Edep_M5_T3_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T4_C = new TH1F("Edep_M5_T4_C" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M6_T1_C = new TH1F("Edep_M6_T1_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T2_C = new TH1F("Edep_M6_T2_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T3_C = new TH1F("Edep_M6_T3_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T4_C = new TH1F("Edep_M6_T4_C" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M7_T1_C = new TH1F("Edep_M7_T1_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T2_C = new TH1F("Edep_M7_T2_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T3_C = new TH1F("Edep_M7_T3_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T4_C = new TH1F("Edep_M7_T4_C" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M8_T1_C = new TH1F("Edep_M8_T1_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T2_C = new TH1F("Edep_M8_T2_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T3_C = new TH1F("Edep_M8_T3_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T4_C = new TH1F("Edep_M8_T4_C" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M9_T1_C = new TH1F("Edep_M9_T1_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T2_C = new TH1F("Edep_M9_T2_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T3_C = new TH1F("Edep_M9_T3_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T4_C = new TH1F("Edep_M9_T4_C" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M1_T1_S = new TH1F("Edep_M1_T1_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T2_S = new TH1F("Edep_M1_T2_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T3_S = new TH1F("Edep_M1_T3_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T4_S = new TH1F("Edep_M1_T4_S" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M2_T1_S = new TH1F("Edep_M2_T1_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T2_S = new TH1F("Edep_M2_T2_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T3_S = new TH1F("Edep_M2_T3_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T4_S = new TH1F("Edep_M2_T4_S" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M3_T1_S = new TH1F("Edep_M3_T1_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T2_S = new TH1F("Edep_M3_T2_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T3_S = new TH1F("Edep_M3_T3_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T4_S = new TH1F("Edep_M3_T4_S" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M4_T1_S = new TH1F("Edep_M4_T1_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T2_S = new TH1F("Edep_M4_T2_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T3_S = new TH1F("Edep_M4_T3_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T4_S = new TH1F("Edep_M4_T4_S" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M5_T1_S = new TH1F("Edep_M5_T1_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T2_S = new TH1F("Edep_M5_T2_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T3_S = new TH1F("Edep_M5_T3_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T4_S = new TH1F("Edep_M5_T4_S" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M6_T1_S = new TH1F("Edep_M6_T1_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T2_S = new TH1F("Edep_M6_T2_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T3_S = new TH1F("Edep_M6_T3_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T4_S = new TH1F("Edep_M6_T4_S" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M7_T1_S = new TH1F("Edep_M7_T1_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T2_S = new TH1F("Edep_M7_T2_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T3_S = new TH1F("Edep_M7_T3_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T4_S = new TH1F("Edep_M7_T4_S" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M8_T1_S = new TH1F("Edep_M8_T1_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T2_S = new TH1F("Edep_M8_T2_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T3_S = new TH1F("Edep_M8_T3_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T4_S = new TH1F("Edep_M8_T4_S" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M9_T1_S = new TH1F("Edep_M9_T1_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T2_S = new TH1F("Edep_M9_T2_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T3_S = new TH1F("Edep_M9_T3_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T4_S = new TH1F("Edep_M9_T4_S" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M1_C = new TH1F("Edep_M1_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_C = new TH1F("Edep_M2_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_C = new TH1F("Edep_M3_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_C = new TH1F("Edep_M4_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_C = new TH1F("Edep_M5_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_C = new TH1F("Edep_M6_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_C = new TH1F("Edep_M7_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_C = new TH1F("Edep_M8_C" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_C = new TH1F("Edep_M9_C" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M1_S = new TH1F("Edep_M1_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_S = new TH1F("Edep_M2_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_S = new TH1F("Edep_M3_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_S = new TH1F("Edep_M4_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_S = new TH1F("Edep_M5_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_S = new TH1F("Edep_M6_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_S = new TH1F("Edep_M7_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_S = new TH1F("Edep_M8_S" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_S = new TH1F("Edep_M9_S" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_CC1 = new TH1F("CC1", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_CC2 = new TH1F("CC2", ";peakADC;Events", 1024, 0, 4096);
    
    // TH1F* hist_PS = new TH1F("PS" , ";intADC;nEvents", 320, -20000, 300000);
    // TH1F* hist_MC = new TH1F("MC" , ";intADC;nEvents", 320, -20000, 300000);
    // TH1F* hist_TC = new TH1F("TC" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_PS = new TH1F("PS" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_MC = new TH1F("MC" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_TC = new TH1F("TC" , ";peakADC;nEvents", 1024, 0, 4096);
    
    TH1F* hist_LC2  = new TH1F("LC2"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC3  = new TH1F("LC3"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC4  = new TH1F("LC4"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC5  = new TH1F("LC5"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC7  = new TH1F("LC7"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC8  = new TH1F("LC8"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC9  = new TH1F("LC9"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC10 = new TH1F("LC10" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC11 = new TH1F("LC11" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC12 = new TH1F("LC12" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC13 = new TH1F("LC13" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC14 = new TH1F("LC14" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC15 = new TH1F("LC15" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC16 = new TH1F("LC16" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC19 = new TH1F("LC19" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC20 = new TH1F("LC20" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_totalEdep_C          = new TH1F("totalEdep_C",    ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdep_S          = new TH1F("totalEdep_S",    ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdep_S_LC_compensated = new TH1F("totalEdep_S_LC_compensated", ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdep_Comb       = new TH1F("totalEdep_Comb", ";GeV;Event", 110, -40, 400);
    TH1F* hist_totalEdep_Comb_LC_compensated = new TH1F("totalEdep_Comb_LC_compensated", ";GeV;Event", 110, -40, 400);
    
    TH1F* hist_totalEdepScaled_C    = new TH1F("totalEdepScaled_C",    ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdepScaled_S    = new TH1F("totalEdepScaled_S",    ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdepScaled_S_LC_compensated = new TH1F("totalEdepScaled_S_LC_compensated", ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdepScaled_Comb = new TH1F("totalEdepScaled_Comb", ";GeV;Event", 110, -40, 400);
    TH1F* hist_totalEdepScaled_Comb_LC_compensated = new TH1F("totalEdepScaled_Comb_LC_compensated", ";GeV;Event", 110, -40, 400);
    
    TH2D* hist_DWC1_pos_corrected   = new TH2D("DWC1_pos_corrected",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_corrected   = new TH2D("DWC2_pos_corrected",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_corrected = new TH2D("DWC_x_corr_corrected", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_corrected = new TH2D("DWC_y_corr_corrected", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);
    
    // Histograms after PID
    
    TH1F* hist_M1_T1_C_after = new TH1F("M1_T1_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T2_C_after = new TH1F("M1_T2_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T3_C_after = new TH1F("M1_T3_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T4_C_after = new TH1F("M1_T4_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M2_T1_C_after = new TH1F("M2_T1_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T2_C_after = new TH1F("M2_T2_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T3_C_after = new TH1F("M2_T3_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T4_C_after = new TH1F("M2_T4_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M3_T1_C_after = new TH1F("M3_T1_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T2_C_after = new TH1F("M3_T2_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T3_C_after = new TH1F("M3_T3_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T4_C_after = new TH1F("M3_T4_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M4_T1_C_after = new TH1F("M4_T1_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T2_C_after = new TH1F("M4_T2_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T3_C_after = new TH1F("M4_T3_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T4_C_after = new TH1F("M4_T4_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M5_T1_C_after = new TH1F("M5_T1_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T2_C_after = new TH1F("M5_T2_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T3_C_after = new TH1F("M5_T3_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T4_C_after = new TH1F("M5_T4_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M6_T1_C_after = new TH1F("M6_T1_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T2_C_after = new TH1F("M6_T2_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T3_C_after = new TH1F("M6_T3_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T4_C_after = new TH1F("M6_T4_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M7_T1_C_after = new TH1F("M7_T1_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T2_C_after = new TH1F("M7_T2_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T3_C_after = new TH1F("M7_T3_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T4_C_after = new TH1F("M7_T4_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M8_T1_C_after = new TH1F("M8_T1_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T2_C_after = new TH1F("M8_T2_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T3_C_after = new TH1F("M8_T3_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T4_C_after = new TH1F("M8_T4_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M9_T1_C_after = new TH1F("M9_T1_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T2_C_after = new TH1F("M9_T2_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T3_C_after = new TH1F("M9_T3_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T4_C_after = new TH1F("M9_T4_C_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M1_T1_S_after = new TH1F("M1_T1_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T2_S_after = new TH1F("M1_T2_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T3_S_after = new TH1F("M1_T3_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M1_T4_S_after = new TH1F("M1_T4_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M2_T1_S_after = new TH1F("M2_T1_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T2_S_after = new TH1F("M2_T2_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T3_S_after = new TH1F("M2_T3_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M2_T4_S_after = new TH1F("M2_T4_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M3_T1_S_after = new TH1F("M3_T1_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T2_S_after = new TH1F("M3_T2_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T3_S_after = new TH1F("M3_T3_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M3_T4_S_after = new TH1F("M3_T4_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M4_T1_S_after = new TH1F("M4_T1_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T2_S_after = new TH1F("M4_T2_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T3_S_after = new TH1F("M4_T3_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M4_T4_S_after = new TH1F("M4_T4_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M5_T1_S_after = new TH1F("M5_T1_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T2_S_after = new TH1F("M5_T2_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T3_S_after = new TH1F("M5_T3_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M5_T4_S_after = new TH1F("M5_T4_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M6_T1_S_after = new TH1F("M6_T1_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T2_S_after = new TH1F("M6_T2_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T3_S_after = new TH1F("M6_T3_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M6_T4_S_after = new TH1F("M6_T4_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M7_T1_S_after = new TH1F("M7_T1_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T2_S_after = new TH1F("M7_T2_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T3_S_after = new TH1F("M7_T3_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M7_T4_S_after = new TH1F("M7_T4_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M8_T1_S_after = new TH1F("M8_T1_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T2_S_after = new TH1F("M8_T2_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T3_S_after = new TH1F("M8_T3_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M8_T4_S_after = new TH1F("M8_T4_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_M9_T1_S_after = new TH1F("M9_T1_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T2_S_after = new TH1F("M9_T2_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T3_S_after = new TH1F("M9_T3_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_M9_T4_S_after = new TH1F("M9_T4_S_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_Edep_M1_T1_C_after = new TH1F("Edep_M1_T1_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T2_C_after = new TH1F("Edep_M1_T2_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T3_C_after = new TH1F("Edep_M1_T3_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T4_C_after = new TH1F("Edep_M1_T4_C_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M2_T1_C_after = new TH1F("Edep_M2_T1_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T2_C_after = new TH1F("Edep_M2_T2_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T3_C_after = new TH1F("Edep_M2_T3_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T4_C_after = new TH1F("Edep_M2_T4_C_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M3_T1_C_after = new TH1F("Edep_M3_T1_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T2_C_after = new TH1F("Edep_M3_T2_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T3_C_after = new TH1F("Edep_M3_T3_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T4_C_after = new TH1F("Edep_M3_T4_C_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M4_T1_C_after = new TH1F("Edep_M4_T1_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T2_C_after = new TH1F("Edep_M4_T2_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T3_C_after = new TH1F("Edep_M4_T3_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T4_C_after = new TH1F("Edep_M4_T4_C_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M5_T1_C_after = new TH1F("Edep_M5_T1_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T2_C_after = new TH1F("Edep_M5_T2_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T3_C_after = new TH1F("Edep_M5_T3_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T4_C_after = new TH1F("Edep_M5_T4_C_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M6_T1_C_after = new TH1F("Edep_M6_T1_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T2_C_after = new TH1F("Edep_M6_T2_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T3_C_after = new TH1F("Edep_M6_T3_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T4_C_after = new TH1F("Edep_M6_T4_C_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M7_T1_C_after = new TH1F("Edep_M7_T1_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T2_C_after = new TH1F("Edep_M7_T2_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T3_C_after = new TH1F("Edep_M7_T3_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T4_C_after = new TH1F("Edep_M7_T4_C_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M8_T1_C_after = new TH1F("Edep_M8_T1_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T2_C_after = new TH1F("Edep_M8_T2_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T3_C_after = new TH1F("Edep_M8_T3_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T4_C_after = new TH1F("Edep_M8_T4_C_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M9_T1_C_after = new TH1F("Edep_M9_T1_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T2_C_after = new TH1F("Edep_M9_T2_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T3_C_after = new TH1F("Edep_M9_T3_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T4_C_after = new TH1F("Edep_M9_T4_C_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M1_T1_S_after = new TH1F("Edep_M1_T1_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T2_S_after = new TH1F("Edep_M1_T2_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T3_S_after = new TH1F("Edep_M1_T3_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M1_T4_S_after = new TH1F("Edep_M1_T4_S_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M2_T1_S_after = new TH1F("Edep_M2_T1_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T2_S_after = new TH1F("Edep_M2_T2_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T3_S_after = new TH1F("Edep_M2_T3_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_T4_S_after = new TH1F("Edep_M2_T4_S_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M3_T1_S_after = new TH1F("Edep_M3_T1_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T2_S_after = new TH1F("Edep_M3_T2_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T3_S_after = new TH1F("Edep_M3_T3_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_T4_S_after = new TH1F("Edep_M3_T4_S_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M4_T1_S_after = new TH1F("Edep_M4_T1_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T2_S_after = new TH1F("Edep_M4_T2_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T3_S_after = new TH1F("Edep_M4_T3_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_T4_S_after = new TH1F("Edep_M4_T4_S_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M5_T1_S_after = new TH1F("Edep_M5_T1_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T2_S_after = new TH1F("Edep_M5_T2_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T3_S_after = new TH1F("Edep_M5_T3_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_T4_S_after = new TH1F("Edep_M5_T4_S_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M6_T1_S_after = new TH1F("Edep_M6_T1_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T2_S_after = new TH1F("Edep_M6_T2_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T3_S_after = new TH1F("Edep_M6_T3_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_T4_S_after = new TH1F("Edep_M6_T4_S_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M7_T1_S_after = new TH1F("Edep_M7_T1_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T2_S_after = new TH1F("Edep_M7_T2_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T3_S_after = new TH1F("Edep_M7_T3_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_T4_S_after = new TH1F("Edep_M7_T4_S_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M8_T1_S_after = new TH1F("Edep_M8_T1_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T2_S_after = new TH1F("Edep_M8_T2_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T3_S_after = new TH1F("Edep_M8_T3_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_T4_S_after = new TH1F("Edep_M8_T4_S_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M9_T1_S_after = new TH1F("Edep_M9_T1_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T2_S_after = new TH1F("Edep_M9_T2_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T3_S_after = new TH1F("Edep_M9_T3_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_T4_S_after = new TH1F("Edep_M9_T4_S_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M12_C_after = new TH1F("Edep_M12_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M23_C_after = new TH1F("Edep_M23_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M34_C_after = new TH1F("Edep_M34_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M45_C_after = new TH1F("Edep_M45_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M56_C_after = new TH1F("Edep_M56_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M67_C_after = new TH1F("Edep_M67_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M78_C_after = new TH1F("Edep_M78_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M89_C_after = new TH1F("Edep_M89_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M91_C_after = new TH1F("Edep_M91_C_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M12_S_after = new TH1F("Edep_M12_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M23_S_after = new TH1F("Edep_M23_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M34_S_after = new TH1F("Edep_M34_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M45_S_after = new TH1F("Edep_M45_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M56_S_after = new TH1F("Edep_M56_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M67_S_after = new TH1F("Edep_M67_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M78_S_after = new TH1F("Edep_M78_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M89_S_after = new TH1F("Edep_M89_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M91_S_after = new TH1F("Edep_M91_S_after" , ";GeV;Event", 280, -5, 30);

    TH1F* hist_Edep_M1_C_after = new TH1F("Edep_M1_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_C_after = new TH1F("Edep_M2_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_C_after = new TH1F("Edep_M3_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_C_after = new TH1F("Edep_M4_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_C_after = new TH1F("Edep_M5_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_C_after = new TH1F("Edep_M6_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_C_after = new TH1F("Edep_M7_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_C_after = new TH1F("Edep_M8_C_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_C_after = new TH1F("Edep_M9_C_after" , ";GeV;Event", 280, -5, 30);
    
    TH1F* hist_Edep_M1_S_after = new TH1F("Edep_M1_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M2_S_after = new TH1F("Edep_M2_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M3_S_after = new TH1F("Edep_M3_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M4_S_after = new TH1F("Edep_M4_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M5_S_after = new TH1F("Edep_M5_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M6_S_after = new TH1F("Edep_M6_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M7_S_after = new TH1F("Edep_M7_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M8_S_after = new TH1F("Edep_M8_S_after" , ";GeV;Event", 280, -5, 30);
    TH1F* hist_Edep_M9_S_after = new TH1F("Edep_M9_S_after" , ";GeV;Event", 280, -5, 30);
    
    // TH1F* hist_PS_after = new TH1F("PS_after" , ";intADC;nEvents", 320, -20000, 300000);
    // TH1F* hist_MC_after = new TH1F("MC_after" , ";intADC;nEvents", 320, -20000, 300000);
    // TH1F* hist_TC_after = new TH1F("TC_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_PS_after = new TH1F("PS_after" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_MC_after = new TH1F("MC_after" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_TC_after = new TH1F("TC_after" , ";peakADC;nEvents", 1024, 0, 4096);
    
    TH1F* hist_CC1_after = new TH1F("CC1_after", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_CC2_after = new TH1F("CC2_after", ";peakADC;Events", 1024, 0, 4096);
    
    TH1F* hist_LC2_after  = new TH1F("LC2_after"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC3_after  = new TH1F("LC3_after"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC4_after  = new TH1F("LC4_after"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC5_after  = new TH1F("LC5_after"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC7_after  = new TH1F("LC7_after"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC8_after  = new TH1F("LC8_after"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC9_after  = new TH1F("LC9_after"  , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC10_after = new TH1F("LC10_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC11_after = new TH1F("LC11_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC12_after = new TH1F("LC12_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC13_after = new TH1F("LC13_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC14_after = new TH1F("LC14_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC15_after = new TH1F("LC15_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC16_after = new TH1F("LC16_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC19_after = new TH1F("LC19_after" , ";intADC;nEvents", 320, -20000, 300000);
    TH1F* hist_LC20_after = new TH1F("LC20_after" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH1F* hist_totalEdep_C_after          = new TH1F("totalEdep_C_after",    ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdep_S_after          = new TH1F("totalEdep_S_after",    ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdep_S_LC_compensated_after = new TH1F("totalEdep_S_LC_compensated_after", ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdep_Comb_after       = new TH1F("totalEdep_Comb_after", ";GeV;Event", 110, -40, 400);
    TH1F* hist_totalEdep_Comb_LC_compensated_after = new TH1F("totalEdep_Comb_LC_compensated_after", ";GeV;Event", 110, -40, 400);
    
    TH1F* hist_totalEdepScaled_C_after    = new TH1F("totalEdepScaled_C_after",    ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdepScaled_S_after    = new TH1F("totalEdepScaled_S_after",    ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdepScaled_S_LC_compensated_after = new TH1F("totalEdepScaled_S_LC_compensated_after", ";GeV;Event", 280, -5, 30);
    TH1F* hist_totalEdepScaled_Comb_after = new TH1F("totalEdepScaled_Comb_after", ";GeV;Event", 110, -40, 400);
    TH1F* hist_totalEdepScaled_Comb_LC_compensated_after = new TH1F("totalEdepScaled_Comb_LC_compensated_after", ";GeV;Event", 110, -40, 400);

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
            wave_M1_C.emplace_back( anEvt.GetData(cid_M1_C.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M2_C.emplace_back( anEvt.GetData(cid_M2_C.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M3_C.emplace_back( anEvt.GetData(cid_M3_C.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M4_C.emplace_back( anEvt.GetData(cid_M4_C.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M5_C.emplace_back( anEvt.GetData(cid_M5_C.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M6_C.emplace_back( anEvt.GetData(cid_M6_C.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M7_C.emplace_back( anEvt.GetData(cid_M7_C.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M8_C.emplace_back( anEvt.GetData(cid_M8_C.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M9_C.emplace_back( anEvt.GetData(cid_M9_C.at(tower)).ADCcorrectedWaveformF() ); 
            
            wave_M1_S.emplace_back( anEvt.GetData(cid_M1_S.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M2_S.emplace_back( anEvt.GetData(cid_M2_S.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M3_S.emplace_back( anEvt.GetData(cid_M3_S.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M4_S.emplace_back( anEvt.GetData(cid_M4_S.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M5_S.emplace_back( anEvt.GetData(cid_M5_S.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M6_S.emplace_back( anEvt.GetData(cid_M6_S.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M7_S.emplace_back( anEvt.GetData(cid_M7_S.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M8_S.emplace_back( anEvt.GetData(cid_M8_S.at(tower)).ADCcorrectedWaveformF() ); 
            wave_M9_S.emplace_back( anEvt.GetData(cid_M9_S.at(tower)).ADCcorrectedWaveformF() ); 
        } 
        
        std::vector<float> intADC_M1_C; std::vector<float> intADC_M1_S;
        std::vector<float> intADC_M2_C; std::vector<float> intADC_M2_S;
        std::vector<float> intADC_M3_C; std::vector<float> intADC_M3_S;
        std::vector<float> intADC_M4_C; std::vector<float> intADC_M4_S;
        std::vector<float> intADC_M5_C; std::vector<float> intADC_M5_S;
        std::vector<float> intADC_M6_C; std::vector<float> intADC_M6_S;
        std::vector<float> intADC_M7_C; std::vector<float> intADC_M7_S;
        std::vector<float> intADC_M8_C; std::vector<float> intADC_M8_S;
        std::vector<float> intADC_M9_C; std::vector<float> intADC_M9_S;
        for(int tower = 0; tower < 4; tower++) {
            intADC_M1_C.emplace_back( GetInt(wave_M1_C.at(tower), C_first, C_last) );
            intADC_M2_C.emplace_back( GetInt(wave_M2_C.at(tower), C_first, C_last) );
            intADC_M3_C.emplace_back( GetInt(wave_M3_C.at(tower), C_first, C_last) );
            intADC_M4_C.emplace_back( GetInt(wave_M4_C.at(tower), C_first, C_last) );
            intADC_M6_C.emplace_back( GetInt(wave_M6_C.at(tower), C_first, C_last) );
            intADC_M7_C.emplace_back( GetInt(wave_M7_C.at(tower), C_first, C_last) );
            intADC_M8_C.emplace_back( GetInt(wave_M8_C.at(tower), C_first, C_last) );
            intADC_M9_C.emplace_back( GetInt(wave_M9_C.at(tower), C_first, C_last) );
            
            intADC_M1_S.emplace_back( GetInt(wave_M1_S.at(tower), S_first, S_last) );
            intADC_M2_S.emplace_back( GetInt(wave_M2_S.at(tower), S_first, S_last) );
            intADC_M3_S.emplace_back( GetInt(wave_M3_S.at(tower), S_first, S_last) );
            intADC_M4_S.emplace_back( GetInt(wave_M4_S.at(tower), S_first, S_last) );
            intADC_M6_S.emplace_back( GetInt(wave_M6_S.at(tower), S_first, S_last) );
            intADC_M7_S.emplace_back( GetInt(wave_M7_S.at(tower), S_first, S_last) );
            intADC_M8_S.emplace_back( GetInt(wave_M8_S.at(tower), S_first, S_last) );
            intADC_M9_S.emplace_back( GetInt(wave_M9_S.at(tower), S_first, S_last) );
        } 
        // M5 integration, use different integration range for M5T3
        intADC_M5_C.emplace_back( GetInt(wave_M5_C.at(0), C_first, C_last) );
        intADC_M5_C.emplace_back( GetInt(wave_M5_C.at(1), C_first, C_last) );
        intADC_M5_C.emplace_back( GetInt(wave_M5_C.at(2), M5T3C_first, M5T3C_last) );
        intADC_M5_C.emplace_back( GetInt(wave_M5_C.at(3), C_first, C_last) );
        intADC_M5_S.emplace_back( GetInt(wave_M5_S.at(0), S_first, S_last) );
        intADC_M5_S.emplace_back( GetInt(wave_M5_S.at(1), S_first, S_last) );
        intADC_M5_S.emplace_back( GetInt(wave_M5_S.at(2), M5T3S_first, M5T3S_last) );
        intADC_M5_S.emplace_back( GetInt(wave_M5_S.at(3), S_first, S_last) );
        
        std::vector<short> wave_CC1 = (anEvt.GetData(cid_CC1)).waveform();
        std::vector<short> wave_CC2 = (anEvt.GetData(cid_CC2)).waveform();
        
        std::vector<short> wave_PS = (anEvt.GetData(cid_PS)).waveform();
        std::vector<short> wave_MC = (anEvt.GetData(cid_MC)).waveform();
        std::vector<short> wave_TC = (anEvt.GetData(cid_TC)).waveform();
        
        // float signal_CC1 = GetInt(wave_CC1, CC1_peak_first, CC1_peak_last); // IntADC
        // float signal_CC2 = GetInt(wave_CC2, CC2_peak_first, CC2_peak_last); // IntADC
        float signal_CC1 = GetPeak(wave_CC1, CC1_peak_first, CC1_peak_last); // PeakADC
        float signal_CC2 = GetPeak(wave_CC2, CC2_peak_first, CC2_peak_last); // PeakADC
        
        // float signal_PS = GetInt(wave_PS, PS_first, PS_last); // IntADC
        // float signal_MC = GetInt(wave_MC, MC_first, MC_last); // IntADC
        // float signal_TC = GetInt(wave_TC, TC_first, TC_last); // IntADC
        float signal_PS = GetPeak(wave_PS, PS_first, PS_last); // PeakADC
        float signal_MC = GetPeak(wave_MC, MC_first, MC_last); // PeakADC
        float signal_TC = GetPeak(wave_TC, TC_first, TC_last); // PeakADC
        
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
        
        std::vector<short> wave_LC2  = (anEvt.GetData(cid_LC2 )).waveform();
        std::vector<short> wave_LC3  = (anEvt.GetData(cid_LC3 )).waveform();
        std::vector<short> wave_LC4  = (anEvt.GetData(cid_LC4 )).waveform();
        std::vector<short> wave_LC5  = (anEvt.GetData(cid_LC5 )).waveform();
        std::vector<short> wave_LC7  = (anEvt.GetData(cid_LC7 )).waveform();
        std::vector<short> wave_LC8  = (anEvt.GetData(cid_LC8 )).waveform();
        std::vector<short> wave_LC9  = (anEvt.GetData(cid_LC9 )).waveform();
        std::vector<short> wave_LC10 = (anEvt.GetData(cid_LC10)).waveform();
        std::vector<short> wave_LC11 = (anEvt.GetData(cid_LC11)).waveform();
        std::vector<short> wave_LC12 = (anEvt.GetData(cid_LC12)).waveform();
        std::vector<short> wave_LC13 = (anEvt.GetData(cid_LC13)).waveform();
        std::vector<short> wave_LC14 = (anEvt.GetData(cid_LC14)).waveform();
        std::vector<short> wave_LC15 = (anEvt.GetData(cid_LC15)).waveform();
        std::vector<short> wave_LC16 = (anEvt.GetData(cid_LC16)).waveform();
        std::vector<short> wave_LC19 = (anEvt.GetData(cid_LC19)).waveform();
        std::vector<short> wave_LC20 = (anEvt.GetData(cid_LC20)).waveform();
        
        float intADC_LC2  = GetInt( wave_LC2 , LC_first, LC_last);
        float intADC_LC3  = GetInt( wave_LC3 , LC_first, LC_last);
        float intADC_LC4  = GetInt( wave_LC4 , LC_first, LC_last);
        float intADC_LC5  = GetInt( wave_LC5 , LC_first, LC_last);
        float intADC_LC7  = GetInt( wave_LC7 , LC_first, LC_last);
        float intADC_LC8  = GetInt( wave_LC8 , LC_first, LC_last);
        float intADC_LC9  = GetInt( wave_LC9 , LC_first, LC_last);
        float intADC_LC10 = GetInt( wave_LC10, LC_first, LC_last);
        float intADC_LC11 = GetInt( wave_LC11, LC_first, LC_last);
        float intADC_LC12 = GetInt( wave_LC12, LC_first, LC_last);
        float intADC_LC13 = GetInt( wave_LC13, LC_first, LC_last);
        float intADC_LC14 = GetInt( wave_LC14, LC_first, LC_last);
        float intADC_LC15 = GetInt( wave_LC15, LC_first, LC_last);
        float intADC_LC16 = GetInt( wave_LC16, LC_first, LC_last);
        float intADC_LC19 = GetInt( wave_LC19, LC_first, LC_last);
        float intADC_LC20 = GetInt( wave_LC20, LC_first, LC_last);

        float Edep_LC2  = (intADC_LC2  * calib_LC2 ); if (Edep_LC2 < 0) Edep_LC2 = 0;
        float Edep_LC3  = (intADC_LC3  * calib_LC3 ); if (Edep_LC3 < 0) Edep_LC3 = 0;
        float Edep_LC4  = (intADC_LC4  * calib_LC4 ); if (Edep_LC4 < 0) Edep_LC4 = 0;
        float Edep_LC5  = (intADC_LC5  * calib_LC5 ); if (Edep_LC5 < 0) Edep_LC5 = 0;
        float Edep_LC7  = (intADC_LC7  * calib_LC7 ); if (Edep_LC7 < 0) Edep_LC7 = 0;
        float Edep_LC8  = (intADC_LC8  * calib_LC8 ); if (Edep_LC8 < 0) Edep_LC8 = 0;
        float Edep_LC9  = (intADC_LC9  * calib_LC9 ); if (Edep_LC9 < 0) Edep_LC9 = 0;
        float Edep_LC10 = (intADC_LC10 * calib_LC10); if (Edep_LC10 < 0) Edep_LC10 = 0;
        float Edep_LC11 = (intADC_LC11 * calib_LC11); if (Edep_LC11 < 0) Edep_LC11 = 0;
        float Edep_LC12 = (intADC_LC12 * calib_LC12); if (Edep_LC12 < 0) Edep_LC12 = 0;
        float Edep_LC13 = (intADC_LC13 * calib_LC13); if (Edep_LC13 < 0) Edep_LC13 = 0;
        float Edep_LC14 = (intADC_LC14 * calib_LC14); if (Edep_LC14 < 0) Edep_LC14 = 0;
        float Edep_LC15 = (intADC_LC15 * calib_LC15); if (Edep_LC15 < 0) Edep_LC15 = 0;
        float Edep_LC16 = (intADC_LC16 * calib_LC16); if (Edep_LC16 < 0) Edep_LC16 = 0;
        float Edep_LC19 = (intADC_LC19 * calib_LC19); if (Edep_LC19 < 0) Edep_LC19 = 0;
        float Edep_LC20 = (intADC_LC20 * calib_LC20); if (Edep_LC20 < 0) Edep_LC20 = 0;
        float Edep_LC = Edep_LC2 + Edep_LC4 + Edep_LC8 + Edep_LC10 // Saleve
                      + Edep_LC3 + Edep_LC5 + Edep_LC7 + Edep_LC9 // Jura
                      + Edep_LC11 + Edep_LC12 + Edep_LC13 + Edep_LC19 // Top
                      + Edep_LC14 + Edep_LC15 + Edep_LC16 + Edep_LC20; // Bottom

        
        float Edep_M1T1_C = (intADC_M1_C.at(0) * calib_M1_T1_C);
        float Edep_M1T2_C = (intADC_M1_C.at(1) * calib_M1_T2_C);
        float Edep_M1T3_C = (intADC_M1_C.at(2) * calib_M1_T3_C);
        float Edep_M1T4_C = (intADC_M1_C.at(3) * calib_M1_T4_C);
        
        float Edep_M2T1_C = (intADC_M2_C.at(0) * calib_M2_T1_C);
        float Edep_M2T2_C = (intADC_M2_C.at(1) * calib_M2_T2_C);
        float Edep_M2T3_C = (intADC_M2_C.at(2) * calib_M2_T3_C);
        float Edep_M2T4_C = (intADC_M2_C.at(3) * calib_M2_T4_C);
        
        float Edep_M3T1_C = (intADC_M3_C.at(0) * calib_M3_T1_C);
        float Edep_M3T2_C = (intADC_M3_C.at(1) * calib_M3_T2_C);
        float Edep_M3T3_C = (intADC_M3_C.at(2) * calib_M3_T3_C);
        float Edep_M3T4_C = (intADC_M3_C.at(3) * calib_M3_T4_C);
        
        float Edep_M4T1_C = (intADC_M4_C.at(0) * calib_M4_T1_C);
        float Edep_M4T2_C = (intADC_M4_C.at(1) * calib_M4_T2_C);
        float Edep_M4T3_C = (intADC_M4_C.at(2) * calib_M4_T3_C);
        float Edep_M4T4_C = (intADC_M4_C.at(3) * calib_M4_T4_C);
        
        float Edep_M5T1_C = (intADC_M5_C.at(0) * calib_M5_T1_C);
        float Edep_M5T2_C = (intADC_M5_C.at(1) * calib_M5_T2_C);
        float Edep_M5T3_C = (intADC_M5_C.at(2) * calib_M5_T3_C);
        float Edep_M5T4_C = (intADC_M5_C.at(3) * calib_M5_T4_C);
        
        float Edep_M6T1_C = (intADC_M6_C.at(0) * calib_M6_T1_C);
        float Edep_M6T2_C = (intADC_M6_C.at(1) * calib_M6_T2_C);
        float Edep_M6T3_C = (intADC_M6_C.at(2) * calib_M6_T3_C);
        float Edep_M6T4_C = (intADC_M6_C.at(3) * calib_M6_T4_C);
        
        float Edep_M7T1_C = (intADC_M7_C.at(0) * calib_M7_T1_C);
        float Edep_M7T2_C = (intADC_M7_C.at(1) * calib_M7_T2_C);
        float Edep_M7T3_C = (intADC_M7_C.at(2) * calib_M7_T3_C);
        float Edep_M7T4_C = (intADC_M7_C.at(3) * calib_M7_T4_C);
        
        float Edep_M8T1_C = (intADC_M8_C.at(0) * calib_M8_T1_C);
        float Edep_M8T2_C = (intADC_M8_C.at(1) * calib_M8_T2_C);
        float Edep_M8T3_C = (intADC_M8_C.at(2) * calib_M8_T3_C);
        float Edep_M8T4_C = (intADC_M8_C.at(3) * calib_M8_T4_C);
        
        float Edep_M9T1_C = (intADC_M9_C.at(0) * calib_M9_T1_C);
        float Edep_M9T2_C = (intADC_M9_C.at(1) * calib_M9_T2_C);
        float Edep_M9T3_C = (intADC_M9_C.at(2) * calib_M9_T3_C);
        float Edep_M9T4_C = (intADC_M9_C.at(3) * calib_M9_T4_C);
        
        float Edep_M1_C = Edep_M1T1_C + Edep_M1T2_C + Edep_M1T3_C + Edep_M1T4_C;
        float Edep_M2_C = Edep_M2T1_C + Edep_M2T2_C + Edep_M2T3_C + Edep_M2T4_C;
        float Edep_M3_C = Edep_M3T1_C + Edep_M3T2_C + Edep_M3T3_C + Edep_M3T4_C;
        float Edep_M4_C = Edep_M4T1_C + Edep_M4T2_C + Edep_M4T3_C + Edep_M4T4_C;
        float Edep_M5_C = Edep_M5T1_C + Edep_M5T2_C + Edep_M5T3_C + Edep_M5T4_C;
        float Edep_M6_C = Edep_M6T1_C + Edep_M6T2_C + Edep_M6T3_C + Edep_M6T4_C;
        float Edep_M7_C = Edep_M7T1_C + Edep_M7T2_C + Edep_M7T3_C + Edep_M7T4_C;
        float Edep_M8_C = Edep_M8T1_C + Edep_M8T2_C + Edep_M8T3_C + Edep_M8T4_C;
        float Edep_M9_C = Edep_M9T1_C + Edep_M9T2_C + Edep_M9T3_C + Edep_M9T4_C;
        
        float Edep_M12_C = Edep_M1_C + Edep_M2_C ;
        float Edep_M23_C = Edep_M2_C + Edep_M3_C ;
        float Edep_M34_C = Edep_M3_C + Edep_M4_C ;
        float Edep_M45_C = Edep_M4_C + Edep_M5_C ;
        float Edep_M56_C = Edep_M5_C + Edep_M6_C ;
        float Edep_M67_C = Edep_M6_C + Edep_M7_C ;
        float Edep_M78_C = Edep_M7_C + Edep_M8_C ;
        float Edep_M89_C = Edep_M8_C + Edep_M9_C ;
        float Edep_M91_C = Edep_M9_C + Edep_M1_C ;

        float totalEdep_C = Edep_M1_C +
                            Edep_M2_C +
                            Edep_M3_C +
                            Edep_M4_C +
                            Edep_M5_C +
                            Edep_M6_C +
                            Edep_M7_C +
                            Edep_M8_C +
                            Edep_M9_C;
        
        float Edep_M1T1_S = (intADC_M1_S.at(0) * calib_M1_T1_S);
        float Edep_M1T2_S = (intADC_M1_S.at(1) * calib_M1_T2_S);
        float Edep_M1T3_S = (intADC_M1_S.at(2) * calib_M1_T3_S);
        float Edep_M1T4_S = (intADC_M1_S.at(3) * calib_M1_T4_S);
        
        float Edep_M2T1_S = (intADC_M2_S.at(0) * calib_M2_T1_S);
        float Edep_M2T2_S = (intADC_M2_S.at(1) * calib_M2_T2_S);
        float Edep_M2T3_S = (intADC_M2_S.at(2) * calib_M2_T3_S);
        float Edep_M2T4_S = (intADC_M2_S.at(3) * calib_M2_T4_S);
        
        float Edep_M3T1_S = (intADC_M3_S.at(0) * calib_M3_T1_S);
        float Edep_M3T2_S = (intADC_M3_S.at(1) * calib_M3_T2_S);
        float Edep_M3T3_S = (intADC_M3_S.at(2) * calib_M3_T3_S);
        float Edep_M3T4_S = (intADC_M3_S.at(3) * calib_M3_T4_S);
        
        float Edep_M4T1_S = (intADC_M4_S.at(0) * calib_M4_T1_S);
        float Edep_M4T2_S = (intADC_M4_S.at(1) * calib_M4_T2_S);
        float Edep_M4T3_S = (intADC_M4_S.at(2) * calib_M4_T3_S);
        float Edep_M4T4_S = (intADC_M4_S.at(3) * calib_M4_T4_S);
        
        float Edep_M5T1_S = (intADC_M5_S.at(0) * calib_M5_T1_S);
        float Edep_M5T2_S = (intADC_M5_S.at(1) * calib_M5_T2_S);
        float Edep_M5T3_S = (intADC_M5_S.at(2) * calib_M5_T3_S);
        float Edep_M5T4_S = (intADC_M5_S.at(3) * calib_M5_T4_S);
        
        float Edep_M6T1_S = (intADC_M6_S.at(0) * calib_M6_T1_S);
        float Edep_M6T2_S = (intADC_M6_S.at(1) * calib_M6_T2_S);
        float Edep_M6T3_S = (intADC_M6_S.at(2) * calib_M6_T3_S);
        float Edep_M6T4_S = (intADC_M6_S.at(3) * calib_M6_T4_S);
        
        float Edep_M7T1_S = (intADC_M7_S.at(0) * calib_M7_T1_S);
        float Edep_M7T2_S = (intADC_M7_S.at(1) * calib_M7_T2_S);
        float Edep_M7T3_S = (intADC_M7_S.at(2) * calib_M7_T3_S);
        float Edep_M7T4_S = (intADC_M7_S.at(3) * calib_M7_T4_S);
        
        float Edep_M8T1_S = (intADC_M8_S.at(0) * calib_M8_T1_S);
        float Edep_M8T2_S = (intADC_M8_S.at(1) * calib_M8_T2_S);
        float Edep_M8T3_S = (intADC_M8_S.at(2) * calib_M8_T3_S);
        float Edep_M8T4_S = (intADC_M8_S.at(3) * calib_M8_T4_S);
        
        float Edep_M9T1_S = (intADC_M9_S.at(0) * calib_M9_T1_S);
        float Edep_M9T2_S = (intADC_M9_S.at(1) * calib_M9_T2_S);
        float Edep_M9T3_S = (intADC_M9_S.at(2) * calib_M9_T3_S);
        float Edep_M9T4_S = (intADC_M9_S.at(3) * calib_M9_T4_S);
        
        float Edep_M1_S = Edep_M1T1_S + Edep_M1T2_S + Edep_M1T3_S + Edep_M1T4_S;
        float Edep_M2_S = Edep_M2T1_S + Edep_M2T2_S + Edep_M2T3_S + Edep_M2T4_S;
        float Edep_M3_S = Edep_M3T1_S + Edep_M3T2_S + Edep_M3T3_S + Edep_M3T4_S;
        float Edep_M4_S = Edep_M4T1_S + Edep_M4T2_S + Edep_M4T3_S + Edep_M4T4_S;
        float Edep_M5_S = Edep_M5T1_S + Edep_M5T2_S + Edep_M5T3_S + Edep_M5T4_S;
        float Edep_M6_S = Edep_M6T1_S + Edep_M6T2_S + Edep_M6T3_S + Edep_M6T4_S;
        float Edep_M7_S = Edep_M7T1_S + Edep_M7T2_S + Edep_M7T3_S + Edep_M7T4_S;
        float Edep_M8_S = Edep_M8T1_S + Edep_M8T2_S + Edep_M8T3_S + Edep_M8T4_S;
        float Edep_M9_S = Edep_M9T1_S + Edep_M9T2_S + Edep_M9T3_S + Edep_M9T4_S;
        

        float Edep_M12_S = Edep_M1_S + Edep_M2_S ;
        float Edep_M23_S = Edep_M2_S + Edep_M3_S ;
        float Edep_M34_S = Edep_M3_S + Edep_M4_S ;
        float Edep_M45_S = Edep_M4_S + Edep_M5_S ;
        float Edep_M56_S = Edep_M5_S + Edep_M6_S ;
        float Edep_M67_S = Edep_M6_S + Edep_M7_S ;
        float Edep_M78_S = Edep_M7_S + Edep_M8_S ;
        float Edep_M89_S = Edep_M8_S + Edep_M9_S ;
        float Edep_M91_S = Edep_M9_S + Edep_M1_S ;


        float totalEdep_S = Edep_M1_S +
                            Edep_M2_S +
                            Edep_M3_S +
                            Edep_M4_S +
                            Edep_M5_S +
                            Edep_M6_S +
                            Edep_M7_S +
                            Edep_M8_S +
                            Edep_M9_S;

        float totalEdep_S_LC_compensated = totalEdep_S + Edep_LC;
        
        float totalEdep_Comb = totalEdep_C + totalEdep_S;
        float totalEdep_Comb_LC_compensated = totalEdep_C + totalEdep_S_LC_compensated;
        
        float totalEdepScaled_C    = totalEdep_C * scaleFactor_C;
        float totalEdepScaled_S    = totalEdep_S * scaleFactor_S;

        float totalEdepScaled_S_LC_compensated = totalEdepScaled_S + Edep_LC;
        float totalEdepScaled_Comb = totalEdepScaled_C + totalEdepScaled_S;
        float totalEdepScaled_Comb_LC_compensated = totalEdepScaled_C + totalEdepScaled_S_LC_compensated;
        
        hist_M1_T1_C->Fill(intADC_M1_C.at(0));
        hist_M1_T2_C->Fill(intADC_M1_C.at(1));
        hist_M1_T3_C->Fill(intADC_M1_C.at(2));
        hist_M1_T4_C->Fill(intADC_M1_C.at(3));
        
        hist_M2_T1_C->Fill(intADC_M2_C.at(0));
        hist_M2_T2_C->Fill(intADC_M2_C.at(1));
        hist_M2_T3_C->Fill(intADC_M2_C.at(2));
        hist_M2_T4_C->Fill(intADC_M2_C.at(3));
        
        hist_M3_T1_C->Fill(intADC_M3_C.at(0));
        hist_M3_T2_C->Fill(intADC_M3_C.at(1));
        hist_M3_T3_C->Fill(intADC_M3_C.at(2));
        hist_M3_T4_C->Fill(intADC_M3_C.at(3));
        
        hist_M4_T1_C->Fill(intADC_M4_C.at(0));
        hist_M4_T2_C->Fill(intADC_M4_C.at(1));
        hist_M4_T3_C->Fill(intADC_M4_C.at(2));
        hist_M4_T4_C->Fill(intADC_M4_C.at(3));
        
        hist_M5_T1_C->Fill(intADC_M5_C.at(0));
        hist_M5_T2_C->Fill(intADC_M5_C.at(1));
        hist_M5_T3_C->Fill(intADC_M5_C.at(2));
        hist_M5_T4_C->Fill(intADC_M5_C.at(3));
        
        hist_M6_T1_C->Fill(intADC_M6_C.at(0));
        hist_M6_T2_C->Fill(intADC_M6_C.at(1));
        hist_M6_T3_C->Fill(intADC_M6_C.at(2));
        hist_M6_T4_C->Fill(intADC_M6_C.at(3));
        
        hist_M7_T1_C->Fill(intADC_M7_C.at(0));
        hist_M7_T2_C->Fill(intADC_M7_C.at(1));
        hist_M7_T3_C->Fill(intADC_M7_C.at(2));
        hist_M7_T4_C->Fill(intADC_M7_C.at(3));
        
        hist_M8_T1_C->Fill(intADC_M8_C.at(0));
        hist_M8_T2_C->Fill(intADC_M8_C.at(1));
        hist_M8_T3_C->Fill(intADC_M8_C.at(2));
        hist_M8_T4_C->Fill(intADC_M8_C.at(3));
        
        hist_M9_T1_C->Fill(intADC_M9_C.at(0));
        hist_M9_T2_C->Fill(intADC_M9_C.at(1));
        hist_M9_T3_C->Fill(intADC_M9_C.at(2));
        hist_M9_T4_C->Fill(intADC_M9_C.at(3));
        
        hist_M1_T1_S->Fill(intADC_M1_S.at(0));
        hist_M1_T2_S->Fill(intADC_M1_S.at(1));
        hist_M1_T3_S->Fill(intADC_M1_S.at(2));
        hist_M1_T4_S->Fill(intADC_M1_S.at(3));
        
        hist_M2_T1_S->Fill(intADC_M2_S.at(0));
        hist_M2_T2_S->Fill(intADC_M2_S.at(1));
        hist_M2_T3_S->Fill(intADC_M2_S.at(2));
        hist_M2_T4_S->Fill(intADC_M2_S.at(3));
        
        hist_M3_T1_S->Fill(intADC_M3_S.at(0));
        hist_M3_T2_S->Fill(intADC_M3_S.at(1));
        hist_M3_T3_S->Fill(intADC_M3_S.at(2));
        hist_M3_T4_S->Fill(intADC_M3_S.at(3));
        
        hist_M4_T1_S->Fill(intADC_M4_S.at(0));
        hist_M4_T2_S->Fill(intADC_M4_S.at(1));
        hist_M4_T3_S->Fill(intADC_M4_S.at(2));
        hist_M4_T4_S->Fill(intADC_M4_S.at(3));
        
        hist_M5_T1_S->Fill(intADC_M5_S.at(0));
        hist_M5_T2_S->Fill(intADC_M5_S.at(1));
        hist_M5_T3_S->Fill(intADC_M5_S.at(2));
        hist_M5_T4_S->Fill(intADC_M5_S.at(3));
        
        hist_M6_T1_S->Fill(intADC_M6_S.at(0));
        hist_M6_T2_S->Fill(intADC_M6_S.at(1));
        hist_M6_T3_S->Fill(intADC_M6_S.at(2));
        hist_M6_T4_S->Fill(intADC_M6_S.at(3));
        
        hist_M7_T1_S->Fill(intADC_M7_S.at(0));
        hist_M7_T2_S->Fill(intADC_M7_S.at(1));
        hist_M7_T3_S->Fill(intADC_M7_S.at(2));
        hist_M7_T4_S->Fill(intADC_M7_S.at(3));
        
        hist_M8_T1_S->Fill(intADC_M8_S.at(0));
        hist_M8_T2_S->Fill(intADC_M8_S.at(1));
        hist_M8_T3_S->Fill(intADC_M8_S.at(2));
        hist_M8_T4_S->Fill(intADC_M8_S.at(3));
        
        hist_M9_T1_S->Fill(intADC_M9_S.at(0));
        hist_M9_T2_S->Fill(intADC_M9_S.at(1));
        hist_M9_T3_S->Fill(intADC_M9_S.at(2));
        hist_M9_T4_S->Fill(intADC_M9_S.at(3));
        
        hist_Edep_M1_T1_C->Fill(Edep_M1T1_C);
        hist_Edep_M1_T2_C->Fill(Edep_M1T2_C);
        hist_Edep_M1_T3_C->Fill(Edep_M1T3_C);
        hist_Edep_M1_T4_C->Fill(Edep_M1T4_C);
        
        hist_Edep_M2_T1_C->Fill(Edep_M2T1_C);
        hist_Edep_M2_T2_C->Fill(Edep_M2T2_C);
        hist_Edep_M2_T3_C->Fill(Edep_M2T3_C);
        hist_Edep_M2_T4_C->Fill(Edep_M2T4_C);
        
        hist_Edep_M3_T1_C->Fill(Edep_M3T1_C);
        hist_Edep_M3_T2_C->Fill(Edep_M3T2_C);
        hist_Edep_M3_T3_C->Fill(Edep_M3T3_C);
        hist_Edep_M3_T4_C->Fill(Edep_M3T4_C);
        
        hist_Edep_M4_T1_C->Fill(Edep_M4T1_C);
        hist_Edep_M4_T2_C->Fill(Edep_M4T2_C);
        hist_Edep_M4_T3_C->Fill(Edep_M4T3_C);
        hist_Edep_M4_T4_C->Fill(Edep_M4T4_C);
        
        hist_Edep_M5_T1_C->Fill(Edep_M5T1_C);
        hist_Edep_M5_T2_C->Fill(Edep_M5T2_C);
        hist_Edep_M5_T3_C->Fill(Edep_M5T3_C);
        hist_Edep_M5_T4_C->Fill(Edep_M5T4_C);
        
        hist_Edep_M6_T1_C->Fill(Edep_M6T1_C);
        hist_Edep_M6_T2_C->Fill(Edep_M6T2_C);
        hist_Edep_M6_T3_C->Fill(Edep_M6T3_C);
        hist_Edep_M6_T4_C->Fill(Edep_M6T4_C);
        
        hist_Edep_M7_T1_C->Fill(Edep_M7T1_C);
        hist_Edep_M7_T2_C->Fill(Edep_M7T2_C);
        hist_Edep_M7_T3_C->Fill(Edep_M7T3_C);
        hist_Edep_M7_T4_C->Fill(Edep_M7T4_C);
        
        hist_Edep_M8_T1_C->Fill(Edep_M8T1_C);
        hist_Edep_M8_T2_C->Fill(Edep_M8T2_C);
        hist_Edep_M8_T3_C->Fill(Edep_M8T3_C);
        hist_Edep_M8_T4_C->Fill(Edep_M8T4_C);
        
        hist_Edep_M9_T1_C->Fill(Edep_M9T1_C);
        hist_Edep_M9_T2_C->Fill(Edep_M9T2_C);
        hist_Edep_M9_T3_C->Fill(Edep_M9T3_C);
        hist_Edep_M9_T4_C->Fill(Edep_M9T4_C);
        
        hist_Edep_M1_C->Fill(Edep_M1_C);
        hist_Edep_M2_C->Fill(Edep_M2_C);
        hist_Edep_M3_C->Fill(Edep_M3_C);
        hist_Edep_M4_C->Fill(Edep_M4_C);
        hist_Edep_M5_C->Fill(Edep_M5_C);
        hist_Edep_M6_C->Fill(Edep_M6_C);
        hist_Edep_M7_C->Fill(Edep_M7_C);
        hist_Edep_M8_C->Fill(Edep_M8_C);
        hist_Edep_M9_C->Fill(Edep_M9_C);
        
        hist_Edep_M1_T1_S->Fill(Edep_M1T1_S);
        hist_Edep_M1_T2_S->Fill(Edep_M1T2_S);
        hist_Edep_M1_T3_S->Fill(Edep_M1T3_S);
        hist_Edep_M1_T4_S->Fill(Edep_M1T4_S);
        
        hist_Edep_M2_T1_S->Fill(Edep_M2T1_S);
        hist_Edep_M2_T2_S->Fill(Edep_M2T2_S);
        hist_Edep_M2_T3_S->Fill(Edep_M2T3_S);
        hist_Edep_M2_T4_S->Fill(Edep_M2T4_S);
        
        hist_Edep_M3_T1_S->Fill(Edep_M3T1_S);
        hist_Edep_M3_T2_S->Fill(Edep_M3T2_S);
        hist_Edep_M3_T3_S->Fill(Edep_M3T3_S);
        hist_Edep_M3_T4_S->Fill(Edep_M3T4_S);
        
        hist_Edep_M4_T1_S->Fill(Edep_M4T1_S);
        hist_Edep_M4_T2_S->Fill(Edep_M4T2_S);
        hist_Edep_M4_T3_S->Fill(Edep_M4T3_S);
        hist_Edep_M4_T4_S->Fill(Edep_M4T4_S);
        
        hist_Edep_M5_T1_S->Fill(Edep_M5T1_S);
        hist_Edep_M5_T2_S->Fill(Edep_M5T2_S);
        hist_Edep_M5_T3_S->Fill(Edep_M5T3_S);
        hist_Edep_M5_T4_S->Fill(Edep_M5T4_S);
        
        hist_Edep_M6_T1_S->Fill(Edep_M6T1_S);
        hist_Edep_M6_T2_S->Fill(Edep_M6T2_S);
        hist_Edep_M6_T3_S->Fill(Edep_M6T3_S);
        hist_Edep_M6_T4_S->Fill(Edep_M6T4_S);
        
        hist_Edep_M7_T1_S->Fill(Edep_M7T1_S);
        hist_Edep_M7_T2_S->Fill(Edep_M7T2_S);
        hist_Edep_M7_T3_S->Fill(Edep_M7T3_S);
        hist_Edep_M7_T4_S->Fill(Edep_M7T4_S);
        
        hist_Edep_M8_T1_S->Fill(Edep_M8T1_S);
        hist_Edep_M8_T2_S->Fill(Edep_M8T2_S);
        hist_Edep_M8_T3_S->Fill(Edep_M8T3_S);
        hist_Edep_M8_T4_S->Fill(Edep_M8T4_S);
        
        hist_Edep_M9_T1_S->Fill(Edep_M9T1_S);
        hist_Edep_M9_T2_S->Fill(Edep_M9T2_S);
        hist_Edep_M9_T3_S->Fill(Edep_M9T3_S);
        hist_Edep_M9_T4_S->Fill(Edep_M9T4_S);
        
        hist_Edep_M1_S->Fill(Edep_M1_S);
        hist_Edep_M2_S->Fill(Edep_M2_S);
        hist_Edep_M3_S->Fill(Edep_M3_S);
        hist_Edep_M4_S->Fill(Edep_M4_S);
        hist_Edep_M5_S->Fill(Edep_M5_S);
        hist_Edep_M6_S->Fill(Edep_M6_S);
        hist_Edep_M7_S->Fill(Edep_M7_S);
        hist_Edep_M8_S->Fill(Edep_M8_S);
        hist_Edep_M9_S->Fill(Edep_M9_S);
        
        hist_totalEdep_C   ->Fill(totalEdep_C);
        hist_totalEdep_S   ->Fill(totalEdep_S);
        hist_totalEdep_S_LC_compensated ->Fill(totalEdep_S_LC_compensated);
        hist_totalEdep_Comb->Fill(totalEdep_Comb);
        hist_totalEdep_Comb_LC_compensated->Fill(totalEdep_Comb_LC_compensated);
        
        hist_totalEdepScaled_C   ->Fill(totalEdepScaled_C   );
        hist_totalEdepScaled_S   ->Fill(totalEdepScaled_S   );
        hist_totalEdepScaled_S_LC_compensated ->Fill(totalEdepScaled_S_LC_compensated);
        hist_totalEdepScaled_Comb->Fill(totalEdepScaled_Comb);
        hist_totalEdepScaled_Comb_LC_compensated->Fill(totalEdepScaled_Comb_LC_compensated);
        
        hist_CC1->Fill(signal_CC1);
        hist_CC2->Fill(signal_CC2);
        
        hist_PS->Fill(signal_PS);
        hist_MC->Fill(signal_MC);
        hist_TC->Fill(signal_TC);
        
        hist_LC2 ->Fill(intADC_LC2 );
        hist_LC3 ->Fill(intADC_LC3 );
        hist_LC4 ->Fill(intADC_LC4 );
        hist_LC5 ->Fill(intADC_LC5 );
        hist_LC7 ->Fill(intADC_LC7 );
        hist_LC8 ->Fill(intADC_LC8 );
        hist_LC9 ->Fill(intADC_LC9 );
        hist_LC10->Fill(intADC_LC10);
        hist_LC11->Fill(intADC_LC11);
        hist_LC12->Fill(intADC_LC12);
        hist_LC13->Fill(intADC_LC13);
        hist_LC14->Fill(intADC_LC14);
        hist_LC15->Fill(intADC_LC15);
        hist_LC16->Fill(intADC_LC16);
        hist_LC19->Fill(intADC_LC19);
        hist_LC20->Fill(intADC_LC20);
        
        hist_DWC1_pos_corrected  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_corrected  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1)); 
        hist_DWC_x_corr_corrected->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_corrected->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));
        
        // Event selection here!!
        // Require C1, C2 to have intADC larger than cut
        // if ( !((cut_CC1 < signal_CC1) && (cut_CC2 < signal_CC2)) ) continue;
        if ( !(dwcCorrelationCut(DWC1_corrected_pos, DWC2_corrected_pos, cut_DWC)) ) continue;
        if ( signal_PS < cut_PS ) continue; // Select above 3 mip peak
        if ( signal_MC > cut_MC ) continue; // Select only pedestals
        
        // Do whatever need to be done after PID
        hist_M1_T1_C_after->Fill(intADC_M1_C.at(0));
        hist_M1_T2_C_after->Fill(intADC_M1_C.at(1));
        hist_M1_T3_C_after->Fill(intADC_M1_C.at(2));
        hist_M1_T4_C_after->Fill(intADC_M1_C.at(3));
        
        hist_M2_T1_C_after->Fill(intADC_M2_C.at(0));
        hist_M2_T2_C_after->Fill(intADC_M2_C.at(1));
        hist_M2_T3_C_after->Fill(intADC_M2_C.at(2));
        hist_M2_T4_C_after->Fill(intADC_M2_C.at(3));
        
        hist_M3_T1_C_after->Fill(intADC_M3_C.at(0));
        hist_M3_T2_C_after->Fill(intADC_M3_C.at(1));
        hist_M3_T3_C_after->Fill(intADC_M3_C.at(2));
        hist_M3_T4_C_after->Fill(intADC_M3_C.at(3));
        
        hist_M4_T1_C_after->Fill(intADC_M4_C.at(0));
        hist_M4_T2_C_after->Fill(intADC_M4_C.at(1));
        hist_M4_T3_C_after->Fill(intADC_M4_C.at(2));
        hist_M4_T4_C_after->Fill(intADC_M4_C.at(3));
        
        hist_M5_T1_C_after->Fill(intADC_M5_C.at(0));
        hist_M5_T2_C_after->Fill(intADC_M5_C.at(1));
        hist_M5_T3_C_after->Fill(intADC_M5_C.at(2));
        hist_M5_T4_C_after->Fill(intADC_M5_C.at(3));
        
        hist_M6_T1_C_after->Fill(intADC_M6_C.at(0));
        hist_M6_T2_C_after->Fill(intADC_M6_C.at(1));
        hist_M6_T3_C_after->Fill(intADC_M6_C.at(2));
        hist_M6_T4_C_after->Fill(intADC_M6_C.at(3));
        
        hist_M7_T1_C_after->Fill(intADC_M7_C.at(0));
        hist_M7_T2_C_after->Fill(intADC_M7_C.at(1));
        hist_M7_T3_C_after->Fill(intADC_M7_C.at(2));
        hist_M7_T4_C_after->Fill(intADC_M7_C.at(3));
        
        hist_M8_T1_C_after->Fill(intADC_M8_C.at(0));
        hist_M8_T2_C_after->Fill(intADC_M8_C.at(1));
        hist_M8_T3_C_after->Fill(intADC_M8_C.at(2));
        hist_M8_T4_C_after->Fill(intADC_M8_C.at(3));
        
        hist_M9_T1_C_after->Fill(intADC_M9_C.at(0));
        hist_M9_T2_C_after->Fill(intADC_M9_C.at(1));
        hist_M9_T3_C_after->Fill(intADC_M9_C.at(2));
        hist_M9_T4_C_after->Fill(intADC_M9_C.at(3));
        
        hist_M1_T1_S_after->Fill(intADC_M1_S.at(0));
        hist_M1_T2_S_after->Fill(intADC_M1_S.at(1));
        hist_M1_T3_S_after->Fill(intADC_M1_S.at(2));
        hist_M1_T4_S_after->Fill(intADC_M1_S.at(3));
        
        hist_M2_T1_S_after->Fill(intADC_M2_S.at(0));
        hist_M2_T2_S_after->Fill(intADC_M2_S.at(1));
        hist_M2_T3_S_after->Fill(intADC_M2_S.at(2));
        hist_M2_T4_S_after->Fill(intADC_M2_S.at(3));
        
        hist_M3_T1_S_after->Fill(intADC_M3_S.at(0));
        hist_M3_T2_S_after->Fill(intADC_M3_S.at(1));
        hist_M3_T3_S_after->Fill(intADC_M3_S.at(2));
        hist_M3_T4_S_after->Fill(intADC_M3_S.at(3));
        
        hist_M4_T1_S_after->Fill(intADC_M4_S.at(0));
        hist_M4_T2_S_after->Fill(intADC_M4_S.at(1));
        hist_M4_T3_S_after->Fill(intADC_M4_S.at(2));
        hist_M4_T4_S_after->Fill(intADC_M4_S.at(3));
        
        hist_M5_T1_S_after->Fill(intADC_M5_S.at(0));
        hist_M5_T2_S_after->Fill(intADC_M5_S.at(1));
        hist_M5_T3_S_after->Fill(intADC_M5_S.at(2));
        hist_M5_T4_S_after->Fill(intADC_M5_S.at(3));
        
        hist_M6_T1_S_after->Fill(intADC_M6_S.at(0));
        hist_M6_T2_S_after->Fill(intADC_M6_S.at(1));
        hist_M6_T3_S_after->Fill(intADC_M6_S.at(2));
        hist_M6_T4_S_after->Fill(intADC_M6_S.at(3));
        
        hist_M7_T1_S_after->Fill(intADC_M7_S.at(0));
        hist_M7_T2_S_after->Fill(intADC_M7_S.at(1));
        hist_M7_T3_S_after->Fill(intADC_M7_S.at(2));
        hist_M7_T4_S_after->Fill(intADC_M7_S.at(3));
        
        hist_M8_T1_S_after->Fill(intADC_M8_S.at(0));
        hist_M8_T2_S_after->Fill(intADC_M8_S.at(1));
        hist_M8_T3_S_after->Fill(intADC_M8_S.at(2));
        hist_M8_T4_S_after->Fill(intADC_M8_S.at(3));
        
        hist_M9_T1_S_after->Fill(intADC_M9_S.at(0));
        hist_M9_T2_S_after->Fill(intADC_M9_S.at(1));
        hist_M9_T3_S_after->Fill(intADC_M9_S.at(2));
        hist_M9_T4_S_after->Fill(intADC_M9_S.at(3));
        
        hist_Edep_M1_T1_C_after->Fill(Edep_M1T1_C);
        hist_Edep_M1_T2_C_after->Fill(Edep_M1T2_C);
        hist_Edep_M1_T3_C_after->Fill(Edep_M1T3_C);
        hist_Edep_M1_T4_C_after->Fill(Edep_M1T4_C);
        
        hist_Edep_M2_T1_C_after->Fill(Edep_M2T1_C);
        hist_Edep_M2_T2_C_after->Fill(Edep_M2T2_C);
        hist_Edep_M2_T3_C_after->Fill(Edep_M2T3_C);
        hist_Edep_M2_T4_C_after->Fill(Edep_M2T4_C);
        
        hist_Edep_M3_T1_C_after->Fill(Edep_M3T1_C);
        hist_Edep_M3_T2_C_after->Fill(Edep_M3T2_C);
        hist_Edep_M3_T3_C_after->Fill(Edep_M3T3_C);
        hist_Edep_M3_T4_C_after->Fill(Edep_M3T4_C);
        
        hist_Edep_M4_T1_C_after->Fill(Edep_M4T1_C);
        hist_Edep_M4_T2_C_after->Fill(Edep_M4T2_C);
        hist_Edep_M4_T3_C_after->Fill(Edep_M4T3_C);
        hist_Edep_M4_T4_C_after->Fill(Edep_M4T4_C);
        
        hist_Edep_M5_T1_C_after->Fill(Edep_M5T1_C);
        hist_Edep_M5_T2_C_after->Fill(Edep_M5T2_C);
        hist_Edep_M5_T3_C_after->Fill(Edep_M5T3_C);
        hist_Edep_M5_T4_C_after->Fill(Edep_M5T4_C);
        
        hist_Edep_M6_T1_C_after->Fill(Edep_M6T1_C);
        hist_Edep_M6_T2_C_after->Fill(Edep_M6T2_C);
        hist_Edep_M6_T3_C_after->Fill(Edep_M6T3_C);
        hist_Edep_M6_T4_C_after->Fill(Edep_M6T4_C);
        
        hist_Edep_M7_T1_C_after->Fill(Edep_M7T1_C);
        hist_Edep_M7_T2_C_after->Fill(Edep_M7T2_C);
        hist_Edep_M7_T3_C_after->Fill(Edep_M7T3_C);
        hist_Edep_M7_T4_C_after->Fill(Edep_M7T4_C);
        
        hist_Edep_M8_T1_C_after->Fill(Edep_M8T1_C);
        hist_Edep_M8_T2_C_after->Fill(Edep_M8T2_C);
        hist_Edep_M8_T3_C_after->Fill(Edep_M8T3_C);
        hist_Edep_M8_T4_C_after->Fill(Edep_M8T4_C);
        
        hist_Edep_M9_T1_C_after->Fill(Edep_M9T1_C);
        hist_Edep_M9_T2_C_after->Fill(Edep_M9T2_C);
        hist_Edep_M9_T3_C_after->Fill(Edep_M9T3_C);
        hist_Edep_M9_T4_C_after->Fill(Edep_M9T4_C);
        
        hist_Edep_M1_C_after->Fill(Edep_M1_C);
        hist_Edep_M2_C_after->Fill(Edep_M2_C);
        hist_Edep_M3_C_after->Fill(Edep_M3_C);
        hist_Edep_M4_C_after->Fill(Edep_M4_C);
        hist_Edep_M5_C_after->Fill(Edep_M5_C);
        hist_Edep_M6_C_after->Fill(Edep_M6_C);
        hist_Edep_M7_C_after->Fill(Edep_M7_C);
        hist_Edep_M8_C_after->Fill(Edep_M8_C);
        hist_Edep_M9_C_after->Fill(Edep_M9_C);
        
        hist_Edep_M12_C_after->Fill(Edep_M12_C);
        hist_Edep_M23_C_after->Fill(Edep_M23_C);
        hist_Edep_M34_C_after->Fill(Edep_M34_C);
        hist_Edep_M45_C_after->Fill(Edep_M45_C);
        hist_Edep_M56_C_after->Fill(Edep_M56_C);
        hist_Edep_M67_C_after->Fill(Edep_M67_C);
        hist_Edep_M78_C_after->Fill(Edep_M78_C);
        hist_Edep_M89_C_after->Fill(Edep_M89_C);
        hist_Edep_M91_C_after->Fill(Edep_M91_C);
        
        hist_Edep_M1_T1_S_after->Fill(Edep_M1T1_S);
        hist_Edep_M1_T2_S_after->Fill(Edep_M1T2_S);
        hist_Edep_M1_T3_S_after->Fill(Edep_M1T3_S);
        hist_Edep_M1_T4_S_after->Fill(Edep_M1T4_S);
        
        hist_Edep_M2_T1_S_after->Fill(Edep_M2T1_S);
        hist_Edep_M2_T2_S_after->Fill(Edep_M2T2_S);
        hist_Edep_M2_T3_S_after->Fill(Edep_M2T3_S);
        hist_Edep_M2_T4_S_after->Fill(Edep_M2T4_S);
        
        hist_Edep_M3_T1_S_after->Fill(Edep_M3T1_S);
        hist_Edep_M3_T2_S_after->Fill(Edep_M3T2_S);
        hist_Edep_M3_T3_S_after->Fill(Edep_M3T3_S);
        hist_Edep_M3_T4_S_after->Fill(Edep_M3T4_S);
        
        hist_Edep_M4_T1_S_after->Fill(Edep_M4T1_S);
        hist_Edep_M4_T2_S_after->Fill(Edep_M4T2_S);
        hist_Edep_M4_T3_S_after->Fill(Edep_M4T3_S);
        hist_Edep_M4_T4_S_after->Fill(Edep_M4T4_S);
        
        hist_Edep_M5_T1_S_after->Fill(Edep_M5T1_S);
        hist_Edep_M5_T2_S_after->Fill(Edep_M5T2_S);
        hist_Edep_M5_T3_S_after->Fill(Edep_M5T3_S);
        hist_Edep_M5_T4_S_after->Fill(Edep_M5T4_S);
        
        hist_Edep_M6_T1_S_after->Fill(Edep_M6T1_S);
        hist_Edep_M6_T2_S_after->Fill(Edep_M6T2_S);
        hist_Edep_M6_T3_S_after->Fill(Edep_M6T3_S);
        hist_Edep_M6_T4_S_after->Fill(Edep_M6T4_S);
        
        hist_Edep_M7_T1_S_after->Fill(Edep_M7T1_S);
        hist_Edep_M7_T2_S_after->Fill(Edep_M7T2_S);
        hist_Edep_M7_T3_S_after->Fill(Edep_M7T3_S);
        hist_Edep_M7_T4_S_after->Fill(Edep_M7T4_S);
        
        hist_Edep_M8_T1_S_after->Fill(Edep_M8T1_S);
        hist_Edep_M8_T2_S_after->Fill(Edep_M8T2_S);
        hist_Edep_M8_T3_S_after->Fill(Edep_M8T3_S);
        hist_Edep_M8_T4_S_after->Fill(Edep_M8T4_S);
        
        hist_Edep_M9_T1_S_after->Fill(Edep_M9T1_S);
        hist_Edep_M9_T2_S_after->Fill(Edep_M9T2_S);
        hist_Edep_M9_T3_S_after->Fill(Edep_M9T3_S);
        hist_Edep_M9_T4_S_after->Fill(Edep_M9T4_S);
        
        hist_Edep_M1_S_after->Fill(Edep_M1_S);
        hist_Edep_M2_S_after->Fill(Edep_M2_S);
        hist_Edep_M3_S_after->Fill(Edep_M3_S);
        hist_Edep_M4_S_after->Fill(Edep_M4_S);
        hist_Edep_M5_S_after->Fill(Edep_M5_S);
        hist_Edep_M6_S_after->Fill(Edep_M6_S);
        hist_Edep_M7_S_after->Fill(Edep_M7_S);
        hist_Edep_M8_S_after->Fill(Edep_M8_S);
        hist_Edep_M9_S_after->Fill(Edep_M9_S);
        
        hist_Edep_M12_S_after->Fill(Edep_M12_S);
        hist_Edep_M23_S_after->Fill(Edep_M23_S);
        hist_Edep_M34_S_after->Fill(Edep_M34_S);
        hist_Edep_M45_S_after->Fill(Edep_M45_S);
        hist_Edep_M56_S_after->Fill(Edep_M56_S);
        hist_Edep_M67_S_after->Fill(Edep_M67_S);
        hist_Edep_M78_S_after->Fill(Edep_M78_S);
        hist_Edep_M89_S_after->Fill(Edep_M89_S);
        hist_Edep_M91_S_after->Fill(Edep_M91_S);
        
        hist_totalEdep_C_after   ->Fill(totalEdep_C);
        hist_totalEdep_S_after   ->Fill(totalEdep_S);
        hist_totalEdep_S_LC_compensated_after ->Fill(totalEdep_S_LC_compensated);
        hist_totalEdep_Comb_after->Fill(totalEdep_Comb);
        hist_totalEdep_Comb_LC_compensated_after->Fill(totalEdep_Comb_LC_compensated);
        
        hist_totalEdepScaled_C_after   ->Fill(totalEdepScaled_C);
        hist_totalEdepScaled_S_after   ->Fill(totalEdepScaled_S);
        hist_totalEdepScaled_S_LC_compensated_after ->Fill(totalEdepScaled_S_LC_compensated);
        hist_totalEdepScaled_Comb_after->Fill(totalEdepScaled_Comb);
        hist_totalEdepScaled_Comb_LC_compensated_after->Fill(totalEdepScaled_Comb_LC_compensated);
        
        hist_CC1_after->Fill(signal_CC1);
        hist_CC2_after->Fill(signal_CC2);
        
        hist_PS_after->Fill(signal_PS);
        hist_MC_after->Fill(signal_MC);
        hist_TC_after->Fill(signal_TC);
        
        hist_LC2_after ->Fill(intADC_LC2 );
        hist_LC3_after ->Fill(intADC_LC3 );
        hist_LC4_after ->Fill(intADC_LC4 );
        hist_LC5_after ->Fill(intADC_LC5 );
        hist_LC7_after ->Fill(intADC_LC7 );
        hist_LC8_after ->Fill(intADC_LC8 );
        hist_LC9_after ->Fill(intADC_LC9 );
        hist_LC10_after->Fill(intADC_LC10);
        hist_LC11_after->Fill(intADC_LC11);
        hist_LC12_after->Fill(intADC_LC12);
        hist_LC13_after->Fill(intADC_LC13);
        hist_LC14_after->Fill(intADC_LC14);
        hist_LC15_after->Fill(intADC_LC15);
        hist_LC16_after->Fill(intADC_LC16);
        hist_LC19_after->Fill(intADC_LC19);
        hist_LC20_after->Fill(intADC_LC20);
        
        hist_DWC1_pos_after  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_after  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1));
        hist_DWC_x_corr_after->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_after->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));
    }
    
    // Fit function for calculating Scale factor
    TF1 *fit_scale_C = new TF1("fit_scale_C","gaus",0,200);
    fit_scale_C->SetParameters(1,0,1);
    hist_totalEdep_C_after->Fit("fit_scale_C");
    
    TF1 *fit_scale_S = new TF1("fit_scale_S","gaus",0,200);
    fit_scale_S->SetParameters(1,0,1);
    hist_totalEdep_S_after->Fit("fit_scale_S");
    
    // Fit function for energy resolution fit
    TF1 *fit_reso_C = new TF1("fit_reso_C","gaus",0,200);
    fit_reso_C->SetParameters(1,0,1);
    hist_totalEdepScaled_C_after->Fit("fit_reso_C");
    
    TF1 *fit_reso_S = new TF1("fit_reso_S","gaus",0,200);
    fit_reso_S->SetParameters(1,0,1);
    hist_totalEdepScaled_S_after->Fit("fit_reso_S");
    
    TF1 *fit_reso_Comb = new TF1("fit_reso_Comb","gaus",0,400);
    fit_reso_Comb->SetParameters(1,0,1);
    hist_totalEdepScaled_Comb_after->Fit("fit_reso_Comb");
    
    // Output file
    std::string outFile = "./Calib_ADC/Calib_ADC_Run_" + std::to_string(fRunNum) +"_" +correctionMode +".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();
    
    fit_scale_C->Write();
    fit_scale_S->Write();
    
    fit_reso_C->Write();
    fit_reso_S->Write();
    fit_reso_Comb->Write();
    
    hist_M1_T1_C->Write();
    hist_M1_T2_C->Write();
    hist_M1_T3_C->Write();
    hist_M1_T4_C->Write();
    
    hist_M2_T1_C->Write();
    hist_M2_T2_C->Write();
    hist_M2_T3_C->Write();
    hist_M2_T4_C->Write();
    
    hist_M3_T1_C->Write();
    hist_M3_T2_C->Write();
    hist_M3_T3_C->Write();
    hist_M3_T4_C->Write();
    
    hist_M4_T1_C->Write();
    hist_M4_T2_C->Write();
    hist_M4_T3_C->Write();
    hist_M4_T4_C->Write();
    
    hist_M5_T1_C->Write();
    hist_M5_T2_C->Write();
    hist_M5_T3_C->Write();
    hist_M5_T4_C->Write();
    
    hist_M6_T1_C->Write();
    hist_M6_T2_C->Write();
    hist_M6_T3_C->Write();
    hist_M6_T4_C->Write();
    
    hist_M7_T1_C->Write();
    hist_M7_T2_C->Write();
    hist_M7_T3_C->Write();
    hist_M7_T4_C->Write();
    
    hist_M8_T1_C->Write();
    hist_M8_T2_C->Write();
    hist_M8_T3_C->Write();
    hist_M8_T4_C->Write();
    
    hist_M9_T1_C->Write();
    hist_M9_T2_C->Write();
    hist_M9_T3_C->Write();
    hist_M9_T4_C->Write();
    
    hist_M1_T1_S->Write();
    hist_M1_T2_S->Write();
    hist_M1_T3_S->Write();
    hist_M1_T4_S->Write();
    
    hist_M2_T1_S->Write();
    hist_M2_T2_S->Write();
    hist_M2_T3_S->Write();
    hist_M2_T4_S->Write();
    
    hist_M3_T1_S->Write();
    hist_M3_T2_S->Write();
    hist_M3_T3_S->Write();
    hist_M3_T4_S->Write();
    
    hist_M4_T1_S->Write();
    hist_M4_T2_S->Write();
    hist_M4_T3_S->Write();
    hist_M4_T4_S->Write();
    
    hist_M5_T1_S->Write();
    hist_M5_T2_S->Write();
    hist_M5_T3_S->Write();
    hist_M5_T4_S->Write();
    
    hist_M6_T1_S->Write();
    hist_M6_T2_S->Write();
    hist_M6_T3_S->Write();
    hist_M6_T4_S->Write();
    
    hist_M7_T1_S->Write();
    hist_M7_T2_S->Write();
    hist_M7_T3_S->Write();
    hist_M7_T4_S->Write();
    
    hist_M8_T1_S->Write();
    hist_M8_T2_S->Write();
    hist_M8_T3_S->Write();
    hist_M8_T4_S->Write();
    
    hist_M9_T1_S->Write();
    hist_M9_T2_S->Write();
    hist_M9_T3_S->Write();
    hist_M9_T4_S->Write();
    
    hist_Edep_M1_T1_C->Write();
    hist_Edep_M1_T2_C->Write();
    hist_Edep_M1_T3_C->Write();
    hist_Edep_M1_T4_C->Write();
    
    hist_Edep_M2_T1_C->Write();
    hist_Edep_M2_T2_C->Write();
    hist_Edep_M2_T3_C->Write();
    hist_Edep_M2_T4_C->Write();
    
    hist_Edep_M3_T1_C->Write();
    hist_Edep_M3_T2_C->Write();
    hist_Edep_M3_T3_C->Write();
    hist_Edep_M3_T4_C->Write();
    
    hist_Edep_M4_T1_C->Write();
    hist_Edep_M4_T2_C->Write();
    hist_Edep_M4_T3_C->Write();
    hist_Edep_M4_T4_C->Write();
    
    hist_Edep_M5_T1_C->Write();
    hist_Edep_M5_T2_C->Write();
    hist_Edep_M5_T3_C->Write();
    hist_Edep_M5_T4_C->Write();
    
    hist_Edep_M6_T1_C->Write();
    hist_Edep_M6_T2_C->Write();
    hist_Edep_M6_T3_C->Write();
    hist_Edep_M6_T4_C->Write();
    
    hist_Edep_M7_T1_C->Write();
    hist_Edep_M7_T2_C->Write();
    hist_Edep_M7_T3_C->Write();
    hist_Edep_M7_T4_C->Write();
    
    hist_Edep_M8_T1_C->Write();
    hist_Edep_M8_T2_C->Write();
    hist_Edep_M8_T3_C->Write();
    hist_Edep_M8_T4_C->Write();
    
    hist_Edep_M9_T1_C->Write();
    hist_Edep_M9_T2_C->Write();
    hist_Edep_M9_T3_C->Write();
    hist_Edep_M9_T4_C->Write();
    
    hist_Edep_M1_C->Write();
    hist_Edep_M2_C->Write();
    hist_Edep_M3_C->Write();
    hist_Edep_M4_C->Write();
    hist_Edep_M5_C->Write();
    hist_Edep_M6_C->Write();
    hist_Edep_M7_C->Write();
    hist_Edep_M8_C->Write();
    hist_Edep_M9_C->Write();
    
    hist_Edep_M1_T1_S->Write();
    hist_Edep_M1_T2_S->Write();
    hist_Edep_M1_T3_S->Write();
    hist_Edep_M1_T4_S->Write();
    
    hist_Edep_M2_T1_S->Write();
    hist_Edep_M2_T2_S->Write();
    hist_Edep_M2_T3_S->Write();
    hist_Edep_M2_T4_S->Write();
    
    hist_Edep_M3_T1_S->Write();
    hist_Edep_M3_T2_S->Write();
    hist_Edep_M3_T3_S->Write();
    hist_Edep_M3_T4_S->Write();
    
    hist_Edep_M4_T1_S->Write();
    hist_Edep_M4_T2_S->Write();
    hist_Edep_M4_T3_S->Write();
    hist_Edep_M4_T4_S->Write();
    
    hist_Edep_M5_T1_S->Write();
    hist_Edep_M5_T2_S->Write();
    hist_Edep_M5_T3_S->Write();
    hist_Edep_M5_T4_S->Write();
    
    hist_Edep_M6_T1_S->Write();
    hist_Edep_M6_T2_S->Write();
    hist_Edep_M6_T3_S->Write();
    hist_Edep_M6_T4_S->Write();
    
    hist_Edep_M7_T1_S->Write();
    hist_Edep_M7_T2_S->Write();
    hist_Edep_M7_T3_S->Write();
    hist_Edep_M7_T4_S->Write();
    
    hist_Edep_M8_T1_S->Write();
    hist_Edep_M8_T2_S->Write();
    hist_Edep_M8_T3_S->Write();
    hist_Edep_M8_T4_S->Write();
    
    hist_Edep_M9_T1_S->Write();
    hist_Edep_M9_T2_S->Write();
    hist_Edep_M9_T3_S->Write();
    hist_Edep_M9_T4_S->Write();
    
    hist_Edep_M1_S->Write();
    hist_Edep_M2_S->Write();
    hist_Edep_M3_S->Write();
    hist_Edep_M4_S->Write();
    hist_Edep_M5_S->Write();
    hist_Edep_M6_S->Write();
    hist_Edep_M7_S->Write();
    hist_Edep_M8_S->Write();
    hist_Edep_M9_S->Write();
    
    hist_totalEdep_C   ->Write();
    hist_totalEdep_S   ->Write();
    hist_totalEdep_S_LC_compensated ->Write();
    hist_totalEdep_Comb->Write();
    hist_totalEdep_Comb_LC_compensated->Write();
    
    hist_totalEdepScaled_C   ->Write();
    hist_totalEdepScaled_S   ->Write();
    hist_totalEdepScaled_S_LC_compensated ->Write();
    hist_totalEdepScaled_Comb->Write();
    hist_totalEdepScaled_Comb_LC_compensated->Write();
    
    hist_CC1->Write();
    hist_CC2->Write();
    
    hist_PS->Write();
    hist_MC->Write();
    hist_TC->Write();
    
    hist_LC2 ->Write();
    hist_LC3 ->Write();
    hist_LC4 ->Write();
    hist_LC5 ->Write();
    hist_LC7 ->Write();
    hist_LC8 ->Write();
    hist_LC9 ->Write();
    hist_LC10->Write();
    hist_LC11->Write();
    hist_LC12->Write();
    hist_LC13->Write();
    hist_LC14->Write();
    hist_LC15->Write();
    hist_LC16->Write();
    hist_LC19->Write();
    hist_LC20->Write();
    
    hist_DWC1_pos_corrected  ->Write();
    hist_DWC2_pos_corrected  ->Write();
    hist_DWC_x_corr_corrected->Write();
    hist_DWC_y_corr_corrected->Write();
    
    // Hist after PID
    hist_M1_T1_C_after->Write();
    hist_M1_T2_C_after->Write();
    hist_M1_T3_C_after->Write();
    hist_M1_T4_C_after->Write();
    
    hist_M2_T1_C_after->Write();
    hist_M2_T2_C_after->Write();
    hist_M2_T3_C_after->Write();
    hist_M2_T4_C_after->Write();
    
    hist_M3_T1_C_after->Write();
    hist_M3_T2_C_after->Write();
    hist_M3_T3_C_after->Write();
    hist_M3_T4_C_after->Write();
    
    hist_M4_T1_C_after->Write();
    hist_M4_T2_C_after->Write();
    hist_M4_T3_C_after->Write();
    hist_M4_T4_C_after->Write();
    
    hist_M5_T1_C_after->Write();
    hist_M5_T2_C_after->Write();
    hist_M5_T3_C_after->Write();
    hist_M5_T4_C_after->Write();
    
    hist_M6_T1_C_after->Write();
    hist_M6_T2_C_after->Write();
    hist_M6_T3_C_after->Write();
    hist_M6_T4_C_after->Write();
    
    hist_M7_T1_C_after->Write();
    hist_M7_T2_C_after->Write();
    hist_M7_T3_C_after->Write();
    hist_M7_T4_C_after->Write();
    
    hist_M8_T1_C_after->Write();
    hist_M8_T2_C_after->Write();
    hist_M8_T3_C_after->Write();
    hist_M8_T4_C_after->Write();
    
    hist_M9_T1_C_after->Write();
    hist_M9_T2_C_after->Write();
    hist_M9_T3_C_after->Write();
    hist_M9_T4_C_after->Write();
    
    hist_M1_T1_S_after->Write();
    hist_M1_T2_S_after->Write();
    hist_M1_T3_S_after->Write();
    hist_M1_T4_S_after->Write();
    
    hist_M2_T1_S_after->Write();
    hist_M2_T2_S_after->Write();
    hist_M2_T3_S_after->Write();
    hist_M2_T4_S_after->Write();
    
    hist_M3_T1_S_after->Write();
    hist_M3_T2_S_after->Write();
    hist_M3_T3_S_after->Write();
    hist_M3_T4_S_after->Write();
    
    hist_M4_T1_S_after->Write();
    hist_M4_T2_S_after->Write();
    hist_M4_T3_S_after->Write();
    hist_M4_T4_S_after->Write();
    
    hist_M5_T1_S_after->Write();
    hist_M5_T2_S_after->Write();
    hist_M5_T3_S_after->Write();
    hist_M5_T4_S_after->Write();
    
    hist_M6_T1_S_after->Write();
    hist_M6_T2_S_after->Write();
    hist_M6_T3_S_after->Write();
    hist_M6_T4_S_after->Write();
    
    hist_M7_T1_S_after->Write();
    hist_M7_T2_S_after->Write();
    hist_M7_T3_S_after->Write();
    hist_M7_T4_S_after->Write();
    
    hist_M8_T1_S_after->Write();
    hist_M8_T2_S_after->Write();
    hist_M8_T3_S_after->Write();
    hist_M8_T4_S_after->Write();
    
    hist_M9_T1_S_after->Write();
    hist_M9_T2_S_after->Write();
    hist_M9_T3_S_after->Write();
    hist_M9_T4_S_after->Write();
    
    hist_Edep_M1_T1_C_after->Write();
    hist_Edep_M1_T2_C_after->Write();
    hist_Edep_M1_T3_C_after->Write();
    hist_Edep_M1_T4_C_after->Write();
    
    hist_Edep_M2_T1_C_after->Write();
    hist_Edep_M2_T2_C_after->Write();
    hist_Edep_M2_T3_C_after->Write();
    hist_Edep_M2_T4_C_after->Write();
    
    hist_Edep_M3_T1_C_after->Write();
    hist_Edep_M3_T2_C_after->Write();
    hist_Edep_M3_T3_C_after->Write();
    hist_Edep_M3_T4_C_after->Write();
    
    hist_Edep_M4_T1_C_after->Write();
    hist_Edep_M4_T2_C_after->Write();
    hist_Edep_M4_T3_C_after->Write();
    hist_Edep_M4_T4_C_after->Write();
    
    hist_Edep_M5_T1_C_after->Write();
    hist_Edep_M5_T2_C_after->Write();
    hist_Edep_M5_T3_C_after->Write();
    hist_Edep_M5_T4_C_after->Write();
    
    hist_Edep_M6_T1_C_after->Write();
    hist_Edep_M6_T2_C_after->Write();
    hist_Edep_M6_T3_C_after->Write();
    hist_Edep_M6_T4_C_after->Write();
    
    hist_Edep_M7_T1_C_after->Write();
    hist_Edep_M7_T2_C_after->Write();
    hist_Edep_M7_T3_C_after->Write();
    hist_Edep_M7_T4_C_after->Write();
    
    hist_Edep_M8_T1_C_after->Write();
    hist_Edep_M8_T2_C_after->Write();
    hist_Edep_M8_T3_C_after->Write();
    hist_Edep_M8_T4_C_after->Write();
    
    hist_Edep_M9_T1_C_after->Write();
    hist_Edep_M9_T2_C_after->Write();
    hist_Edep_M9_T3_C_after->Write();
    hist_Edep_M9_T4_C_after->Write();
    
    hist_Edep_M12_C_after->Write();
    hist_Edep_M23_C_after->Write();
    hist_Edep_M34_C_after->Write();
    hist_Edep_M45_C_after->Write();
    hist_Edep_M56_C_after->Write();
    hist_Edep_M67_C_after->Write();
    hist_Edep_M78_C_after->Write();
    hist_Edep_M89_C_after->Write();
    hist_Edep_M91_C_after->Write();
    
    hist_Edep_M1_C_after->Write();
    hist_Edep_M2_C_after->Write();
    hist_Edep_M3_C_after->Write();
    hist_Edep_M4_C_after->Write();
    hist_Edep_M5_C_after->Write();
    hist_Edep_M6_C_after->Write();
    hist_Edep_M7_C_after->Write();
    hist_Edep_M8_C_after->Write();
    hist_Edep_M9_C_after->Write();
    
    hist_Edep_M1_T1_S_after->Write();
    hist_Edep_M1_T2_S_after->Write();
    hist_Edep_M1_T3_S_after->Write();
    hist_Edep_M1_T4_S_after->Write();
    
    hist_Edep_M2_T1_S_after->Write();
    hist_Edep_M2_T2_S_after->Write();
    hist_Edep_M2_T3_S_after->Write();
    hist_Edep_M2_T4_S_after->Write();
    
    hist_Edep_M3_T1_S_after->Write();
    hist_Edep_M3_T2_S_after->Write();
    hist_Edep_M3_T3_S_after->Write();
    hist_Edep_M3_T4_S_after->Write();
    
    hist_Edep_M4_T1_S_after->Write();
    hist_Edep_M4_T2_S_after->Write();
    hist_Edep_M4_T3_S_after->Write();
    hist_Edep_M4_T4_S_after->Write();
    
    hist_Edep_M5_T1_S_after->Write();
    hist_Edep_M5_T2_S_after->Write();
    hist_Edep_M5_T3_S_after->Write();
    hist_Edep_M5_T4_S_after->Write();
    
    hist_Edep_M6_T1_S_after->Write();
    hist_Edep_M6_T2_S_after->Write();
    hist_Edep_M6_T3_S_after->Write();
    hist_Edep_M6_T4_S_after->Write();
    
    hist_Edep_M7_T1_S_after->Write();
    hist_Edep_M7_T2_S_after->Write();
    hist_Edep_M7_T3_S_after->Write();
    hist_Edep_M7_T4_S_after->Write();
    
    hist_Edep_M8_T1_S_after->Write();
    hist_Edep_M8_T2_S_after->Write();
    hist_Edep_M8_T3_S_after->Write();
    hist_Edep_M8_T4_S_after->Write();
    
    hist_Edep_M9_T1_S_after->Write();
    hist_Edep_M9_T2_S_after->Write();
    hist_Edep_M9_T3_S_after->Write();
    hist_Edep_M9_T4_S_after->Write();
    
    hist_Edep_M12_S_after->Write();
    hist_Edep_M23_S_after->Write();
    hist_Edep_M34_S_after->Write();
    hist_Edep_M45_S_after->Write();
    hist_Edep_M56_S_after->Write();
    hist_Edep_M67_S_after->Write();
    hist_Edep_M78_S_after->Write();
    hist_Edep_M89_S_after->Write();
    hist_Edep_M91_S_after->Write();
    
    hist_Edep_M1_S_after->Write();
    hist_Edep_M2_S_after->Write();
    hist_Edep_M3_S_after->Write();
    hist_Edep_M4_S_after->Write();
    hist_Edep_M5_S_after->Write();
    hist_Edep_M6_S_after->Write();
    hist_Edep_M7_S_after->Write();
    hist_Edep_M8_S_after->Write();
    hist_Edep_M9_S_after->Write();
    
    hist_totalEdep_C_after   ->Write();
    hist_totalEdep_S_after   ->Write();
    hist_totalEdep_S_LC_compensated_after ->Write();
    hist_totalEdep_Comb_after->Write();
    hist_totalEdep_Comb_LC_compensated_after->Write();
    
    hist_totalEdepScaled_C_after   ->Write();
    hist_totalEdepScaled_S_after   ->Write();
    hist_totalEdepScaled_S_LC_compensated_after ->Write();
    hist_totalEdepScaled_Comb_after->Write();
    hist_totalEdepScaled_Comb_LC_compensated_after->Write();
    
    hist_CC1_after->Write();
    hist_CC2_after->Write();
    
    hist_PS_after->Write();
    hist_MC_after->Write();
    hist_TC_after->Write();
    
    hist_LC2_after->Write();
    hist_LC3_after->Write();
    hist_LC4_after->Write();
    hist_LC5_after->Write();
    hist_LC7_after->Write();
    hist_LC8_after->Write();
    hist_LC9_after->Write();
    hist_LC10_after->Write();
    hist_LC11_after->Write();
    hist_LC12_after->Write();
    hist_LC13_after->Write();
    hist_LC14_after->Write();
    hist_LC15_after->Write();
    hist_LC16_after->Write();
    hist_LC19_after->Write();
    hist_LC20_after->Write();
    
    hist_DWC1_pos_after  ->Write();
    hist_DWC2_pos_after  ->Write();
    hist_DWC_x_corr_after->Write();
    hist_DWC_y_corr_after->Write();
    
    outputRoot->Close();
    
    return 0;
}
