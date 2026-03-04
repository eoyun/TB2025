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
    
    // Scaled factor for each channel
    float scaleFactor_C = 0.9565; // Using M5T1 C ch with fitting result (99.2%, 59.52 GeV)
    float scaleFactor_S = 0.9843; // Using M5T1 S ch with fitting result (99.2%, 59.52 GeV)
    
    float calib_M1_T1_C = 0.001203555; float calib_M1_T1_S = 0.000608168;
    float calib_M1_T2_C = 0.001212408; float calib_M1_T2_S = 0.000599951;
    float calib_M1_T3_C = 0.001243669; float calib_M1_T3_S = 0.000632579;
    float calib_M1_T4_C = 0.00120427;  float calib_M1_T4_S = 0.00063748;
    
    float calib_M2_T1_C = 0.001114422; float calib_M2_T1_S = 0.000603563;
    float calib_M2_T2_C = 0.001210115; float calib_M2_T2_S = 0.000601509;
    float calib_M2_T3_C = 0.001134603; float calib_M2_T3_S = 0.000624896;
    float calib_M2_T4_C = 0.001228336; float calib_M2_T4_S = 0.000654066;
    
    float calib_M3_T1_C = 0.001167599; float calib_M3_T1_S = 0.000604899;
    float calib_M3_T2_C = 0.001298819; float calib_M3_T2_S = 0.000624518;
    float calib_M3_T3_C = 0.001253319; float calib_M3_T3_S = 0.000621517;
    float calib_M3_T4_C = 0.001245581; float calib_M3_T4_S = 0.000645067;
    
    float calib_M4_T1_C = 0.00119433;  float calib_M4_T1_S = 0.000631232;
    float calib_M4_T2_C = 0.001002412; float calib_M4_T2_S = 0.000598297;
    float calib_M4_T3_C = 0.001195593; float calib_M4_T3_S = 0.00061746;
    float calib_M4_T4_C = 0.001178153; float calib_M4_T4_S = 0.000638033;
    
    float calib_M5_T1_C = 0.001322633; float calib_M5_T1_S = 0.000628109;
    float calib_M5_T2_C = 0.00120786;  float calib_M5_T2_S = 0.000577394;
    float calib_M5_T3_C = 0.001544522; float calib_M5_T3_S = 0.000777161;
    float calib_M5_T4_C = 0.001155995; float calib_M5_T4_S = 0.000660589; 
    
    float calib_M6_T1_C = 0.001242049; float calib_M6_T1_S = 0.000642289;
    float calib_M6_T2_C = 0.001142602; float calib_M6_T2_S = 0.000637514;
    float calib_M6_T3_C = 0.001283866; float calib_M6_T3_S = 0.000651367; 
    float calib_M6_T4_C = 0.001343688; float calib_M6_T4_S = 0.000665772;
    
    float calib_M7_T1_C = 0.00124939;  float calib_M7_T1_S = 0.000631831;
    float calib_M7_T2_C = 0.001223514; float calib_M7_T2_S = 0.000611742;
    float calib_M7_T3_C = 0.001252899; float calib_M7_T3_S = 0.000592046;
    float calib_M7_T4_C = 0.001233478; float calib_M7_T4_S = 0.000641848;
    
    float calib_M8_T1_C = 0.001276618; float calib_M8_T1_S = 0.000625958;
    float calib_M8_T2_C = 0.001222038; float calib_M8_T2_S = 0.000599988;
    float calib_M8_T3_C = 0.001211442; float calib_M8_T3_S = 0.000612211;
    float calib_M8_T4_C = 0.001226694; float calib_M8_T4_S = 0.000637865;
    
    float calib_M9_T1_C = 0.00123999;  float calib_M9_T1_S = 0.000631297;
    float calib_M9_T2_C = 0.001236395; float calib_M9_T2_S = 0.000614992;
    float calib_M9_T3_C = 0.001227406; float calib_M9_T3_S = 0.000635913;
    float calib_M9_T4_C = 0.001174254; float calib_M9_T4_S = 0.000655751;

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
    
    fs::path dir("./DRC_ntuple");   
    if (!(fs::exists(dir))) fs::create_directory(dir);
        
    // initialize the utility class
    TButility util = TButility();
    util.LoadMapping("../mapping/mapping_TB2025_v1.root");
    
    TFile* f_DWC = TFile::Open((TString)("./DWC/DWC_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    TH2D* h_DWC1_pos   = (TH2D*) f_DWC->Get("dwc1_pos");
    TH2D* h_DWC2_pos   = (TH2D*) f_DWC->Get("dwc2_pos");
    std::vector<float> DWC1_offset = getDWCoffset(h_DWC1_pos); // DWC1_offset.at(0) == X, DWC1_offset.at(1) == Y
    std::vector<float> DWC2_offset = getDWCoffset(h_DWC2_pos);
    
    
    // prepare the histograms wa want to draw
    TH1F* hist_M1_T1_C_100 = new TH1F("M1_T1_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_C_100 = new TH1F("M1_T2_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_C_100 = new TH1F("M1_T3_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_C_100 = new TH1F("M1_T4_C_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_C_100 = new TH1F("M2_T1_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_C_100 = new TH1F("M2_T2_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_C_100 = new TH1F("M2_T3_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_C_100 = new TH1F("M2_T4_C_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_C_100 = new TH1F("M3_T1_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_C_100 = new TH1F("M3_T2_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_C_100 = new TH1F("M3_T3_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_C_100 = new TH1F("M3_T4_C_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_C_100 = new TH1F("M4_T1_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_C_100 = new TH1F("M4_T2_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_C_100 = new TH1F("M4_T3_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_C_100 = new TH1F("M4_T4_C_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_C_100 = new TH1F("M5_T1_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_C_100 = new TH1F("M5_T2_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_C_100 = new TH1F("M5_T3_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_C_100 = new TH1F("M5_T4_C_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_C_100 = new TH1F("M6_T1_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_C_100 = new TH1F("M6_T2_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_C_100 = new TH1F("M6_T3_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_C_100 = new TH1F("M6_T4_C_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_C_100 = new TH1F("M7_T1_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_C_100 = new TH1F("M7_T2_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_C_100 = new TH1F("M7_T3_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_C_100 = new TH1F("M7_T4_C_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_C_100 = new TH1F("M8_T1_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_C_100 = new TH1F("M8_T2_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_C_100 = new TH1F("M8_T3_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_C_100 = new TH1F("M8_T4_C_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_C_100 = new TH1F("M9_T1_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_C_100 = new TH1F("M9_T2_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_C_100 = new TH1F("M9_T3_C_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_C_100 = new TH1F("M9_T4_C_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_S_100 = new TH1F("M1_T1_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_S_100 = new TH1F("M1_T2_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_S_100 = new TH1F("M1_T3_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_S_100 = new TH1F("M1_T4_S_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_S_100 = new TH1F("M2_T1_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_S_100 = new TH1F("M2_T2_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_S_100 = new TH1F("M2_T3_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_S_100 = new TH1F("M2_T4_S_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_S_100 = new TH1F("M3_T1_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_S_100 = new TH1F("M3_T2_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_S_100 = new TH1F("M3_T3_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_S_100 = new TH1F("M3_T4_S_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_S_100 = new TH1F("M4_T1_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_S_100 = new TH1F("M4_T2_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_S_100 = new TH1F("M4_T3_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_S_100 = new TH1F("M4_T4_S_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_S_100 = new TH1F("M5_T1_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_S_100 = new TH1F("M5_T2_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_S_100 = new TH1F("M5_T3_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_S_100 = new TH1F("M5_T4_S_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_S_100 = new TH1F("M6_T1_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_S_100 = new TH1F("M6_T2_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_S_100 = new TH1F("M6_T3_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_S_100 = new TH1F("M6_T4_S_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_S_100 = new TH1F("M7_T1_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_S_100 = new TH1F("M7_T2_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_S_100 = new TH1F("M7_T3_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_S_100 = new TH1F("M7_T4_S_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_S_100 = new TH1F("M8_T1_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_S_100 = new TH1F("M8_T2_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_S_100 = new TH1F("M8_T3_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_S_100 = new TH1F("M8_T4_S_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_S_100 = new TH1F("M9_T1_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_S_100 = new TH1F("M9_T2_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_S_100 = new TH1F("M9_T3_S_100" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_S_100 = new TH1F("M9_T4_S_100" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_C_200 = new TH1F("M1_T1_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_C_200 = new TH1F("M1_T2_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_C_200 = new TH1F("M1_T3_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_C_200 = new TH1F("M1_T4_C_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_C_200 = new TH1F("M2_T1_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_C_200 = new TH1F("M2_T2_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_C_200 = new TH1F("M2_T3_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_C_200 = new TH1F("M2_T4_C_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_C_200 = new TH1F("M3_T1_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_C_200 = new TH1F("M3_T2_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_C_200 = new TH1F("M3_T3_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_C_200 = new TH1F("M3_T4_C_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_C_200 = new TH1F("M4_T1_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_C_200 = new TH1F("M4_T2_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_C_200 = new TH1F("M4_T3_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_C_200 = new TH1F("M4_T4_C_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_C_200 = new TH1F("M5_T1_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_C_200 = new TH1F("M5_T2_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_C_200 = new TH1F("M5_T3_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_C_200 = new TH1F("M5_T4_C_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_C_200 = new TH1F("M6_T1_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_C_200 = new TH1F("M6_T2_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_C_200 = new TH1F("M6_T3_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_C_200 = new TH1F("M6_T4_C_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_C_200 = new TH1F("M7_T1_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_C_200 = new TH1F("M7_T2_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_C_200 = new TH1F("M7_T3_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_C_200 = new TH1F("M7_T4_C_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_C_200 = new TH1F("M8_T1_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_C_200 = new TH1F("M8_T2_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_C_200 = new TH1F("M8_T3_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_C_200 = new TH1F("M8_T4_C_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_C_200 = new TH1F("M9_T1_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_C_200 = new TH1F("M9_T2_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_C_200 = new TH1F("M9_T3_C_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_C_200 = new TH1F("M9_T4_C_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_S_200 = new TH1F("M1_T1_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_S_200 = new TH1F("M1_T2_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_S_200 = new TH1F("M1_T3_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_S_200 = new TH1F("M1_T4_S_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_S_200 = new TH1F("M2_T1_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_S_200 = new TH1F("M2_T2_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_S_200 = new TH1F("M2_T3_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_S_200 = new TH1F("M2_T4_S_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_S_200 = new TH1F("M3_T1_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_S_200 = new TH1F("M3_T2_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_S_200 = new TH1F("M3_T3_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_S_200 = new TH1F("M3_T4_S_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_S_200 = new TH1F("M4_T1_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_S_200 = new TH1F("M4_T2_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_S_200 = new TH1F("M4_T3_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_S_200 = new TH1F("M4_T4_S_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_S_200 = new TH1F("M5_T1_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_S_200 = new TH1F("M5_T2_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_S_200 = new TH1F("M5_T3_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_S_200 = new TH1F("M5_T4_S_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_S_200 = new TH1F("M6_T1_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_S_200 = new TH1F("M6_T2_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_S_200 = new TH1F("M6_T3_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_S_200 = new TH1F("M6_T4_S_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_S_200 = new TH1F("M7_T1_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_S_200 = new TH1F("M7_T2_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_S_200 = new TH1F("M7_T3_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_S_200 = new TH1F("M7_T4_S_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_S_200 = new TH1F("M8_T1_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_S_200 = new TH1F("M8_T2_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_S_200 = new TH1F("M8_T3_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_S_200 = new TH1F("M8_T4_S_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_S_200 = new TH1F("M9_T1_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_S_200 = new TH1F("M9_T2_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_S_200 = new TH1F("M9_T3_S_200" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_S_200 = new TH1F("M9_T4_S_200" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_C_300 = new TH1F("M1_T1_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_C_300 = new TH1F("M1_T2_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_C_300 = new TH1F("M1_T3_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_C_300 = new TH1F("M1_T4_C_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_C_300 = new TH1F("M2_T1_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_C_300 = new TH1F("M2_T2_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_C_300 = new TH1F("M2_T3_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_C_300 = new TH1F("M2_T4_C_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_C_300 = new TH1F("M3_T1_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_C_300 = new TH1F("M3_T2_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_C_300 = new TH1F("M3_T3_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_C_300 = new TH1F("M3_T4_C_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_C_300 = new TH1F("M4_T1_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_C_300 = new TH1F("M4_T2_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_C_300 = new TH1F("M4_T3_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_C_300 = new TH1F("M4_T4_C_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_C_300 = new TH1F("M5_T1_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_C_300 = new TH1F("M5_T2_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_C_300 = new TH1F("M5_T3_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_C_300 = new TH1F("M5_T4_C_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_C_300 = new TH1F("M6_T1_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_C_300 = new TH1F("M6_T2_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_C_300 = new TH1F("M6_T3_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_C_300 = new TH1F("M6_T4_C_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_C_300 = new TH1F("M7_T1_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_C_300 = new TH1F("M7_T2_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_C_300 = new TH1F("M7_T3_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_C_300 = new TH1F("M7_T4_C_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_C_300 = new TH1F("M8_T1_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_C_300 = new TH1F("M8_T2_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_C_300 = new TH1F("M8_T3_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_C_300 = new TH1F("M8_T4_C_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_C_300 = new TH1F("M9_T1_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_C_300 = new TH1F("M9_T2_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_C_300 = new TH1F("M9_T3_C_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_C_300 = new TH1F("M9_T4_C_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_S_300 = new TH1F("M1_T1_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_S_300 = new TH1F("M1_T2_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_S_300 = new TH1F("M1_T3_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_S_300 = new TH1F("M1_T4_S_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_S_300 = new TH1F("M2_T1_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_S_300 = new TH1F("M2_T2_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_S_300 = new TH1F("M2_T3_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_S_300 = new TH1F("M2_T4_S_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_S_300 = new TH1F("M3_T1_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_S_300 = new TH1F("M3_T2_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_S_300 = new TH1F("M3_T3_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_S_300 = new TH1F("M3_T4_S_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_S_300 = new TH1F("M4_T1_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_S_300 = new TH1F("M4_T2_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_S_300 = new TH1F("M4_T3_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_S_300 = new TH1F("M4_T4_S_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_S_300 = new TH1F("M5_T1_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_S_300 = new TH1F("M5_T2_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_S_300 = new TH1F("M5_T3_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_S_300 = new TH1F("M5_T4_S_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_S_300 = new TH1F("M6_T1_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_S_300 = new TH1F("M6_T2_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_S_300 = new TH1F("M6_T3_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_S_300 = new TH1F("M6_T4_S_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_S_300 = new TH1F("M7_T1_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_S_300 = new TH1F("M7_T2_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_S_300 = new TH1F("M7_T3_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_S_300 = new TH1F("M7_T4_S_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_S_300 = new TH1F("M8_T1_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_S_300 = new TH1F("M8_T2_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_S_300 = new TH1F("M8_T3_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_S_300 = new TH1F("M8_T4_S_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_S_300 = new TH1F("M9_T1_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_S_300 = new TH1F("M9_T2_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_S_300 = new TH1F("M9_T3_S_300" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_S_300 = new TH1F("M9_T4_S_300" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_C_400 = new TH1F("M1_T1_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_C_400 = new TH1F("M1_T2_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_C_400 = new TH1F("M1_T3_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_C_400 = new TH1F("M1_T4_C_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_C_400 = new TH1F("M2_T1_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_C_400 = new TH1F("M2_T2_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_C_400 = new TH1F("M2_T3_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_C_400 = new TH1F("M2_T4_C_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_C_400 = new TH1F("M3_T1_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_C_400 = new TH1F("M3_T2_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_C_400 = new TH1F("M3_T3_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_C_400 = new TH1F("M3_T4_C_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_C_400 = new TH1F("M4_T1_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_C_400 = new TH1F("M4_T2_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_C_400 = new TH1F("M4_T3_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_C_400 = new TH1F("M4_T4_C_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_C_400 = new TH1F("M5_T1_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_C_400 = new TH1F("M5_T2_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_C_400 = new TH1F("M5_T3_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_C_400 = new TH1F("M5_T4_C_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_C_400 = new TH1F("M6_T1_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_C_400 = new TH1F("M6_T2_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_C_400 = new TH1F("M6_T3_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_C_400 = new TH1F("M6_T4_C_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_C_400 = new TH1F("M7_T1_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_C_400 = new TH1F("M7_T2_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_C_400 = new TH1F("M7_T3_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_C_400 = new TH1F("M7_T4_C_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_C_400 = new TH1F("M8_T1_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_C_400 = new TH1F("M8_T2_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_C_400 = new TH1F("M8_T3_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_C_400 = new TH1F("M8_T4_C_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_C_400 = new TH1F("M9_T1_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_C_400 = new TH1F("M9_T2_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_C_400 = new TH1F("M9_T3_C_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_C_400 = new TH1F("M9_T4_C_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_S_400 = new TH1F("M1_T1_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_S_400 = new TH1F("M1_T2_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_S_400 = new TH1F("M1_T3_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_S_400 = new TH1F("M1_T4_S_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_S_400 = new TH1F("M2_T1_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_S_400 = new TH1F("M2_T2_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_S_400 = new TH1F("M2_T3_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_S_400 = new TH1F("M2_T4_S_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_S_400 = new TH1F("M3_T1_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_S_400 = new TH1F("M3_T2_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_S_400 = new TH1F("M3_T3_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_S_400 = new TH1F("M3_T4_S_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_S_400 = new TH1F("M4_T1_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_S_400 = new TH1F("M4_T2_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_S_400 = new TH1F("M4_T3_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_S_400 = new TH1F("M4_T4_S_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_S_400 = new TH1F("M5_T1_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_S_400 = new TH1F("M5_T2_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_S_400 = new TH1F("M5_T3_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_S_400 = new TH1F("M5_T4_S_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_S_400 = new TH1F("M6_T1_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_S_400 = new TH1F("M6_T2_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_S_400 = new TH1F("M6_T3_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_S_400 = new TH1F("M6_T4_S_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_S_400 = new TH1F("M7_T1_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_S_400 = new TH1F("M7_T2_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_S_400 = new TH1F("M7_T3_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_S_400 = new TH1F("M7_T4_S_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_S_400 = new TH1F("M8_T1_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_S_400 = new TH1F("M8_T2_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_S_400 = new TH1F("M8_T3_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_S_400 = new TH1F("M8_T4_S_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_S_400 = new TH1F("M9_T1_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_S_400 = new TH1F("M9_T2_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_S_400 = new TH1F("M9_T3_S_400" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_S_400 = new TH1F("M9_T4_S_400" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_C_500 = new TH1F("M1_T1_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_C_500 = new TH1F("M1_T2_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_C_500 = new TH1F("M1_T3_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_C_500 = new TH1F("M1_T4_C_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_C_500 = new TH1F("M2_T1_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_C_500 = new TH1F("M2_T2_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_C_500 = new TH1F("M2_T3_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_C_500 = new TH1F("M2_T4_C_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_C_500 = new TH1F("M3_T1_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_C_500 = new TH1F("M3_T2_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_C_500 = new TH1F("M3_T3_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_C_500 = new TH1F("M3_T4_C_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_C_500 = new TH1F("M4_T1_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_C_500 = new TH1F("M4_T2_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_C_500 = new TH1F("M4_T3_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_C_500 = new TH1F("M4_T4_C_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_C_500 = new TH1F("M5_T1_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_C_500 = new TH1F("M5_T2_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_C_500 = new TH1F("M5_T3_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_C_500 = new TH1F("M5_T4_C_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_C_500 = new TH1F("M6_T1_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_C_500 = new TH1F("M6_T2_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_C_500 = new TH1F("M6_T3_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_C_500 = new TH1F("M6_T4_C_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_C_500 = new TH1F("M7_T1_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_C_500 = new TH1F("M7_T2_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_C_500 = new TH1F("M7_T3_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_C_500 = new TH1F("M7_T4_C_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_C_500 = new TH1F("M8_T1_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_C_500 = new TH1F("M8_T2_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_C_500 = new TH1F("M8_T3_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_C_500 = new TH1F("M8_T4_C_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_C_500 = new TH1F("M9_T1_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_C_500 = new TH1F("M9_T2_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_C_500 = new TH1F("M9_T3_C_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_C_500 = new TH1F("M9_T4_C_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_S_500 = new TH1F("M1_T1_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_S_500 = new TH1F("M1_T2_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_S_500 = new TH1F("M1_T3_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_S_500 = new TH1F("M1_T4_S_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_S_500 = new TH1F("M2_T1_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_S_500 = new TH1F("M2_T2_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_S_500 = new TH1F("M2_T3_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_S_500 = new TH1F("M2_T4_S_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_S_500 = new TH1F("M3_T1_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_S_500 = new TH1F("M3_T2_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_S_500 = new TH1F("M3_T3_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_S_500 = new TH1F("M3_T4_S_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_S_500 = new TH1F("M4_T1_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_S_500 = new TH1F("M4_T2_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_S_500 = new TH1F("M4_T3_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_S_500 = new TH1F("M4_T4_S_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_S_500 = new TH1F("M5_T1_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_S_500 = new TH1F("M5_T2_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_S_500 = new TH1F("M5_T3_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_S_500 = new TH1F("M5_T4_S_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_S_500 = new TH1F("M6_T1_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_S_500 = new TH1F("M6_T2_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_S_500 = new TH1F("M6_T3_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_S_500 = new TH1F("M6_T4_S_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_S_500 = new TH1F("M7_T1_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_S_500 = new TH1F("M7_T2_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_S_500 = new TH1F("M7_T3_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_S_500 = new TH1F("M7_T4_S_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_S_500 = new TH1F("M8_T1_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_S_500 = new TH1F("M8_T2_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_S_500 = new TH1F("M8_T3_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_S_500 = new TH1F("M8_T4_S_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_S_500 = new TH1F("M9_T1_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_S_500 = new TH1F("M9_T2_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_S_500 = new TH1F("M9_T3_S_500" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_S_500 = new TH1F("M9_T4_S_500" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_C_600 = new TH1F("M1_T1_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_C_600 = new TH1F("M1_T2_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_C_600 = new TH1F("M1_T3_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_C_600 = new TH1F("M1_T4_C_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_C_600 = new TH1F("M2_T1_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_C_600 = new TH1F("M2_T2_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_C_600 = new TH1F("M2_T3_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_C_600 = new TH1F("M2_T4_C_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_C_600 = new TH1F("M3_T1_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_C_600 = new TH1F("M3_T2_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_C_600 = new TH1F("M3_T3_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_C_600 = new TH1F("M3_T4_C_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_C_600 = new TH1F("M4_T1_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_C_600 = new TH1F("M4_T2_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_C_600 = new TH1F("M4_T3_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_C_600 = new TH1F("M4_T4_C_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_C_600 = new TH1F("M5_T1_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_C_600 = new TH1F("M5_T2_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_C_600 = new TH1F("M5_T3_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_C_600 = new TH1F("M5_T4_C_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_C_600 = new TH1F("M6_T1_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_C_600 = new TH1F("M6_T2_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_C_600 = new TH1F("M6_T3_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_C_600 = new TH1F("M6_T4_C_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_C_600 = new TH1F("M7_T1_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_C_600 = new TH1F("M7_T2_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_C_600 = new TH1F("M7_T3_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_C_600 = new TH1F("M7_T4_C_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_C_600 = new TH1F("M8_T1_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_C_600 = new TH1F("M8_T2_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_C_600 = new TH1F("M8_T3_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_C_600 = new TH1F("M8_T4_C_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_C_600 = new TH1F("M9_T1_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_C_600 = new TH1F("M9_T2_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_C_600 = new TH1F("M9_T3_C_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_C_600 = new TH1F("M9_T4_C_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_S_600 = new TH1F("M1_T1_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_S_600 = new TH1F("M1_T2_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_S_600 = new TH1F("M1_T3_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_S_600 = new TH1F("M1_T4_S_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_S_600 = new TH1F("M2_T1_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_S_600 = new TH1F("M2_T2_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_S_600 = new TH1F("M2_T3_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_S_600 = new TH1F("M2_T4_S_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_S_600 = new TH1F("M3_T1_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_S_600 = new TH1F("M3_T2_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_S_600 = new TH1F("M3_T3_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_S_600 = new TH1F("M3_T4_S_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_S_600 = new TH1F("M4_T1_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_S_600 = new TH1F("M4_T2_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_S_600 = new TH1F("M4_T3_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_S_600 = new TH1F("M4_T4_S_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_S_600 = new TH1F("M5_T1_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_S_600 = new TH1F("M5_T2_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_S_600 = new TH1F("M5_T3_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_S_600 = new TH1F("M5_T4_S_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_S_600 = new TH1F("M6_T1_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_S_600 = new TH1F("M6_T2_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_S_600 = new TH1F("M6_T3_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_S_600 = new TH1F("M6_T4_S_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_S_600 = new TH1F("M7_T1_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_S_600 = new TH1F("M7_T2_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_S_600 = new TH1F("M7_T3_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_S_600 = new TH1F("M7_T4_S_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_S_600 = new TH1F("M8_T1_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_S_600 = new TH1F("M8_T2_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_S_600 = new TH1F("M8_T3_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_S_600 = new TH1F("M8_T4_S_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_S_600 = new TH1F("M9_T1_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_S_600 = new TH1F("M9_T2_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_S_600 = new TH1F("M9_T3_S_600" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_S_600 = new TH1F("M9_T4_S_600" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_C_700 = new TH1F("M1_T1_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_C_700 = new TH1F("M1_T2_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_C_700 = new TH1F("M1_T3_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_C_700 = new TH1F("M1_T4_C_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_C_700 = new TH1F("M2_T1_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_C_700 = new TH1F("M2_T2_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_C_700 = new TH1F("M2_T3_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_C_700 = new TH1F("M2_T4_C_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_C_700 = new TH1F("M3_T1_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_C_700 = new TH1F("M3_T2_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_C_700 = new TH1F("M3_T3_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_C_700 = new TH1F("M3_T4_C_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_C_700 = new TH1F("M4_T1_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_C_700 = new TH1F("M4_T2_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_C_700 = new TH1F("M4_T3_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_C_700 = new TH1F("M4_T4_C_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_C_700 = new TH1F("M5_T1_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_C_700 = new TH1F("M5_T2_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_C_700 = new TH1F("M5_T3_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_C_700 = new TH1F("M5_T4_C_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_C_700 = new TH1F("M6_T1_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_C_700 = new TH1F("M6_T2_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_C_700 = new TH1F("M6_T3_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_C_700 = new TH1F("M6_T4_C_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_C_700 = new TH1F("M7_T1_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_C_700 = new TH1F("M7_T2_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_C_700 = new TH1F("M7_T3_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_C_700 = new TH1F("M7_T4_C_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_C_700 = new TH1F("M8_T1_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_C_700 = new TH1F("M8_T2_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_C_700 = new TH1F("M8_T3_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_C_700 = new TH1F("M8_T4_C_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_C_700 = new TH1F("M9_T1_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_C_700 = new TH1F("M9_T2_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_C_700 = new TH1F("M9_T3_C_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_C_700 = new TH1F("M9_T4_C_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_S_700 = new TH1F("M1_T1_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_S_700 = new TH1F("M1_T2_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_S_700 = new TH1F("M1_T3_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_S_700 = new TH1F("M1_T4_S_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_S_700 = new TH1F("M2_T1_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_S_700 = new TH1F("M2_T2_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_S_700 = new TH1F("M2_T3_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_S_700 = new TH1F("M2_T4_S_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_S_700 = new TH1F("M3_T1_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_S_700 = new TH1F("M3_T2_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_S_700 = new TH1F("M3_T3_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_S_700 = new TH1F("M3_T4_S_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_S_700 = new TH1F("M4_T1_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_S_700 = new TH1F("M4_T2_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_S_700 = new TH1F("M4_T3_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_S_700 = new TH1F("M4_T4_S_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_S_700 = new TH1F("M5_T1_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_S_700 = new TH1F("M5_T2_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_S_700 = new TH1F("M5_T3_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_S_700 = new TH1F("M5_T4_S_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_S_700 = new TH1F("M6_T1_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_S_700 = new TH1F("M6_T2_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_S_700 = new TH1F("M6_T3_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_S_700 = new TH1F("M6_T4_S_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_S_700 = new TH1F("M7_T1_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_S_700 = new TH1F("M7_T2_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_S_700 = new TH1F("M7_T3_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_S_700 = new TH1F("M7_T4_S_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_S_700 = new TH1F("M8_T1_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_S_700 = new TH1F("M8_T2_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_S_700 = new TH1F("M8_T3_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_S_700 = new TH1F("M8_T4_S_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_S_700 = new TH1F("M9_T1_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_S_700 = new TH1F("M9_T2_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_S_700 = new TH1F("M9_T3_S_700" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_S_700 = new TH1F("M9_T4_S_700" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_C_800 = new TH1F("M1_T1_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_C_800 = new TH1F("M1_T2_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_C_800 = new TH1F("M1_T3_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_C_800 = new TH1F("M1_T4_C_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_C_800 = new TH1F("M2_T1_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_C_800 = new TH1F("M2_T2_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_C_800 = new TH1F("M2_T3_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_C_800 = new TH1F("M2_T4_C_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_C_800 = new TH1F("M3_T1_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_C_800 = new TH1F("M3_T2_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_C_800 = new TH1F("M3_T3_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_C_800 = new TH1F("M3_T4_C_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_C_800 = new TH1F("M4_T1_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_C_800 = new TH1F("M4_T2_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_C_800 = new TH1F("M4_T3_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_C_800 = new TH1F("M4_T4_C_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_C_800 = new TH1F("M5_T1_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_C_800 = new TH1F("M5_T2_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_C_800 = new TH1F("M5_T3_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_C_800 = new TH1F("M5_T4_C_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_C_800 = new TH1F("M6_T1_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_C_800 = new TH1F("M6_T2_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_C_800 = new TH1F("M6_T3_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_C_800 = new TH1F("M6_T4_C_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_C_800 = new TH1F("M7_T1_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_C_800 = new TH1F("M7_T2_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_C_800 = new TH1F("M7_T3_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_C_800 = new TH1F("M7_T4_C_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_C_800 = new TH1F("M8_T1_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_C_800 = new TH1F("M8_T2_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_C_800 = new TH1F("M8_T3_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_C_800 = new TH1F("M8_T4_C_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_C_800 = new TH1F("M9_T1_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_C_800 = new TH1F("M9_T2_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_C_800 = new TH1F("M9_T3_C_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_C_800 = new TH1F("M9_T4_C_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_S_800 = new TH1F("M1_T1_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_S_800 = new TH1F("M1_T2_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_S_800 = new TH1F("M1_T3_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_S_800 = new TH1F("M1_T4_S_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_S_800 = new TH1F("M2_T1_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_S_800 = new TH1F("M2_T2_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_S_800 = new TH1F("M2_T3_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_S_800 = new TH1F("M2_T4_S_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_S_800 = new TH1F("M3_T1_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_S_800 = new TH1F("M3_T2_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_S_800 = new TH1F("M3_T3_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_S_800 = new TH1F("M3_T4_S_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_S_800 = new TH1F("M4_T1_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_S_800 = new TH1F("M4_T2_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_S_800 = new TH1F("M4_T3_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_S_800 = new TH1F("M4_T4_S_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_S_800 = new TH1F("M5_T1_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_S_800 = new TH1F("M5_T2_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_S_800 = new TH1F("M5_T3_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_S_800 = new TH1F("M5_T4_S_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_S_800 = new TH1F("M6_T1_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_S_800 = new TH1F("M6_T2_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_S_800 = new TH1F("M6_T3_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_S_800 = new TH1F("M6_T4_S_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_S_800 = new TH1F("M7_T1_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_S_800 = new TH1F("M7_T2_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_S_800 = new TH1F("M7_T3_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_S_800 = new TH1F("M7_T4_S_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_S_800 = new TH1F("M8_T1_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_S_800 = new TH1F("M8_T2_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_S_800 = new TH1F("M8_T3_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_S_800 = new TH1F("M8_T4_S_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_S_800 = new TH1F("M9_T1_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_S_800 = new TH1F("M9_T2_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_S_800 = new TH1F("M9_T3_S_800" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_S_800 = new TH1F("M9_T4_S_800" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_C_900 = new TH1F("M1_T1_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_C_900 = new TH1F("M1_T2_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_C_900 = new TH1F("M1_T3_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_C_900 = new TH1F("M1_T4_C_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_C_900 = new TH1F("M2_T1_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_C_900 = new TH1F("M2_T2_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_C_900 = new TH1F("M2_T3_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_C_900 = new TH1F("M2_T4_C_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_C_900 = new TH1F("M3_T1_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_C_900 = new TH1F("M3_T2_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_C_900 = new TH1F("M3_T3_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_C_900 = new TH1F("M3_T4_C_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_C_900 = new TH1F("M4_T1_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_C_900 = new TH1F("M4_T2_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_C_900 = new TH1F("M4_T3_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_C_900 = new TH1F("M4_T4_C_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_C_900 = new TH1F("M5_T1_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_C_900 = new TH1F("M5_T2_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_C_900 = new TH1F("M5_T3_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_C_900 = new TH1F("M5_T4_C_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_C_900 = new TH1F("M6_T1_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_C_900 = new TH1F("M6_T2_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_C_900 = new TH1F("M6_T3_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_C_900 = new TH1F("M6_T4_C_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_C_900 = new TH1F("M7_T1_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_C_900 = new TH1F("M7_T2_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_C_900 = new TH1F("M7_T3_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_C_900 = new TH1F("M7_T4_C_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_C_900 = new TH1F("M8_T1_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_C_900 = new TH1F("M8_T2_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_C_900 = new TH1F("M8_T3_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_C_900 = new TH1F("M8_T4_C_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_C_900 = new TH1F("M9_T1_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_C_900 = new TH1F("M9_T2_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_C_900 = new TH1F("M9_T3_C_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_C_900 = new TH1F("M9_T4_C_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_S_900 = new TH1F("M1_T1_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_S_900 = new TH1F("M1_T2_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_S_900 = new TH1F("M1_T3_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_S_900 = new TH1F("M1_T4_S_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_S_900 = new TH1F("M2_T1_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_S_900 = new TH1F("M2_T2_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_S_900 = new TH1F("M2_T3_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_S_900 = new TH1F("M2_T4_S_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_S_900 = new TH1F("M3_T1_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_S_900 = new TH1F("M3_T2_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_S_900 = new TH1F("M3_T3_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_S_900 = new TH1F("M3_T4_S_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_S_900 = new TH1F("M4_T1_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_S_900 = new TH1F("M4_T2_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_S_900 = new TH1F("M4_T3_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_S_900 = new TH1F("M4_T4_S_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_S_900 = new TH1F("M5_T1_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_S_900 = new TH1F("M5_T2_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_S_900 = new TH1F("M5_T3_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_S_900 = new TH1F("M5_T4_S_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_S_900 = new TH1F("M6_T1_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_S_900 = new TH1F("M6_T2_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_S_900 = new TH1F("M6_T3_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_S_900 = new TH1F("M6_T4_S_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_S_900 = new TH1F("M7_T1_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_S_900 = new TH1F("M7_T2_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_S_900 = new TH1F("M7_T3_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_S_900 = new TH1F("M7_T4_S_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_S_900 = new TH1F("M8_T1_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_S_900 = new TH1F("M8_T2_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_S_900 = new TH1F("M8_T3_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_S_900 = new TH1F("M8_T4_S_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_S_900 = new TH1F("M9_T1_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_S_900 = new TH1F("M9_T2_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_S_900 = new TH1F("M9_T3_S_900" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_S_900 = new TH1F("M9_T4_S_900" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_C_1000 = new TH1F("M1_T1_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_C_1000 = new TH1F("M1_T2_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_C_1000 = new TH1F("M1_T3_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_C_1000 = new TH1F("M1_T4_C_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_C_1000 = new TH1F("M2_T1_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_C_1000 = new TH1F("M2_T2_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_C_1000 = new TH1F("M2_T3_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_C_1000 = new TH1F("M2_T4_C_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_C_1000 = new TH1F("M3_T1_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_C_1000 = new TH1F("M3_T2_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_C_1000 = new TH1F("M3_T3_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_C_1000 = new TH1F("M3_T4_C_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_C_1000 = new TH1F("M4_T1_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_C_1000 = new TH1F("M4_T2_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_C_1000 = new TH1F("M4_T3_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_C_1000 = new TH1F("M4_T4_C_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_C_1000 = new TH1F("M5_T1_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_C_1000 = new TH1F("M5_T2_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_C_1000 = new TH1F("M5_T3_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_C_1000 = new TH1F("M5_T4_C_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_C_1000 = new TH1F("M6_T1_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_C_1000 = new TH1F("M6_T2_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_C_1000 = new TH1F("M6_T3_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_C_1000 = new TH1F("M6_T4_C_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_C_1000 = new TH1F("M7_T1_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_C_1000 = new TH1F("M7_T2_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_C_1000 = new TH1F("M7_T3_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_C_1000 = new TH1F("M7_T4_C_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_C_1000 = new TH1F("M8_T1_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_C_1000 = new TH1F("M8_T2_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_C_1000 = new TH1F("M8_T3_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_C_1000 = new TH1F("M8_T4_C_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_C_1000 = new TH1F("M9_T1_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_C_1000 = new TH1F("M9_T2_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_C_1000 = new TH1F("M9_T3_C_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_C_1000 = new TH1F("M9_T4_C_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_S_1000 = new TH1F("M1_T1_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_S_1000 = new TH1F("M1_T2_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_S_1000 = new TH1F("M1_T3_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_S_1000 = new TH1F("M1_T4_S_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_S_1000 = new TH1F("M2_T1_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_S_1000 = new TH1F("M2_T2_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_S_1000 = new TH1F("M2_T3_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_S_1000 = new TH1F("M2_T4_S_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_S_1000 = new TH1F("M3_T1_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_S_1000 = new TH1F("M3_T2_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_S_1000 = new TH1F("M3_T3_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_S_1000 = new TH1F("M3_T4_S_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_S_1000 = new TH1F("M4_T1_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_S_1000 = new TH1F("M4_T2_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_S_1000 = new TH1F("M4_T3_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_S_1000 = new TH1F("M4_T4_S_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_S_1000 = new TH1F("M5_T1_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_S_1000 = new TH1F("M5_T2_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_S_1000 = new TH1F("M5_T3_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_S_1000 = new TH1F("M5_T4_S_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_S_1000 = new TH1F("M6_T1_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_S_1000 = new TH1F("M6_T2_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_S_1000 = new TH1F("M6_T3_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_S_1000 = new TH1F("M6_T4_S_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_S_1000 = new TH1F("M7_T1_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_S_1000 = new TH1F("M7_T2_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_S_1000 = new TH1F("M7_T3_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_S_1000 = new TH1F("M7_T4_S_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_S_1000 = new TH1F("M8_T1_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_S_1000 = new TH1F("M8_T2_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_S_1000 = new TH1F("M8_T3_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_S_1000 = new TH1F("M8_T4_S_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_S_1000 = new TH1F("M9_T1_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_S_1000 = new TH1F("M9_T2_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_S_1000 = new TH1F("M9_T3_S_1000" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_S_1000 = new TH1F("M9_T4_S_1000" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_C = new TH1F("M1_T1_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_C = new TH1F("M1_T2_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_C = new TH1F("M1_T3_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_C = new TH1F("M1_T4_C" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_C = new TH1F("M2_T1_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_C = new TH1F("M2_T2_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_C = new TH1F("M2_T3_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_C = new TH1F("M2_T4_C" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_C = new TH1F("M3_T1_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_C = new TH1F("M3_T2_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_C = new TH1F("M3_T3_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_C = new TH1F("M3_T4_C" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_C = new TH1F("M4_T1_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_C = new TH1F("M4_T2_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_C = new TH1F("M4_T3_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_C = new TH1F("M4_T4_C" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_C = new TH1F("M5_T1_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_C = new TH1F("M5_T2_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_C = new TH1F("M5_T3_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_C = new TH1F("M5_T4_C" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_C = new TH1F("M6_T1_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_C = new TH1F("M6_T2_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_C = new TH1F("M6_T3_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_C = new TH1F("M6_T4_C" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_C = new TH1F("M7_T1_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_C = new TH1F("M7_T2_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_C = new TH1F("M7_T3_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_C = new TH1F("M7_T4_C" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_C = new TH1F("M8_T1_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_C = new TH1F("M8_T2_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_C = new TH1F("M8_T3_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_C = new TH1F("M8_T4_C" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_C = new TH1F("M9_T1_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_C = new TH1F("M9_T2_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_C = new TH1F("M9_T3_C" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_C = new TH1F("M9_T4_C" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M1_T1_S = new TH1F("M1_T1_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T2_S = new TH1F("M1_T2_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T3_S = new TH1F("M1_T3_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M1_T4_S = new TH1F("M1_T4_S" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M2_T1_S = new TH1F("M2_T1_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T2_S = new TH1F("M2_T2_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T3_S = new TH1F("M2_T3_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M2_T4_S = new TH1F("M2_T4_S" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M3_T1_S = new TH1F("M3_T1_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T2_S = new TH1F("M3_T2_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T3_S = new TH1F("M3_T3_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M3_T4_S = new TH1F("M3_T4_S" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M4_T1_S = new TH1F("M4_T1_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T2_S = new TH1F("M4_T2_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T3_S = new TH1F("M4_T3_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M4_T4_S = new TH1F("M4_T4_S" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M5_T1_S = new TH1F("M5_T1_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T2_S = new TH1F("M5_T2_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T3_S = new TH1F("M5_T3_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M5_T4_S = new TH1F("M5_T4_S" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M6_T1_S = new TH1F("M6_T1_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T2_S = new TH1F("M6_T2_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T3_S = new TH1F("M6_T3_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M6_T4_S = new TH1F("M6_T4_S" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M7_T1_S = new TH1F("M7_T1_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T2_S = new TH1F("M7_T2_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T3_S = new TH1F("M7_T3_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M7_T4_S = new TH1F("M7_T4_S" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M8_T1_S = new TH1F("M8_T1_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T2_S = new TH1F("M8_T2_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T3_S = new TH1F("M8_T3_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M8_T4_S = new TH1F("M8_T4_S" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_M9_T1_S = new TH1F("M9_T1_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T2_S = new TH1F("M9_T2_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T3_S = new TH1F("M9_T3_S" , ";intADC;nEvents", 100, -50, 50);
    TH1F* hist_M9_T4_S = new TH1F("M9_T4_S" , ";intADC;nEvents", 100, -50, 50);
    
    TH1F* hist_Edep_M1_T1_C = new TH1F("Edep_M1_T1_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T2_C = new TH1F("Edep_M1_T2_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T3_C = new TH1F("Edep_M1_T3_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T4_C = new TH1F("Edep_M1_T4_C" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M2_T1_C = new TH1F("Edep_M2_T1_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T2_C = new TH1F("Edep_M2_T2_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T3_C = new TH1F("Edep_M2_T3_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T4_C = new TH1F("Edep_M2_T4_C" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M3_T1_C = new TH1F("Edep_M3_T1_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T2_C = new TH1F("Edep_M3_T2_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T3_C = new TH1F("Edep_M3_T3_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T4_C = new TH1F("Edep_M3_T4_C" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M4_T1_C = new TH1F("Edep_M4_T1_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T2_C = new TH1F("Edep_M4_T2_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T3_C = new TH1F("Edep_M4_T3_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T4_C = new TH1F("Edep_M4_T4_C" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M5_T1_C = new TH1F("Edep_M5_T1_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T2_C = new TH1F("Edep_M5_T2_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T3_C = new TH1F("Edep_M5_T3_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T4_C = new TH1F("Edep_M5_T4_C" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M6_T1_C = new TH1F("Edep_M6_T1_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T2_C = new TH1F("Edep_M6_T2_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T3_C = new TH1F("Edep_M6_T3_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T4_C = new TH1F("Edep_M6_T4_C" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M7_T1_C = new TH1F("Edep_M7_T1_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T2_C = new TH1F("Edep_M7_T2_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T3_C = new TH1F("Edep_M7_T3_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T4_C = new TH1F("Edep_M7_T4_C" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M8_T1_C = new TH1F("Edep_M8_T1_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T2_C = new TH1F("Edep_M8_T2_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T3_C = new TH1F("Edep_M8_T3_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T4_C = new TH1F("Edep_M8_T4_C" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M9_T1_C = new TH1F("Edep_M9_T1_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T2_C = new TH1F("Edep_M9_T2_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T3_C = new TH1F("Edep_M9_T3_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T4_C = new TH1F("Edep_M9_T4_C" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M1_T1_S = new TH1F("Edep_M1_T1_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T2_S = new TH1F("Edep_M1_T2_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T3_S = new TH1F("Edep_M1_T3_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T4_S = new TH1F("Edep_M1_T4_S" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M2_T1_S = new TH1F("Edep_M2_T1_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T2_S = new TH1F("Edep_M2_T2_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T3_S = new TH1F("Edep_M2_T3_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T4_S = new TH1F("Edep_M2_T4_S" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M3_T1_S = new TH1F("Edep_M3_T1_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T2_S = new TH1F("Edep_M3_T2_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T3_S = new TH1F("Edep_M3_T3_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T4_S = new TH1F("Edep_M3_T4_S" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M4_T1_S = new TH1F("Edep_M4_T1_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T2_S = new TH1F("Edep_M4_T2_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T3_S = new TH1F("Edep_M4_T3_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T4_S = new TH1F("Edep_M4_T4_S" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M5_T1_S = new TH1F("Edep_M5_T1_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T2_S = new TH1F("Edep_M5_T2_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T3_S = new TH1F("Edep_M5_T3_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T4_S = new TH1F("Edep_M5_T4_S" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M6_T1_S = new TH1F("Edep_M6_T1_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T2_S = new TH1F("Edep_M6_T2_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T3_S = new TH1F("Edep_M6_T3_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T4_S = new TH1F("Edep_M6_T4_S" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M7_T1_S = new TH1F("Edep_M7_T1_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T2_S = new TH1F("Edep_M7_T2_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T3_S = new TH1F("Edep_M7_T3_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T4_S = new TH1F("Edep_M7_T4_S" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M8_T1_S = new TH1F("Edep_M8_T1_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T2_S = new TH1F("Edep_M8_T2_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T3_S = new TH1F("Edep_M8_T3_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T4_S = new TH1F("Edep_M8_T4_S" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M9_T1_S = new TH1F("Edep_M9_T1_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T2_S = new TH1F("Edep_M9_T2_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T3_S = new TH1F("Edep_M9_T3_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T4_S = new TH1F("Edep_M9_T4_S" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M1_C = new TH1F("Edep_M1_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_C = new TH1F("Edep_M2_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_C = new TH1F("Edep_M3_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_C = new TH1F("Edep_M4_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_C = new TH1F("Edep_M5_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_C = new TH1F("Edep_M6_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_C = new TH1F("Edep_M7_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_C = new TH1F("Edep_M8_C" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_C = new TH1F("Edep_M9_C" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M1_S = new TH1F("Edep_M1_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_S = new TH1F("Edep_M2_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_S = new TH1F("Edep_M3_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_S = new TH1F("Edep_M4_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_S = new TH1F("Edep_M5_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_S = new TH1F("Edep_M6_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_S = new TH1F("Edep_M7_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_S = new TH1F("Edep_M8_S" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_S = new TH1F("Edep_M9_S" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_CC1 = new TH1F("CC1", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_CC2 = new TH1F("CC2", ";peakADC;Events", 1024, 0, 4096);
    
    TH1F* hist_PS = new TH1F("PS" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_MC = new TH1F("MC" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_TC = new TH1F("TC" , ";peakADC;nEvents", 1024, 0, 4096);
    
    TH1F* hist_LC2  = new TH1F("LC2"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC3  = new TH1F("LC3"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC4  = new TH1F("LC4"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC5  = new TH1F("LC5"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC7  = new TH1F("LC7"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC8  = new TH1F("LC8"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC9  = new TH1F("LC9"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC10 = new TH1F("LC10" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC11 = new TH1F("LC11" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC12 = new TH1F("LC12" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC13 = new TH1F("LC13" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC14 = new TH1F("LC14" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC15 = new TH1F("LC15" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC16 = new TH1F("LC16" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC19 = new TH1F("LC19" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC20 = new TH1F("LC20" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_totalEdep_C          = new TH1F("totalEdep_C",    ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdep_S          = new TH1F("totalEdep_S",    ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdep_S_LC_compensated = new TH1F("totalEdep_S_LC_compensated", ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdep_Comb       = new TH1F("totalEdep_Comb", ";GeV;Event", 110, -40, 400);
    TH1F* hist_totalEdep_Comb_LC_compensated = new TH1F("totalEdep_Comb_LC_compensated", ";GeV;Event", 110, -40, 400);
    
    TH1F* hist_totalEdepScaled_C    = new TH1F("totalEdepScaled_C",    ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdepScaled_S    = new TH1F("totalEdepScaled_S",    ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdepScaled_S_LC_compensated = new TH1F("totalEdepScaled_S_LC_compensated", ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdepScaled_Comb = new TH1F("totalEdepScaled_Comb", ";GeV;Event", 110, -40, 400);
    TH1F* hist_totalEdepScaled_Comb_LC_compensated = new TH1F("totalEdepScaled_Comb_LC_compensated", ";GeV;Event", 110, -40, 400);
    
    TH2D* hist_DWC1_pos_corrected   = new TH2D("DWC1_pos_corrected",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_corrected   = new TH2D("DWC2_pos_corrected",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_corrected = new TH2D("DWC_x_corr_corrected", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_corrected = new TH2D("DWC_y_corr_corrected", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);
    
    // Histograms after PID
    
    TH1F* hist_M1_T1_C_after = new TH1F("M1_T1_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M1_T2_C_after = new TH1F("M1_T2_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M1_T3_C_after = new TH1F("M1_T3_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M1_T4_C_after = new TH1F("M1_T4_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M2_T1_C_after = new TH1F("M2_T1_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M2_T2_C_after = new TH1F("M2_T2_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M2_T3_C_after = new TH1F("M2_T3_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M2_T4_C_after = new TH1F("M2_T4_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M3_T1_C_after = new TH1F("M3_T1_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M3_T2_C_after = new TH1F("M3_T2_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M3_T3_C_after = new TH1F("M3_T3_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M3_T4_C_after = new TH1F("M3_T4_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M4_T1_C_after = new TH1F("M4_T1_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M4_T2_C_after = new TH1F("M4_T2_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M4_T3_C_after = new TH1F("M4_T3_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M4_T4_C_after = new TH1F("M4_T4_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M5_T1_C_after = new TH1F("M5_T1_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M5_T2_C_after = new TH1F("M5_T2_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M5_T3_C_after = new TH1F("M5_T3_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M5_T4_C_after = new TH1F("M5_T4_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M6_T1_C_after = new TH1F("M6_T1_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M6_T2_C_after = new TH1F("M6_T2_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M6_T3_C_after = new TH1F("M6_T3_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M6_T4_C_after = new TH1F("M6_T4_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M7_T1_C_after = new TH1F("M7_T1_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M7_T2_C_after = new TH1F("M7_T2_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M7_T3_C_after = new TH1F("M7_T3_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M7_T4_C_after = new TH1F("M7_T4_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M8_T1_C_after = new TH1F("M8_T1_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M8_T2_C_after = new TH1F("M8_T2_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M8_T3_C_after = new TH1F("M8_T3_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M8_T4_C_after = new TH1F("M8_T4_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M9_T1_C_after = new TH1F("M9_T1_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M9_T2_C_after = new TH1F("M9_T2_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M9_T3_C_after = new TH1F("M9_T3_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M9_T4_C_after = new TH1F("M9_T4_C_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M1_T1_S_after = new TH1F("M1_T1_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M1_T2_S_after = new TH1F("M1_T2_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M1_T3_S_after = new TH1F("M1_T3_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M1_T4_S_after = new TH1F("M1_T4_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M2_T1_S_after = new TH1F("M2_T1_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M2_T2_S_after = new TH1F("M2_T2_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M2_T3_S_after = new TH1F("M2_T3_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M2_T4_S_after = new TH1F("M2_T4_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M3_T1_S_after = new TH1F("M3_T1_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M3_T2_S_after = new TH1F("M3_T2_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M3_T3_S_after = new TH1F("M3_T3_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M3_T4_S_after = new TH1F("M3_T4_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M4_T1_S_after = new TH1F("M4_T1_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M4_T2_S_after = new TH1F("M4_T2_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M4_T3_S_after = new TH1F("M4_T3_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M4_T4_S_after = new TH1F("M4_T4_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M5_T1_S_after = new TH1F("M5_T1_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M5_T2_S_after = new TH1F("M5_T2_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M5_T3_S_after = new TH1F("M5_T3_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M5_T4_S_after = new TH1F("M5_T4_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M6_T1_S_after = new TH1F("M6_T1_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M6_T2_S_after = new TH1F("M6_T2_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M6_T3_S_after = new TH1F("M6_T3_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M6_T4_S_after = new TH1F("M6_T4_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M7_T1_S_after = new TH1F("M7_T1_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M7_T2_S_after = new TH1F("M7_T2_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M7_T3_S_after = new TH1F("M7_T3_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M7_T4_S_after = new TH1F("M7_T4_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M8_T1_S_after = new TH1F("M8_T1_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M8_T2_S_after = new TH1F("M8_T2_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M8_T3_S_after = new TH1F("M8_T3_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M8_T4_S_after = new TH1F("M8_T4_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_M9_T1_S_after = new TH1F("M9_T1_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M9_T2_S_after = new TH1F("M9_T2_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M9_T3_S_after = new TH1F("M9_T3_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_M9_T4_S_after = new TH1F("M9_T4_S_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_Edep_M1_T1_C_after = new TH1F("Edep_M1_T1_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T2_C_after = new TH1F("Edep_M1_T2_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T3_C_after = new TH1F("Edep_M1_T3_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T4_C_after = new TH1F("Edep_M1_T4_C_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M2_T1_C_after = new TH1F("Edep_M2_T1_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T2_C_after = new TH1F("Edep_M2_T2_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T3_C_after = new TH1F("Edep_M2_T3_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T4_C_after = new TH1F("Edep_M2_T4_C_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M3_T1_C_after = new TH1F("Edep_M3_T1_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T2_C_after = new TH1F("Edep_M3_T2_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T3_C_after = new TH1F("Edep_M3_T3_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T4_C_after = new TH1F("Edep_M3_T4_C_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M4_T1_C_after = new TH1F("Edep_M4_T1_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T2_C_after = new TH1F("Edep_M4_T2_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T3_C_after = new TH1F("Edep_M4_T3_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T4_C_after = new TH1F("Edep_M4_T4_C_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M5_T1_C_after = new TH1F("Edep_M5_T1_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T2_C_after = new TH1F("Edep_M5_T2_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T3_C_after = new TH1F("Edep_M5_T3_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T4_C_after = new TH1F("Edep_M5_T4_C_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M6_T1_C_after = new TH1F("Edep_M6_T1_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T2_C_after = new TH1F("Edep_M6_T2_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T3_C_after = new TH1F("Edep_M6_T3_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T4_C_after = new TH1F("Edep_M6_T4_C_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M7_T1_C_after = new TH1F("Edep_M7_T1_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T2_C_after = new TH1F("Edep_M7_T2_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T3_C_after = new TH1F("Edep_M7_T3_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T4_C_after = new TH1F("Edep_M7_T4_C_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M8_T1_C_after = new TH1F("Edep_M8_T1_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T2_C_after = new TH1F("Edep_M8_T2_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T3_C_after = new TH1F("Edep_M8_T3_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T4_C_after = new TH1F("Edep_M8_T4_C_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M9_T1_C_after = new TH1F("Edep_M9_T1_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T2_C_after = new TH1F("Edep_M9_T2_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T3_C_after = new TH1F("Edep_M9_T3_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T4_C_after = new TH1F("Edep_M9_T4_C_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M1_T1_S_after = new TH1F("Edep_M1_T1_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T2_S_after = new TH1F("Edep_M1_T2_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T3_S_after = new TH1F("Edep_M1_T3_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M1_T4_S_after = new TH1F("Edep_M1_T4_S_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M2_T1_S_after = new TH1F("Edep_M2_T1_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T2_S_after = new TH1F("Edep_M2_T2_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T3_S_after = new TH1F("Edep_M2_T3_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_T4_S_after = new TH1F("Edep_M2_T4_S_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M3_T1_S_after = new TH1F("Edep_M3_T1_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T2_S_after = new TH1F("Edep_M3_T2_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T3_S_after = new TH1F("Edep_M3_T3_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_T4_S_after = new TH1F("Edep_M3_T4_S_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M4_T1_S_after = new TH1F("Edep_M4_T1_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T2_S_after = new TH1F("Edep_M4_T2_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T3_S_after = new TH1F("Edep_M4_T3_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_T4_S_after = new TH1F("Edep_M4_T4_S_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M5_T1_S_after = new TH1F("Edep_M5_T1_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T2_S_after = new TH1F("Edep_M5_T2_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T3_S_after = new TH1F("Edep_M5_T3_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_T4_S_after = new TH1F("Edep_M5_T4_S_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M6_T1_S_after = new TH1F("Edep_M6_T1_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T2_S_after = new TH1F("Edep_M6_T2_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T3_S_after = new TH1F("Edep_M6_T3_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_T4_S_after = new TH1F("Edep_M6_T4_S_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M7_T1_S_after = new TH1F("Edep_M7_T1_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T2_S_after = new TH1F("Edep_M7_T2_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T3_S_after = new TH1F("Edep_M7_T3_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_T4_S_after = new TH1F("Edep_M7_T4_S_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M8_T1_S_after = new TH1F("Edep_M8_T1_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T2_S_after = new TH1F("Edep_M8_T2_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T3_S_after = new TH1F("Edep_M8_T3_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_T4_S_after = new TH1F("Edep_M8_T4_S_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M9_T1_S_after = new TH1F("Edep_M9_T1_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T2_S_after = new TH1F("Edep_M9_T2_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T3_S_after = new TH1F("Edep_M9_T3_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_T4_S_after = new TH1F("Edep_M9_T4_S_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M1_C_after = new TH1F("Edep_M1_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_C_after = new TH1F("Edep_M2_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_C_after = new TH1F("Edep_M3_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_C_after = new TH1F("Edep_M4_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_C_after = new TH1F("Edep_M5_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_C_after = new TH1F("Edep_M6_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_C_after = new TH1F("Edep_M7_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_C_after = new TH1F("Edep_M8_C_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_C_after = new TH1F("Edep_M9_C_after" , ";GeV;Event", 100,-10,10);
    
    TH1F* hist_Edep_M1_S_after = new TH1F("Edep_M1_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M2_S_after = new TH1F("Edep_M2_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M3_S_after = new TH1F("Edep_M3_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M4_S_after = new TH1F("Edep_M4_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M5_S_after = new TH1F("Edep_M5_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M6_S_after = new TH1F("Edep_M6_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M7_S_after = new TH1F("Edep_M7_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M8_S_after = new TH1F("Edep_M8_S_after" , ";GeV;Event", 100,-10,10);
    TH1F* hist_Edep_M9_S_after = new TH1F("Edep_M9_S_after" , ";GeV;Event", 100,-10,10);
    
    // TH1F* hist_PS_after = new TH1F("PS_after" , ";intADC;nEvents", 2000, -1000, 1000);
    // TH1F* hist_MC_after = new TH1F("MC_after" , ";intADC;nEvents", 2000, -1000, 1000);
    // TH1F* hist_TC_after = new TH1F("TC_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_PS_after = new TH1F("PS_after" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_MC_after = new TH1F("MC_after" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_TC_after = new TH1F("TC_after" , ";peakADC;nEvents", 1024, 0, 4096);
    
    TH1F* hist_CC1_after = new TH1F("CC1_after", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_CC2_after = new TH1F("CC2_after", ";peakADC;Events", 1024, 0, 4096);
    
    TH1F* hist_LC2_after  = new TH1F("LC2_after"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC3_after  = new TH1F("LC3_after"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC4_after  = new TH1F("LC4_after"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC5_after  = new TH1F("LC5_after"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC7_after  = new TH1F("LC7_after"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC8_after  = new TH1F("LC8_after"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC9_after  = new TH1F("LC9_after"  , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC10_after = new TH1F("LC10_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC11_after = new TH1F("LC11_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC12_after = new TH1F("LC12_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC13_after = new TH1F("LC13_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC14_after = new TH1F("LC14_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC15_after = new TH1F("LC15_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC16_after = new TH1F("LC16_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC19_after = new TH1F("LC19_after" , ";intADC;nEvents", 2000, -1000, 1000);
    TH1F* hist_LC20_after = new TH1F("LC20_after" , ";intADC;nEvents", 2000, -1000, 1000);
    
    TH1F* hist_totalEdep_C_after          = new TH1F("totalEdep_C_after",    ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdep_S_after          = new TH1F("totalEdep_S_after",    ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdep_S_LC_compensated_after = new TH1F("totalEdep_S_LC_compensated_after", ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdep_Comb_after       = new TH1F("totalEdep_Comb_after", ";GeV;Event", 110, -40, 400);
    TH1F* hist_totalEdep_Comb_LC_compensated_after = new TH1F("totalEdep_Comb_LC_compensated_after", ";GeV;Event", 110, -40, 400);
    
    TH1F* hist_totalEdepScaled_C_after    = new TH1F("totalEdepScaled_C_after",    ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdepScaled_S_after    = new TH1F("totalEdepScaled_S_after",    ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdepScaled_S_LC_compensated_after = new TH1F("totalEdepScaled_S_LC_compensated_after", ";GeV;Event", 100,-10,10);
    TH1F* hist_totalEdepScaled_Comb_after = new TH1F("totalEdepScaled_Comb_after", ";GeV;Event", 110, -40, 400);
    TH1F* hist_totalEdepScaled_Comb_LC_compensated_after = new TH1F("totalEdepScaled_Comb_LC_compensated_after", ";GeV;Event", 110, -40, 400);

    TH2D* hist_DWC1_pos_after   = new TH2D("DWC1_pos_after",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_after   = new TH2D("DWC2_pos_after",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_after = new TH2D("DWC_x_corr_after", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_after = new TH2D("DWC_y_corr_after", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);

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
        
        std::vector<float> intADC_M1_C; std::vector<float> intADC_M1_S;
        std::vector<float> intADC_M2_C; std::vector<float> intADC_M2_S;
        std::vector<float> intADC_M3_C; std::vector<float> intADC_M3_S;
        std::vector<float> intADC_M4_C; std::vector<float> intADC_M4_S;
        std::vector<float> intADC_M5_C; std::vector<float> intADC_M5_S;
        std::vector<float> intADC_M6_C; std::vector<float> intADC_M6_S;
        std::vector<float> intADC_M7_C; std::vector<float> intADC_M7_S;
        std::vector<float> intADC_M8_C; std::vector<float> intADC_M8_S;
        std::vector<float> intADC_M9_C; std::vector<float> intADC_M9_S;

        // Get IntADC for M1, M2, M3, M4, M5, M6, M7, M8, M9
        intADC_M1_C.emplace_back( GetInt(*wave_M1_T1_C, C_first, C_last) );
        intADC_M1_C.emplace_back( GetInt(*wave_M1_T2_C, C_first, C_last) );
        intADC_M1_C.emplace_back( GetInt(*wave_M1_T3_C, C_first, C_last) );
        intADC_M1_C.emplace_back( GetInt(*wave_M1_T4_C, C_first, C_last) );

        intADC_M2_C.emplace_back( GetInt(*wave_M2_T1_C, C_first, C_last) );
        intADC_M2_C.emplace_back( GetInt(*wave_M2_T2_C, C_first, C_last) );
        intADC_M2_C.emplace_back( GetInt(*wave_M2_T3_C, C_first, C_last) );
        intADC_M2_C.emplace_back( GetInt(*wave_M2_T4_C, C_first, C_last) );

        intADC_M3_C.emplace_back( GetInt(*wave_M3_T1_C, C_first, C_last) );
        intADC_M3_C.emplace_back( GetInt(*wave_M3_T2_C, C_first, C_last) );
        intADC_M3_C.emplace_back( GetInt(*wave_M3_T3_C, C_first, C_last) );
        intADC_M3_C.emplace_back( GetInt(*wave_M3_T4_C, C_first, C_last) );

        intADC_M4_C.emplace_back( GetInt(*wave_M4_T1_C, C_first, C_last) );
        intADC_M4_C.emplace_back( GetInt(*wave_M4_T2_C, C_first, C_last) );
        intADC_M4_C.emplace_back( GetInt(*wave_M4_T3_C, C_first, C_last) );
        intADC_M4_C.emplace_back( GetInt(*wave_M4_T4_C, C_first, C_last) );
        
        intADC_M5_C.emplace_back( GetInt(*wave_M5_T1_C, C_first, C_last) );
        intADC_M5_C.emplace_back( GetInt(*wave_M5_T2_C, C_first, C_last) );
        intADC_M5_C.emplace_back( GetInt(*wave_M5_T3_C, C_first, C_last) );
        intADC_M5_C.emplace_back( GetInt(*wave_M5_T4_C, C_first, C_last) );

        intADC_M6_C.emplace_back( GetInt(*wave_M6_T1_C, C_first, C_last) );
        intADC_M6_C.emplace_back( GetInt(*wave_M6_T2_C, C_first, C_last) );
        intADC_M6_C.emplace_back( GetInt(*wave_M6_T3_C, C_first, C_last) );
        intADC_M6_C.emplace_back( GetInt(*wave_M6_T4_C, C_first, C_last) );

        intADC_M7_C.emplace_back( GetInt(*wave_M7_T1_C, C_first, C_last) );
        intADC_M7_C.emplace_back( GetInt(*wave_M7_T2_C, C_first, C_last) );
        intADC_M7_C.emplace_back( GetInt(*wave_M7_T3_C, C_first, C_last) );
        intADC_M7_C.emplace_back( GetInt(*wave_M7_T4_C, C_first, C_last) );

        intADC_M8_C.emplace_back( GetInt(*wave_M8_T1_C, C_first, C_last) );
        intADC_M8_C.emplace_back( GetInt(*wave_M8_T2_C, C_first, C_last) );
        intADC_M8_C.emplace_back( GetInt(*wave_M8_T3_C, C_first, C_last) );
        intADC_M8_C.emplace_back( GetInt(*wave_M8_T4_C, C_first, C_last) );

        intADC_M9_C.emplace_back( GetInt(*wave_M9_T1_C, C_first, C_last) );
        intADC_M9_C.emplace_back( GetInt(*wave_M9_T2_C, C_first, C_last) );
        intADC_M9_C.emplace_back( GetInt(*wave_M9_T3_C, C_first, C_last) );
        intADC_M9_C.emplace_back( GetInt(*wave_M9_T4_C, C_first, C_last) );

        intADC_M1_S.emplace_back( GetInt(*wave_M1_T1_S, S_first, S_last) );
        intADC_M1_S.emplace_back( GetInt(*wave_M1_T2_S, S_first, S_last) );
        intADC_M1_S.emplace_back( GetInt(*wave_M1_T3_S, S_first, S_last) );
        intADC_M1_S.emplace_back( GetInt(*wave_M1_T4_S, S_first, S_last) );

        intADC_M2_S.emplace_back( GetInt(*wave_M2_T1_S, S_first, S_last) );
        intADC_M2_S.emplace_back( GetInt(*wave_M2_T2_S, S_first, S_last) );
        intADC_M2_S.emplace_back( GetInt(*wave_M2_T3_S, S_first, S_last) );
        intADC_M2_S.emplace_back( GetInt(*wave_M2_T4_S, S_first, S_last) );

        intADC_M3_S.emplace_back( GetInt(*wave_M3_T1_S, S_first, S_last) );
        intADC_M3_S.emplace_back( GetInt(*wave_M3_T2_S, S_first, S_last) );
        intADC_M3_S.emplace_back( GetInt(*wave_M3_T3_S, S_first, S_last) );
        intADC_M3_S.emplace_back( GetInt(*wave_M3_T4_S, S_first, S_last) );

        intADC_M4_S.emplace_back( GetInt(*wave_M4_T1_S, S_first, S_last) );
        intADC_M4_S.emplace_back( GetInt(*wave_M4_T2_S, S_first, S_last) );
        intADC_M4_S.emplace_back( GetInt(*wave_M4_T3_S, S_first, S_last) );
        intADC_M4_S.emplace_back( GetInt(*wave_M4_T4_S, S_first, S_last) );

        intADC_M5_S.emplace_back( GetInt(*wave_M5_T1_S, S_first, S_last) );
        intADC_M5_S.emplace_back( GetInt(*wave_M5_T2_S, S_first, S_last) );
        intADC_M5_S.emplace_back( GetInt(*wave_M5_T3_S, S_first, S_last) );
        intADC_M5_S.emplace_back( GetInt(*wave_M5_T4_S, S_first, S_last) );

        intADC_M6_S.emplace_back( GetInt(*wave_M6_T1_S, S_first, S_last) );
        intADC_M6_S.emplace_back( GetInt(*wave_M6_T2_S, S_first, S_last) );
        intADC_M6_S.emplace_back( GetInt(*wave_M6_T3_S, S_first, S_last) );
        intADC_M6_S.emplace_back( GetInt(*wave_M6_T4_S, S_first, S_last) );

        intADC_M7_S.emplace_back( GetInt(*wave_M7_T1_S, S_first, S_last) );
        intADC_M7_S.emplace_back( GetInt(*wave_M7_T2_S, S_first, S_last) );
        intADC_M7_S.emplace_back( GetInt(*wave_M7_T3_S, S_first, S_last) );
        intADC_M7_S.emplace_back( GetInt(*wave_M7_T4_S, S_first, S_last) );

        intADC_M8_S.emplace_back( GetInt(*wave_M8_T1_S, S_first, S_last) );
        intADC_M8_S.emplace_back( GetInt(*wave_M8_T2_S, S_first, S_last) );
        intADC_M8_S.emplace_back( GetInt(*wave_M8_T3_S, S_first, S_last) );
        intADC_M8_S.emplace_back( GetInt(*wave_M8_T4_S, S_first, S_last) );

        intADC_M9_S.emplace_back( GetInt(*wave_M9_T1_S, S_first, S_last) );
        intADC_M9_S.emplace_back( GetInt(*wave_M9_T2_S, S_first, S_last) );
        intADC_M9_S.emplace_back( GetInt(*wave_M9_T3_S, S_first, S_last) );
        intADC_M9_S.emplace_back( GetInt(*wave_M9_T4_S, S_first, S_last) );

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
                
        float intADC_LC2  = GetInt(*wave_LC2 , LC_first, LC_last);
        float intADC_LC3  = GetInt(*wave_LC3 , LC_first, LC_last);
        float intADC_LC4  = GetInt(*wave_LC4 , LC_first, LC_last);
        float intADC_LC5  = GetInt(*wave_LC5 , LC_first, LC_last);
        float intADC_LC7  = GetInt(*wave_LC7 , LC_first, LC_last);
        float intADC_LC8  = GetInt(*wave_LC8 , LC_first, LC_last);
        float intADC_LC9  = GetInt(*wave_LC9 , LC_first, LC_last);
        float intADC_LC10 = GetInt(*wave_LC10, LC_first, LC_last);
        float intADC_LC11 = GetInt(*wave_LC11, LC_first, LC_last);
        float intADC_LC12 = GetInt(*wave_LC12, LC_first, LC_last);
        float intADC_LC13 = GetInt(*wave_LC13, LC_first, LC_last);
        float intADC_LC14 = GetInt(*wave_LC14, LC_first, LC_last);
        float intADC_LC15 = GetInt(*wave_LC15, LC_first, LC_last);
        float intADC_LC16 = GetInt(*wave_LC16, LC_first, LC_last);
        float intADC_LC19 = GetInt(*wave_LC19, LC_first, LC_last);
        float intADC_LC20 = GetInt(*wave_LC20, LC_first, LC_last);

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
        // if (Edep_M1T1_C < 0) Edep_M1T1_C = 0;
        // if (Edep_M1T2_C < 0) Edep_M1T2_C = 0;
        // if (Edep_M1T3_C < 0) Edep_M1T3_C = 0;
        // if (Edep_M1T4_C < 0) Edep_M1T4_C = 0;
        
        float Edep_M2T1_C = (intADC_M2_C.at(0) * calib_M2_T1_C);
        float Edep_M2T2_C = (intADC_M2_C.at(1) * calib_M2_T2_C);
        float Edep_M2T3_C = (intADC_M2_C.at(2) * calib_M2_T3_C);
        float Edep_M2T4_C = (intADC_M2_C.at(3) * calib_M2_T4_C);
        // if (Edep_M2T1_C < 0) Edep_M2T1_C = 0;
        // if (Edep_M2T2_C < 0) Edep_M2T2_C = 0;
        // if (Edep_M2T3_C < 0) Edep_M2T3_C = 0;
        // if (Edep_M2T4_C < 0) Edep_M2T4_C = 0;
        
        float Edep_M3T1_C = (intADC_M3_C.at(0) * calib_M3_T1_C);
        float Edep_M3T2_C = (intADC_M3_C.at(1) * calib_M3_T2_C);
        float Edep_M3T3_C = (intADC_M3_C.at(2) * calib_M3_T3_C);
        float Edep_M3T4_C = (intADC_M3_C.at(3) * calib_M3_T4_C);
        // if (Edep_M3T1_C < 0) Edep_M3T1_C = 0;
        // if (Edep_M3T2_C < 0) Edep_M3T2_C = 0;
        // if (Edep_M3T3_C < 0) Edep_M3T3_C = 0;
        // if (Edep_M3T4_C < 0) Edep_M3T4_C = 0;
        
        float Edep_M4T1_C = (intADC_M4_C.at(0) * calib_M4_T1_C);
        float Edep_M4T2_C = (intADC_M4_C.at(1) * calib_M4_T2_C);
        float Edep_M4T3_C = (intADC_M4_C.at(2) * calib_M4_T3_C);
        float Edep_M4T4_C = (intADC_M4_C.at(3) * calib_M4_T4_C);
        // if (Edep_M4T1_C < 0) Edep_M4T1_C = 0;
        // if (Edep_M4T2_C < 0) Edep_M4T2_C = 0;
        // if (Edep_M4T3_C < 0) Edep_M4T3_C = 0;
        // if (Edep_M4T4_C < 0) Edep_M4T4_C = 0;
        
        float Edep_M5T1_C = (intADC_M5_C.at(0) * calib_M5_T1_C);
        float Edep_M5T2_C = (intADC_M5_C.at(1) * calib_M5_T2_C);
        float Edep_M5T3_C = (intADC_M5_C.at(2) * calib_M5_T3_C);
        float Edep_M5T4_C = (intADC_M5_C.at(3) * calib_M5_T4_C);
        // if (Edep_M5T1_C < 0) Edep_M5T1_C = 0;
        // if (Edep_M5T2_C < 0) Edep_M5T2_C = 0;
        // if (Edep_M5T3_C < 0) Edep_M5T3_C = 0;
        // if (Edep_M5T4_C < 0) Edep_M5T4_C = 0;
        
        float Edep_M6T1_C = (intADC_M6_C.at(0) * calib_M6_T1_C);
        float Edep_M6T2_C = (intADC_M6_C.at(1) * calib_M6_T2_C);
        float Edep_M6T3_C = (intADC_M6_C.at(2) * calib_M6_T3_C);
        float Edep_M6T4_C = (intADC_M6_C.at(3) * calib_M6_T4_C);
        // if (Edep_M6T1_C < 0) Edep_M6T1_C = 0;
        // if (Edep_M6T2_C < 0) Edep_M6T2_C = 0;
        // if (Edep_M6T3_C < 0) Edep_M6T3_C = 0;
        // if (Edep_M6T4_C < 0) Edep_M6T4_C = 0;
        
        float Edep_M7T1_C = (intADC_M7_C.at(0) * calib_M7_T1_C);
        float Edep_M7T2_C = (intADC_M7_C.at(1) * calib_M7_T2_C);
        float Edep_M7T3_C = (intADC_M7_C.at(2) * calib_M7_T3_C);
        float Edep_M7T4_C = (intADC_M7_C.at(3) * calib_M7_T4_C);
        // if (Edep_M7T1_C < 0) Edep_M7T1_C = 0;
        // if (Edep_M7T2_C < 0) Edep_M7T2_C = 0;
        // if (Edep_M7T3_C < 0) Edep_M7T3_C = 0;
        // if (Edep_M7T4_C < 0) Edep_M7T4_C = 0;
        
        float Edep_M8T1_C = (intADC_M8_C.at(0) * calib_M8_T1_C);
        float Edep_M8T2_C = (intADC_M8_C.at(1) * calib_M8_T2_C);
        float Edep_M8T3_C = (intADC_M8_C.at(2) * calib_M8_T3_C);
        float Edep_M8T4_C = (intADC_M8_C.at(3) * calib_M8_T4_C);
        // if (Edep_M8T1_C < 0) Edep_M8T1_C = 0;
        // if (Edep_M8T2_C < 0) Edep_M8T2_C = 0;
        // if (Edep_M8T3_C < 0) Edep_M8T3_C = 0;
        // if (Edep_M8T4_C < 0) Edep_M8T4_C = 0;
        
        float Edep_M9T1_C = (intADC_M9_C.at(0) * calib_M9_T1_C);
        float Edep_M9T2_C = (intADC_M9_C.at(1) * calib_M9_T2_C);
        float Edep_M9T3_C = (intADC_M9_C.at(2) * calib_M9_T3_C);
        float Edep_M9T4_C = (intADC_M9_C.at(3) * calib_M9_T4_C);
        // if (Edep_M9T1_C < 0) Edep_M9T1_C = 0;
        // if (Edep_M9T2_C < 0) Edep_M9T2_C = 0;
        // if (Edep_M9T3_C < 0) Edep_M9T3_C = 0;
        // if (Edep_M9T4_C < 0) Edep_M9T4_C = 0;
        
        float Edep_M1_C = Edep_M1T1_C + Edep_M1T2_C + Edep_M1T3_C + Edep_M1T4_C;
        float Edep_M2_C = Edep_M2T1_C + Edep_M2T2_C + Edep_M2T3_C + Edep_M2T4_C;
        float Edep_M3_C = Edep_M3T1_C + Edep_M3T2_C + Edep_M3T3_C + Edep_M3T4_C;
        float Edep_M4_C = Edep_M4T1_C + Edep_M4T2_C + Edep_M4T3_C + Edep_M4T4_C;
        float Edep_M5_C = Edep_M5T1_C + Edep_M5T2_C + Edep_M5T3_C + Edep_M5T4_C;
        float Edep_M6_C = Edep_M6T1_C + Edep_M6T2_C + Edep_M6T3_C + Edep_M6T4_C;
        float Edep_M7_C = Edep_M7T1_C + Edep_M7T2_C + Edep_M7T3_C + Edep_M7T4_C;
        float Edep_M8_C = Edep_M8T1_C + Edep_M8T2_C + Edep_M8T3_C + Edep_M8T4_C;
        float Edep_M9_C = Edep_M9T1_C + Edep_M9T2_C + Edep_M9T3_C + Edep_M9T4_C;
        
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
        // if (Edep_M1T1_S < 0) Edep_M1T1_S = 0;
        // if (Edep_M1T2_S < 0) Edep_M1T2_S = 0;
        // if (Edep_M1T3_S < 0) Edep_M1T3_S = 0;
        // if (Edep_M1T4_S < 0) Edep_M1T4_S = 0;
        
        float Edep_M2T1_S = (intADC_M2_S.at(0) * calib_M2_T1_S);
        float Edep_M2T2_S = (intADC_M2_S.at(1) * calib_M2_T2_S);
        float Edep_M2T3_S = (intADC_M2_S.at(2) * calib_M2_T3_S);
        float Edep_M2T4_S = (intADC_M2_S.at(3) * calib_M2_T4_S);
        // if (Edep_M2T1_S < 0) Edep_M2T1_S = 0;
        // if (Edep_M2T2_S < 0) Edep_M2T2_S = 0;
        // if (Edep_M2T3_S < 0) Edep_M2T3_S = 0;
        // if (Edep_M2T4_S < 0) Edep_M2T4_S = 0;
        
        float Edep_M3T1_S = (intADC_M3_S.at(0) * calib_M3_T1_S);
        float Edep_M3T2_S = (intADC_M3_S.at(1) * calib_M3_T2_S);
        float Edep_M3T3_S = (intADC_M3_S.at(2) * calib_M3_T3_S);
        float Edep_M3T4_S = (intADC_M3_S.at(3) * calib_M3_T4_S);
        // if (Edep_M3T1_S < 0) Edep_M3T1_S = 0;
        // if (Edep_M3T2_S < 0) Edep_M3T2_S = 0;
        // if (Edep_M3T3_S < 0) Edep_M3T3_S = 0;
        // if (Edep_M3T4_S < 0) Edep_M3T4_S = 0;
        
        float Edep_M4T1_S = (intADC_M4_S.at(0) * calib_M4_T1_S);
        float Edep_M4T2_S = (intADC_M4_S.at(1) * calib_M4_T2_S);
        float Edep_M4T3_S = (intADC_M4_S.at(2) * calib_M4_T3_S);
        float Edep_M4T4_S = (intADC_M4_S.at(3) * calib_M4_T4_S);
        // if (Edep_M4T1_S < 0) Edep_M4T1_S = 0;
        // if (Edep_M4T2_S < 0) Edep_M4T2_S = 0;
        // if (Edep_M4T3_S < 0) Edep_M4T3_S = 0;
        // if (Edep_M4T4_S < 0) Edep_M4T4_S = 0;
        
        float Edep_M5T1_S = (intADC_M5_S.at(0) * calib_M5_T1_S);
        float Edep_M5T2_S = (intADC_M5_S.at(1) * calib_M5_T2_S);
        float Edep_M5T3_S = (intADC_M5_S.at(2) * calib_M5_T3_S);
        float Edep_M5T4_S = (intADC_M5_S.at(3) * calib_M5_T4_S);
        // if (Edep_M5T1_S < 0) Edep_M5T1_S = 0;
        // if (Edep_M5T2_S < 0) Edep_M5T2_S = 0;
        // if (Edep_M5T3_S < 0) Edep_M5T3_S = 0;
        // if (Edep_M5T4_S < 0) Edep_M5T4_S = 0;
        
        float Edep_M6T1_S = (intADC_M6_S.at(0) * calib_M6_T1_S);
        float Edep_M6T2_S = (intADC_M6_S.at(1) * calib_M6_T2_S);
        float Edep_M6T3_S = (intADC_M6_S.at(2) * calib_M6_T3_S);
        float Edep_M6T4_S = (intADC_M6_S.at(3) * calib_M6_T4_S);
        // if (Edep_M6T1_S < 0) Edep_M6T1_S = 0;
        // if (Edep_M6T2_S < 0) Edep_M6T2_S = 0;
        // if (Edep_M6T3_S < 0) Edep_M6T3_S = 0;
        // if (Edep_M6T4_S < 0) Edep_M6T4_S = 0;
        
        float Edep_M7T1_S = (intADC_M7_S.at(0) * calib_M7_T1_S);
        float Edep_M7T2_S = (intADC_M7_S.at(1) * calib_M7_T2_S);
        float Edep_M7T3_S = (intADC_M7_S.at(2) * calib_M7_T3_S);
        float Edep_M7T4_S = (intADC_M7_S.at(3) * calib_M7_T4_S);
        // if (Edep_M7T1_S < 0) Edep_M7T1_S = 0;
        // if (Edep_M7T2_S < 0) Edep_M7T2_S = 0;
        // if (Edep_M7T3_S < 0) Edep_M7T3_S = 0;
        // if (Edep_M7T4_S < 0) Edep_M7T4_S = 0;
        
        float Edep_M8T1_S = (intADC_M8_S.at(0) * calib_M8_T1_S);
        float Edep_M8T2_S = (intADC_M8_S.at(1) * calib_M8_T2_S);
        float Edep_M8T3_S = (intADC_M8_S.at(2) * calib_M8_T3_S);
        float Edep_M8T4_S = (intADC_M8_S.at(3) * calib_M8_T4_S);
        // if (Edep_M8T1_S < 0) Edep_M8T1_S = 0;
        // if (Edep_M8T2_S < 0) Edep_M8T2_S = 0;
        // if (Edep_M8T3_S < 0) Edep_M8T3_S = 0;
        // if (Edep_M8T4_S < 0) Edep_M8T4_S = 0;
        
        float Edep_M9T1_S = (intADC_M9_S.at(0) * calib_M9_T1_S);
        float Edep_M9T2_S = (intADC_M9_S.at(1) * calib_M9_T2_S);
        float Edep_M9T3_S = (intADC_M9_S.at(2) * calib_M9_T3_S);
        float Edep_M9T4_S = (intADC_M9_S.at(3) * calib_M9_T4_S);
        // if (Edep_M9T1_S < 0) Edep_M9T1_S = 0;
        // if (Edep_M9T2_S < 0) Edep_M9T2_S = 0;
        // if (Edep_M9T3_S < 0) Edep_M9T3_S = 0;
        // if (Edep_M9T4_S < 0) Edep_M9T4_S = 0;
        
        float Edep_M1_S = Edep_M1T1_S + Edep_M1T2_S + Edep_M1T3_S + Edep_M1T4_S;
        float Edep_M2_S = Edep_M2T1_S + Edep_M2T2_S + Edep_M2T3_S + Edep_M2T4_S;
        float Edep_M3_S = Edep_M3T1_S + Edep_M3T2_S + Edep_M3T3_S + Edep_M3T4_S;
        float Edep_M4_S = Edep_M4T1_S + Edep_M4T2_S + Edep_M4T3_S + Edep_M4T4_S;
        float Edep_M5_S = Edep_M5T1_S + Edep_M5T2_S + Edep_M5T3_S + Edep_M5T4_S;
        float Edep_M6_S = Edep_M6T1_S + Edep_M6T2_S + Edep_M6T3_S + Edep_M6T4_S;
        float Edep_M7_S = Edep_M7T1_S + Edep_M7T2_S + Edep_M7T3_S + Edep_M7T4_S;
        float Edep_M8_S = Edep_M8T1_S + Edep_M8T2_S + Edep_M8T3_S + Edep_M8T4_S;
        float Edep_M9_S = Edep_M9T1_S + Edep_M9T2_S + Edep_M9T3_S + Edep_M9T4_S;
        
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
	auto pedcor_M1_T1_C = pedcorwave(*wave_M1_T1_C,100);
	auto pedcor_M1_T2_C = pedcorwave(*wave_M1_T2_C,100);
	auto pedcor_M1_T3_C = pedcorwave(*wave_M1_T3_C,100);
	auto pedcor_M1_T4_C = pedcorwave(*wave_M1_T4_C,100);

	auto pedcor_M2_T1_C = pedcorwave(*wave_M2_T1_C,100);
	auto pedcor_M2_T2_C = pedcorwave(*wave_M2_T2_C,100);
	auto pedcor_M2_T3_C = pedcorwave(*wave_M2_T3_C,100);
	auto pedcor_M2_T4_C = pedcorwave(*wave_M2_T4_C,100);

	auto pedcor_M3_T1_C = pedcorwave(*wave_M3_T1_C,100);
	auto pedcor_M3_T2_C = pedcorwave(*wave_M3_T2_C,100);
	auto pedcor_M3_T3_C = pedcorwave(*wave_M3_T3_C,100);
	auto pedcor_M3_T4_C = pedcorwave(*wave_M3_T4_C,100);

	auto pedcor_M4_T1_C = pedcorwave(*wave_M4_T1_C,100);
	auto pedcor_M4_T2_C = pedcorwave(*wave_M4_T2_C,100);
	auto pedcor_M4_T3_C = pedcorwave(*wave_M4_T3_C,100);
	auto pedcor_M4_T4_C = pedcorwave(*wave_M4_T4_C,100);

	auto pedcor_M5_T1_C = pedcorwave(*wave_M5_T1_C,100);
	auto pedcor_M5_T2_C = pedcorwave(*wave_M5_T2_C,100);
	auto pedcor_M5_T3_C = pedcorwave(*wave_M5_T3_C,100);
	auto pedcor_M5_T4_C = pedcorwave(*wave_M5_T4_C,100);

	auto pedcor_M6_T1_C = pedcorwave(*wave_M6_T1_C,100);
	auto pedcor_M6_T2_C = pedcorwave(*wave_M6_T2_C,100);
	auto pedcor_M6_T3_C = pedcorwave(*wave_M6_T3_C,100);
	auto pedcor_M6_T4_C = pedcorwave(*wave_M6_T4_C,100);

	auto pedcor_M7_T1_C = pedcorwave(*wave_M7_T1_C,100);
	auto pedcor_M7_T2_C = pedcorwave(*wave_M7_T2_C,100);
	auto pedcor_M7_T3_C = pedcorwave(*wave_M7_T3_C,100);
	auto pedcor_M7_T4_C = pedcorwave(*wave_M7_T4_C,100);

	auto pedcor_M8_T1_C = pedcorwave(*wave_M8_T1_C,100);
	auto pedcor_M8_T2_C = pedcorwave(*wave_M8_T2_C,100);
	auto pedcor_M8_T3_C = pedcorwave(*wave_M8_T3_C,100);
	auto pedcor_M8_T4_C = pedcorwave(*wave_M8_T4_C,100);

	auto pedcor_M9_T1_C = pedcorwave(*wave_M9_T1_C,100);
	auto pedcor_M9_T2_C = pedcorwave(*wave_M9_T2_C,100);
	auto pedcor_M9_T3_C = pedcorwave(*wave_M9_T3_C,100);
	auto pedcor_M9_T4_C = pedcorwave(*wave_M9_T4_C,100);

	auto pedcor_M1_T1_S = pedcorwave(*wave_M1_T1_S,100);
	auto pedcor_M1_T2_S = pedcorwave(*wave_M1_T2_S,100);
	auto pedcor_M1_T3_S = pedcorwave(*wave_M1_T3_S,100);
	auto pedcor_M1_T4_S = pedcorwave(*wave_M1_T4_S,100);

	auto pedcor_M2_T1_S = pedcorwave(*wave_M2_T1_S,100);
	auto pedcor_M2_T2_S = pedcorwave(*wave_M2_T2_S,100);
	auto pedcor_M2_T3_S = pedcorwave(*wave_M2_T3_S,100);
	auto pedcor_M2_T4_S = pedcorwave(*wave_M2_T4_S,100);

	auto pedcor_M3_T1_S = pedcorwave(*wave_M3_T1_S,100);
	auto pedcor_M3_T2_S = pedcorwave(*wave_M3_T2_S,100);
	auto pedcor_M3_T3_S = pedcorwave(*wave_M3_T3_S,100);
	auto pedcor_M3_T4_S = pedcorwave(*wave_M3_T4_S,100);

	auto pedcor_M4_T1_S = pedcorwave(*wave_M4_T1_S,100);
	auto pedcor_M4_T2_S = pedcorwave(*wave_M4_T2_S,100);
	auto pedcor_M4_T3_S = pedcorwave(*wave_M4_T3_S,100);
	auto pedcor_M4_T4_S = pedcorwave(*wave_M4_T4_S,100);

	auto pedcor_M5_T1_S = pedcorwave(*wave_M5_T1_S,100);
	auto pedcor_M5_T2_S = pedcorwave(*wave_M5_T2_S,100);
	auto pedcor_M5_T3_S = pedcorwave(*wave_M5_T3_S,100);
	auto pedcor_M5_T4_S = pedcorwave(*wave_M5_T4_S,100);

	auto pedcor_M6_T1_S = pedcorwave(*wave_M6_T1_S,100);
	auto pedcor_M6_T2_S = pedcorwave(*wave_M6_T2_S,100);
	auto pedcor_M6_T3_S = pedcorwave(*wave_M6_T3_S,100);
	auto pedcor_M6_T4_S = pedcorwave(*wave_M6_T4_S,100);

	auto pedcor_M7_T1_S = pedcorwave(*wave_M7_T1_S,100);
	auto pedcor_M7_T2_S = pedcorwave(*wave_M7_T2_S,100);
	auto pedcor_M7_T3_S = pedcorwave(*wave_M7_T3_S,100);
	auto pedcor_M7_T4_S = pedcorwave(*wave_M7_T4_S,100);

	auto pedcor_M8_T1_S = pedcorwave(*wave_M8_T1_S,100);
	auto pedcor_M8_T2_S = pedcorwave(*wave_M8_T2_S,100);
	auto pedcor_M8_T3_S = pedcorwave(*wave_M8_T3_S,100);
	auto pedcor_M8_T4_S = pedcorwave(*wave_M8_T4_S,100);

	auto pedcor_M9_T1_S = pedcorwave(*wave_M9_T1_S,100);
	auto pedcor_M9_T2_S = pedcorwave(*wave_M9_T2_S,100);
	auto pedcor_M9_T3_S = pedcorwave(*wave_M9_T3_S,100);
	auto pedcor_M9_T4_S = pedcorwave(*wave_M9_T4_S,100);

        for (int i=1;i<101;i++){ 
            hist_M1_T1_C_100->Fill(pedcor_M1_T1_C.at(i));
            hist_M1_T2_C_100->Fill(pedcor_M1_T2_C.at(i));
            hist_M1_T3_C_100->Fill(pedcor_M1_T3_C.at(i));
            hist_M1_T4_C_100->Fill(pedcor_M1_T4_C.at(i));
            
            hist_M2_T1_C_100->Fill(pedcor_M2_T1_C.at(i));
            hist_M2_T2_C_100->Fill(pedcor_M2_T2_C.at(i));
            hist_M2_T3_C_100->Fill(pedcor_M2_T3_C.at(i));
            hist_M2_T4_C_100->Fill(pedcor_M2_T4_C.at(i));
            
            hist_M3_T1_C_100->Fill(pedcor_M3_T1_C.at(i));
            hist_M3_T2_C_100->Fill(pedcor_M3_T2_C.at(i));
            hist_M3_T3_C_100->Fill(pedcor_M3_T3_C.at(i));
            hist_M3_T4_C_100->Fill(pedcor_M3_T4_C.at(i));
            
            hist_M4_T1_C_100->Fill(pedcor_M4_T1_C.at(i));
            hist_M4_T2_C_100->Fill(pedcor_M4_T2_C.at(i));
            hist_M4_T3_C_100->Fill(pedcor_M4_T3_C.at(i));
            hist_M4_T4_C_100->Fill(pedcor_M4_T4_C.at(i));
            
            hist_M5_T1_C_100->Fill(pedcor_M5_T1_C.at(i));
            hist_M5_T2_C_100->Fill(pedcor_M5_T2_C.at(i));
            hist_M5_T3_C_100->Fill(pedcor_M5_T3_C.at(i));
            hist_M5_T4_C_100->Fill(pedcor_M5_T4_C.at(i));
            
            hist_M6_T1_C_100->Fill(pedcor_M6_T1_C.at(i));
            hist_M6_T2_C_100->Fill(pedcor_M6_T2_C.at(i));
            hist_M6_T3_C_100->Fill(pedcor_M6_T3_C.at(i));
            hist_M6_T4_C_100->Fill(pedcor_M6_T4_C.at(i));
            
            hist_M7_T1_C_100->Fill(pedcor_M7_T1_C.at(i));
            hist_M7_T2_C_100->Fill(pedcor_M7_T2_C.at(i));
            hist_M7_T3_C_100->Fill(pedcor_M7_T3_C.at(i));
            hist_M7_T4_C_100->Fill(pedcor_M7_T4_C.at(i));
            
            hist_M8_T1_C_100->Fill(pedcor_M8_T1_C.at(i));
            hist_M8_T2_C_100->Fill(pedcor_M8_T2_C.at(i));
            hist_M8_T3_C_100->Fill(pedcor_M8_T3_C.at(i));
            hist_M8_T4_C_100->Fill(pedcor_M8_T4_C.at(i));

            hist_M9_T1_C_100->Fill(pedcor_M9_T1_C.at(i));
            hist_M9_T2_C_100->Fill(pedcor_M9_T2_C.at(i));
            hist_M9_T3_C_100->Fill(pedcor_M9_T3_C.at(i));
            hist_M9_T4_C_100->Fill(pedcor_M9_T4_C.at(i));

            hist_M1_T1_S_100->Fill(pedcor_M1_T1_S.at(i));
            hist_M1_T2_S_100->Fill(pedcor_M1_T2_S.at(i));
            hist_M1_T3_S_100->Fill(pedcor_M1_T3_S.at(i));
            hist_M1_T4_S_100->Fill(pedcor_M1_T4_S.at(i));

            hist_M2_T1_S_100->Fill(pedcor_M2_T1_S.at(i));
            hist_M2_T2_S_100->Fill(pedcor_M2_T2_S.at(i));
            hist_M2_T3_S_100->Fill(pedcor_M2_T3_S.at(i));
            hist_M2_T4_S_100->Fill(pedcor_M2_T4_S.at(i));

            hist_M3_T1_S_100->Fill(pedcor_M3_T1_S.at(i));
            hist_M3_T2_S_100->Fill(pedcor_M3_T2_S.at(i));
            hist_M3_T3_S_100->Fill(pedcor_M3_T3_S.at(i));
            hist_M3_T4_S_100->Fill(pedcor_M3_T4_S.at(i));

            hist_M4_T1_S_100->Fill(pedcor_M4_T1_S.at(i));
            hist_M4_T2_S_100->Fill(pedcor_M4_T2_S.at(i));
            hist_M4_T3_S_100->Fill(pedcor_M4_T3_S.at(i));
            hist_M4_T4_S_100->Fill(pedcor_M4_T4_S.at(i));

            hist_M5_T1_S_100->Fill(pedcor_M5_T1_S.at(i));
            hist_M5_T2_S_100->Fill(pedcor_M5_T2_S.at(i));
            hist_M5_T3_S_100->Fill(pedcor_M5_T3_S.at(i));
            hist_M5_T4_S_100->Fill(pedcor_M5_T4_S.at(i));

            hist_M6_T1_S_100->Fill(pedcor_M6_T1_S.at(i));
            hist_M6_T2_S_100->Fill(pedcor_M6_T2_S.at(i));
            hist_M6_T3_S_100->Fill(pedcor_M6_T3_S.at(i));
            hist_M6_T4_S_100->Fill(pedcor_M6_T4_S.at(i));

            hist_M7_T1_S_100->Fill(pedcor_M7_T1_S.at(i));
            hist_M7_T2_S_100->Fill(pedcor_M7_T2_S.at(i));
            hist_M7_T3_S_100->Fill(pedcor_M7_T3_S.at(i));
            hist_M7_T4_S_100->Fill(pedcor_M7_T4_S.at(i));

            hist_M8_T1_S_100->Fill(pedcor_M8_T1_S.at(i));
            hist_M8_T2_S_100->Fill(pedcor_M8_T2_S.at(i));
            hist_M8_T3_S_100->Fill(pedcor_M8_T3_S.at(i));
            hist_M8_T4_S_100->Fill(pedcor_M8_T4_S.at(i));

            hist_M9_T1_S_100->Fill(pedcor_M9_T1_S.at(i));
            hist_M9_T2_S_100->Fill(pedcor_M9_T2_S.at(i));
            hist_M9_T3_S_100->Fill(pedcor_M9_T3_S.at(i));
            hist_M9_T4_S_100->Fill(pedcor_M9_T4_S.at(i));
	}                                            
        for (int i=101;i<201;i++){ 
            hist_M1_T1_C_200->Fill(pedcor_M1_T1_C.at(i));
            hist_M1_T2_C_200->Fill(pedcor_M1_T2_C.at(i));
            hist_M1_T3_C_200->Fill(pedcor_M1_T3_C.at(i));
            hist_M1_T4_C_200->Fill(pedcor_M1_T4_C.at(i));
            
            hist_M2_T1_C_200->Fill(pedcor_M2_T1_C.at(i));
            hist_M2_T2_C_200->Fill(pedcor_M2_T2_C.at(i));
            hist_M2_T3_C_200->Fill(pedcor_M2_T3_C.at(i));
            hist_M2_T4_C_200->Fill(pedcor_M2_T4_C.at(i));
            
            hist_M3_T1_C_200->Fill(pedcor_M3_T1_C.at(i));
            hist_M3_T2_C_200->Fill(pedcor_M3_T2_C.at(i));
            hist_M3_T3_C_200->Fill(pedcor_M3_T3_C.at(i));
            hist_M3_T4_C_200->Fill(pedcor_M3_T4_C.at(i));
            
            hist_M4_T1_C_200->Fill(pedcor_M4_T1_C.at(i));
            hist_M4_T2_C_200->Fill(pedcor_M4_T2_C.at(i));
            hist_M4_T3_C_200->Fill(pedcor_M4_T3_C.at(i));
            hist_M4_T4_C_200->Fill(pedcor_M4_T4_C.at(i));
            
            hist_M5_T1_C_200->Fill(pedcor_M5_T1_C.at(i));
            hist_M5_T2_C_200->Fill(pedcor_M5_T2_C.at(i));
            hist_M5_T3_C_200->Fill(pedcor_M5_T3_C.at(i));
            hist_M5_T4_C_200->Fill(pedcor_M5_T4_C.at(i));
            
            hist_M6_T1_C_200->Fill(pedcor_M6_T1_C.at(i));
            hist_M6_T2_C_200->Fill(pedcor_M6_T2_C.at(i));
            hist_M6_T3_C_200->Fill(pedcor_M6_T3_C.at(i));
            hist_M6_T4_C_200->Fill(pedcor_M6_T4_C.at(i));
            
            hist_M7_T1_C_200->Fill(pedcor_M7_T1_C.at(i));
            hist_M7_T2_C_200->Fill(pedcor_M7_T2_C.at(i));
            hist_M7_T3_C_200->Fill(pedcor_M7_T3_C.at(i));
            hist_M7_T4_C_200->Fill(pedcor_M7_T4_C.at(i));
            
            hist_M8_T1_C_200->Fill(pedcor_M8_T1_C.at(i));
            hist_M8_T2_C_200->Fill(pedcor_M8_T2_C.at(i));
            hist_M8_T3_C_200->Fill(pedcor_M8_T3_C.at(i));
            hist_M8_T4_C_200->Fill(pedcor_M8_T4_C.at(i));

            hist_M9_T1_C_200->Fill(pedcor_M9_T1_C.at(i));
            hist_M9_T2_C_200->Fill(pedcor_M9_T2_C.at(i));
            hist_M9_T3_C_200->Fill(pedcor_M9_T3_C.at(i));
            hist_M9_T4_C_200->Fill(pedcor_M9_T4_C.at(i));

            hist_M1_T1_S_200->Fill(pedcor_M1_T1_S.at(i));
            hist_M1_T2_S_200->Fill(pedcor_M1_T2_S.at(i));
            hist_M1_T3_S_200->Fill(pedcor_M1_T3_S.at(i));
            hist_M1_T4_S_200->Fill(pedcor_M1_T4_S.at(i));

            hist_M2_T1_S_200->Fill(pedcor_M2_T1_S.at(i));
            hist_M2_T2_S_200->Fill(pedcor_M2_T2_S.at(i));
            hist_M2_T3_S_200->Fill(pedcor_M2_T3_S.at(i));
            hist_M2_T4_S_200->Fill(pedcor_M2_T4_S.at(i));

            hist_M3_T1_S_200->Fill(pedcor_M3_T1_S.at(i));
            hist_M3_T2_S_200->Fill(pedcor_M3_T2_S.at(i));
            hist_M3_T3_S_200->Fill(pedcor_M3_T3_S.at(i));
            hist_M3_T4_S_200->Fill(pedcor_M3_T4_S.at(i));

            hist_M4_T1_S_200->Fill(pedcor_M4_T1_S.at(i));
            hist_M4_T2_S_200->Fill(pedcor_M4_T2_S.at(i));
            hist_M4_T3_S_200->Fill(pedcor_M4_T3_S.at(i));
            hist_M4_T4_S_200->Fill(pedcor_M4_T4_S.at(i));

            hist_M5_T1_S_200->Fill(pedcor_M5_T1_S.at(i));
            hist_M5_T2_S_200->Fill(pedcor_M5_T2_S.at(i));
            hist_M5_T3_S_200->Fill(pedcor_M5_T3_S.at(i));
            hist_M5_T4_S_200->Fill(pedcor_M5_T4_S.at(i));

            hist_M6_T1_S_200->Fill(pedcor_M6_T1_S.at(i));
            hist_M6_T2_S_200->Fill(pedcor_M6_T2_S.at(i));
            hist_M6_T3_S_200->Fill(pedcor_M6_T3_S.at(i));
            hist_M6_T4_S_200->Fill(pedcor_M6_T4_S.at(i));

            hist_M7_T1_S_200->Fill(pedcor_M7_T1_S.at(i));
            hist_M7_T2_S_200->Fill(pedcor_M7_T2_S.at(i));
            hist_M7_T3_S_200->Fill(pedcor_M7_T3_S.at(i));
            hist_M7_T4_S_200->Fill(pedcor_M7_T4_S.at(i));

            hist_M8_T1_S_200->Fill(pedcor_M8_T1_S.at(i));
            hist_M8_T2_S_200->Fill(pedcor_M8_T2_S.at(i));
            hist_M8_T3_S_200->Fill(pedcor_M8_T3_S.at(i));
            hist_M8_T4_S_200->Fill(pedcor_M8_T4_S.at(i));

            hist_M9_T1_S_200->Fill(pedcor_M9_T1_S.at(i));
            hist_M9_T2_S_200->Fill(pedcor_M9_T2_S.at(i));
            hist_M9_T3_S_200->Fill(pedcor_M9_T3_S.at(i));
            hist_M9_T4_S_200->Fill(pedcor_M9_T4_S.at(i));
	}                                            
        for (int i=201;i<301;i++){ 
            hist_M1_T1_C_300->Fill(pedcor_M1_T1_C.at(i));
            hist_M1_T2_C_300->Fill(pedcor_M1_T2_C.at(i));
            hist_M1_T3_C_300->Fill(pedcor_M1_T3_C.at(i));
            hist_M1_T4_C_300->Fill(pedcor_M1_T4_C.at(i));
            
            hist_M2_T1_C_300->Fill(pedcor_M2_T1_C.at(i));
            hist_M2_T2_C_300->Fill(pedcor_M2_T2_C.at(i));
            hist_M2_T3_C_300->Fill(pedcor_M2_T3_C.at(i));
            hist_M2_T4_C_300->Fill(pedcor_M2_T4_C.at(i));
            
            hist_M3_T1_C_300->Fill(pedcor_M3_T1_C.at(i));
            hist_M3_T2_C_300->Fill(pedcor_M3_T2_C.at(i));
            hist_M3_T3_C_300->Fill(pedcor_M3_T3_C.at(i));
            hist_M3_T4_C_300->Fill(pedcor_M3_T4_C.at(i));
            
            hist_M4_T1_C_300->Fill(pedcor_M4_T1_C.at(i));
            hist_M4_T2_C_300->Fill(pedcor_M4_T2_C.at(i));
            hist_M4_T3_C_300->Fill(pedcor_M4_T3_C.at(i));
            hist_M4_T4_C_300->Fill(pedcor_M4_T4_C.at(i));
            
            hist_M5_T1_C_300->Fill(pedcor_M5_T1_C.at(i));
            hist_M5_T2_C_300->Fill(pedcor_M5_T2_C.at(i));
            hist_M5_T3_C_300->Fill(pedcor_M5_T3_C.at(i));
            hist_M5_T4_C_300->Fill(pedcor_M5_T4_C.at(i));
            
            hist_M6_T1_C_300->Fill(pedcor_M6_T1_C.at(i));
            hist_M6_T2_C_300->Fill(pedcor_M6_T2_C.at(i));
            hist_M6_T3_C_300->Fill(pedcor_M6_T3_C.at(i));
            hist_M6_T4_C_300->Fill(pedcor_M6_T4_C.at(i));
            
            hist_M7_T1_C_300->Fill(pedcor_M7_T1_C.at(i));
            hist_M7_T2_C_300->Fill(pedcor_M7_T2_C.at(i));
            hist_M7_T3_C_300->Fill(pedcor_M7_T3_C.at(i));
            hist_M7_T4_C_300->Fill(pedcor_M7_T4_C.at(i));
            
            hist_M8_T1_C_300->Fill(pedcor_M8_T1_C.at(i));
            hist_M8_T2_C_300->Fill(pedcor_M8_T2_C.at(i));
            hist_M8_T3_C_300->Fill(pedcor_M8_T3_C.at(i));
            hist_M8_T4_C_300->Fill(pedcor_M8_T4_C.at(i));

            hist_M9_T1_C_300->Fill(pedcor_M9_T1_C.at(i));
            hist_M9_T2_C_300->Fill(pedcor_M9_T2_C.at(i));
            hist_M9_T3_C_300->Fill(pedcor_M9_T3_C.at(i));
            hist_M9_T4_C_300->Fill(pedcor_M9_T4_C.at(i));

            hist_M1_T1_S_300->Fill(pedcor_M1_T1_S.at(i));
            hist_M1_T2_S_300->Fill(pedcor_M1_T2_S.at(i));
            hist_M1_T3_S_300->Fill(pedcor_M1_T3_S.at(i));
            hist_M1_T4_S_300->Fill(pedcor_M1_T4_S.at(i));

            hist_M2_T1_S_300->Fill(pedcor_M2_T1_S.at(i));
            hist_M2_T2_S_300->Fill(pedcor_M2_T2_S.at(i));
            hist_M2_T3_S_300->Fill(pedcor_M2_T3_S.at(i));
            hist_M2_T4_S_300->Fill(pedcor_M2_T4_S.at(i));

            hist_M3_T1_S_300->Fill(pedcor_M3_T1_S.at(i));
            hist_M3_T2_S_300->Fill(pedcor_M3_T2_S.at(i));
            hist_M3_T3_S_300->Fill(pedcor_M3_T3_S.at(i));
            hist_M3_T4_S_300->Fill(pedcor_M3_T4_S.at(i));

            hist_M4_T1_S_300->Fill(pedcor_M4_T1_S.at(i));
            hist_M4_T2_S_300->Fill(pedcor_M4_T2_S.at(i));
            hist_M4_T3_S_300->Fill(pedcor_M4_T3_S.at(i));
            hist_M4_T4_S_300->Fill(pedcor_M4_T4_S.at(i));

            hist_M5_T1_S_300->Fill(pedcor_M5_T1_S.at(i));
            hist_M5_T2_S_300->Fill(pedcor_M5_T2_S.at(i));
            hist_M5_T3_S_300->Fill(pedcor_M5_T3_S.at(i));
            hist_M5_T4_S_300->Fill(pedcor_M5_T4_S.at(i));

            hist_M6_T1_S_300->Fill(pedcor_M6_T1_S.at(i));
            hist_M6_T2_S_300->Fill(pedcor_M6_T2_S.at(i));
            hist_M6_T3_S_300->Fill(pedcor_M6_T3_S.at(i));
            hist_M6_T4_S_300->Fill(pedcor_M6_T4_S.at(i));

            hist_M7_T1_S_300->Fill(pedcor_M7_T1_S.at(i));
            hist_M7_T2_S_300->Fill(pedcor_M7_T2_S.at(i));
            hist_M7_T3_S_300->Fill(pedcor_M7_T3_S.at(i));
            hist_M7_T4_S_300->Fill(pedcor_M7_T4_S.at(i));

            hist_M8_T1_S_300->Fill(pedcor_M8_T1_S.at(i));
            hist_M8_T2_S_300->Fill(pedcor_M8_T2_S.at(i));
            hist_M8_T3_S_300->Fill(pedcor_M8_T3_S.at(i));
            hist_M8_T4_S_300->Fill(pedcor_M8_T4_S.at(i));

            hist_M9_T1_S_300->Fill(pedcor_M9_T1_S.at(i));
            hist_M9_T2_S_300->Fill(pedcor_M9_T2_S.at(i));
            hist_M9_T3_S_300->Fill(pedcor_M9_T3_S.at(i));
            hist_M9_T4_S_300->Fill(pedcor_M9_T4_S.at(i));
	}                                            
        for (int i=301;i<401;i++){ 
            hist_M1_T1_C_400->Fill(pedcor_M1_T1_C.at(i));
            hist_M1_T2_C_400->Fill(pedcor_M1_T2_C.at(i));
            hist_M1_T3_C_400->Fill(pedcor_M1_T3_C.at(i));
            hist_M1_T4_C_400->Fill(pedcor_M1_T4_C.at(i));
            
            hist_M2_T1_C_400->Fill(pedcor_M2_T1_C.at(i));
            hist_M2_T2_C_400->Fill(pedcor_M2_T2_C.at(i));
            hist_M2_T3_C_400->Fill(pedcor_M2_T3_C.at(i));
            hist_M2_T4_C_400->Fill(pedcor_M2_T4_C.at(i));
            
            hist_M3_T1_C_400->Fill(pedcor_M3_T1_C.at(i));
            hist_M3_T2_C_400->Fill(pedcor_M3_T2_C.at(i));
            hist_M3_T3_C_400->Fill(pedcor_M3_T3_C.at(i));
            hist_M3_T4_C_400->Fill(pedcor_M3_T4_C.at(i));
            
            hist_M4_T1_C_400->Fill(pedcor_M4_T1_C.at(i));
            hist_M4_T2_C_400->Fill(pedcor_M4_T2_C.at(i));
            hist_M4_T3_C_400->Fill(pedcor_M4_T3_C.at(i));
            hist_M4_T4_C_400->Fill(pedcor_M4_T4_C.at(i));
            
            hist_M5_T1_C_400->Fill(pedcor_M5_T1_C.at(i));
            hist_M5_T2_C_400->Fill(pedcor_M5_T2_C.at(i));
            hist_M5_T3_C_400->Fill(pedcor_M5_T3_C.at(i));
            hist_M5_T4_C_400->Fill(pedcor_M5_T4_C.at(i));
            
            hist_M6_T1_C_400->Fill(pedcor_M6_T1_C.at(i));
            hist_M6_T2_C_400->Fill(pedcor_M6_T2_C.at(i));
            hist_M6_T3_C_400->Fill(pedcor_M6_T3_C.at(i));
            hist_M6_T4_C_400->Fill(pedcor_M6_T4_C.at(i));
            
            hist_M7_T1_C_400->Fill(pedcor_M7_T1_C.at(i));
            hist_M7_T2_C_400->Fill(pedcor_M7_T2_C.at(i));
            hist_M7_T3_C_400->Fill(pedcor_M7_T3_C.at(i));
            hist_M7_T4_C_400->Fill(pedcor_M7_T4_C.at(i));
            
            hist_M8_T1_C_400->Fill(pedcor_M8_T1_C.at(i));
            hist_M8_T2_C_400->Fill(pedcor_M8_T2_C.at(i));
            hist_M8_T3_C_400->Fill(pedcor_M8_T3_C.at(i));
            hist_M8_T4_C_400->Fill(pedcor_M8_T4_C.at(i));

            hist_M9_T1_C_400->Fill(pedcor_M9_T1_C.at(i));
            hist_M9_T2_C_400->Fill(pedcor_M9_T2_C.at(i));
            hist_M9_T3_C_400->Fill(pedcor_M9_T3_C.at(i));
            hist_M9_T4_C_400->Fill(pedcor_M9_T4_C.at(i));

            hist_M1_T1_S_400->Fill(pedcor_M1_T1_S.at(i));
            hist_M1_T2_S_400->Fill(pedcor_M1_T2_S.at(i));
            hist_M1_T3_S_400->Fill(pedcor_M1_T3_S.at(i));
            hist_M1_T4_S_400->Fill(pedcor_M1_T4_S.at(i));

            hist_M2_T1_S_400->Fill(pedcor_M2_T1_S.at(i));
            hist_M2_T2_S_400->Fill(pedcor_M2_T2_S.at(i));
            hist_M2_T3_S_400->Fill(pedcor_M2_T3_S.at(i));
            hist_M2_T4_S_400->Fill(pedcor_M2_T4_S.at(i));

            hist_M3_T1_S_400->Fill(pedcor_M3_T1_S.at(i));
            hist_M3_T2_S_400->Fill(pedcor_M3_T2_S.at(i));
            hist_M3_T3_S_400->Fill(pedcor_M3_T3_S.at(i));
            hist_M3_T4_S_400->Fill(pedcor_M3_T4_S.at(i));

            hist_M4_T1_S_400->Fill(pedcor_M4_T1_S.at(i));
            hist_M4_T2_S_400->Fill(pedcor_M4_T2_S.at(i));
            hist_M4_T3_S_400->Fill(pedcor_M4_T3_S.at(i));
            hist_M4_T4_S_400->Fill(pedcor_M4_T4_S.at(i));

            hist_M5_T1_S_400->Fill(pedcor_M5_T1_S.at(i));
            hist_M5_T2_S_400->Fill(pedcor_M5_T2_S.at(i));
            hist_M5_T3_S_400->Fill(pedcor_M5_T3_S.at(i));
            hist_M5_T4_S_400->Fill(pedcor_M5_T4_S.at(i));

            hist_M6_T1_S_400->Fill(pedcor_M6_T1_S.at(i));
            hist_M6_T2_S_400->Fill(pedcor_M6_T2_S.at(i));
            hist_M6_T3_S_400->Fill(pedcor_M6_T3_S.at(i));
            hist_M6_T4_S_400->Fill(pedcor_M6_T4_S.at(i));

            hist_M7_T1_S_400->Fill(pedcor_M7_T1_S.at(i));
            hist_M7_T2_S_400->Fill(pedcor_M7_T2_S.at(i));
            hist_M7_T3_S_400->Fill(pedcor_M7_T3_S.at(i));
            hist_M7_T4_S_400->Fill(pedcor_M7_T4_S.at(i));

            hist_M8_T1_S_400->Fill(pedcor_M8_T1_S.at(i));
            hist_M8_T2_S_400->Fill(pedcor_M8_T2_S.at(i));
            hist_M8_T3_S_400->Fill(pedcor_M8_T3_S.at(i));
            hist_M8_T4_S_400->Fill(pedcor_M8_T4_S.at(i));

            hist_M9_T1_S_400->Fill(pedcor_M9_T1_S.at(i));
            hist_M9_T2_S_400->Fill(pedcor_M9_T2_S.at(i));
            hist_M9_T3_S_400->Fill(pedcor_M9_T3_S.at(i));
            hist_M9_T4_S_400->Fill(pedcor_M9_T4_S.at(i));
	}                                            
        for (int i=401;i<501;i++){ 
            hist_M1_T1_C_500->Fill(pedcor_M1_T1_C.at(i));
            hist_M1_T2_C_500->Fill(pedcor_M1_T2_C.at(i));
            hist_M1_T3_C_500->Fill(pedcor_M1_T3_C.at(i));
            hist_M1_T4_C_500->Fill(pedcor_M1_T4_C.at(i));
            
            hist_M2_T1_C_500->Fill(pedcor_M2_T1_C.at(i));
            hist_M2_T2_C_500->Fill(pedcor_M2_T2_C.at(i));
            hist_M2_T3_C_500->Fill(pedcor_M2_T3_C.at(i));
            hist_M2_T4_C_500->Fill(pedcor_M2_T4_C.at(i));
            
            hist_M3_T1_C_500->Fill(pedcor_M3_T1_C.at(i));
            hist_M3_T2_C_500->Fill(pedcor_M3_T2_C.at(i));
            hist_M3_T3_C_500->Fill(pedcor_M3_T3_C.at(i));
            hist_M3_T4_C_500->Fill(pedcor_M3_T4_C.at(i));
            
            hist_M4_T1_C_500->Fill(pedcor_M4_T1_C.at(i));
            hist_M4_T2_C_500->Fill(pedcor_M4_T2_C.at(i));
            hist_M4_T3_C_500->Fill(pedcor_M4_T3_C.at(i));
            hist_M4_T4_C_500->Fill(pedcor_M4_T4_C.at(i));
            
            hist_M5_T1_C_500->Fill(pedcor_M5_T1_C.at(i));
            hist_M5_T2_C_500->Fill(pedcor_M5_T2_C.at(i));
            hist_M5_T3_C_500->Fill(pedcor_M5_T3_C.at(i));
            hist_M5_T4_C_500->Fill(pedcor_M5_T4_C.at(i));
            
            hist_M6_T1_C_500->Fill(pedcor_M6_T1_C.at(i));
            hist_M6_T2_C_500->Fill(pedcor_M6_T2_C.at(i));
            hist_M6_T3_C_500->Fill(pedcor_M6_T3_C.at(i));
            hist_M6_T4_C_500->Fill(pedcor_M6_T4_C.at(i));
            
            hist_M7_T1_C_500->Fill(pedcor_M7_T1_C.at(i));
            hist_M7_T2_C_500->Fill(pedcor_M7_T2_C.at(i));
            hist_M7_T3_C_500->Fill(pedcor_M7_T3_C.at(i));
            hist_M7_T4_C_500->Fill(pedcor_M7_T4_C.at(i));
            
            hist_M8_T1_C_500->Fill(pedcor_M8_T1_C.at(i));
            hist_M8_T2_C_500->Fill(pedcor_M8_T2_C.at(i));
            hist_M8_T3_C_500->Fill(pedcor_M8_T3_C.at(i));
            hist_M8_T4_C_500->Fill(pedcor_M8_T4_C.at(i));

            hist_M9_T1_C_500->Fill(pedcor_M9_T1_C.at(i));
            hist_M9_T2_C_500->Fill(pedcor_M9_T2_C.at(i));
            hist_M9_T3_C_500->Fill(pedcor_M9_T3_C.at(i));
            hist_M9_T4_C_500->Fill(pedcor_M9_T4_C.at(i));

            hist_M1_T1_S_500->Fill(pedcor_M1_T1_S.at(i));
            hist_M1_T2_S_500->Fill(pedcor_M1_T2_S.at(i));
            hist_M1_T3_S_500->Fill(pedcor_M1_T3_S.at(i));
            hist_M1_T4_S_500->Fill(pedcor_M1_T4_S.at(i));

            hist_M2_T1_S_500->Fill(pedcor_M2_T1_S.at(i));
            hist_M2_T2_S_500->Fill(pedcor_M2_T2_S.at(i));
            hist_M2_T3_S_500->Fill(pedcor_M2_T3_S.at(i));
            hist_M2_T4_S_500->Fill(pedcor_M2_T4_S.at(i));

            hist_M3_T1_S_500->Fill(pedcor_M3_T1_S.at(i));
            hist_M3_T2_S_500->Fill(pedcor_M3_T2_S.at(i));
            hist_M3_T3_S_500->Fill(pedcor_M3_T3_S.at(i));
            hist_M3_T4_S_500->Fill(pedcor_M3_T4_S.at(i));

            hist_M4_T1_S_500->Fill(pedcor_M4_T1_S.at(i));
            hist_M4_T2_S_500->Fill(pedcor_M4_T2_S.at(i));
            hist_M4_T3_S_500->Fill(pedcor_M4_T3_S.at(i));
            hist_M4_T4_S_500->Fill(pedcor_M4_T4_S.at(i));

            hist_M5_T1_S_500->Fill(pedcor_M5_T1_S.at(i));
            hist_M5_T2_S_500->Fill(pedcor_M5_T2_S.at(i));
            hist_M5_T3_S_500->Fill(pedcor_M5_T3_S.at(i));
            hist_M5_T4_S_500->Fill(pedcor_M5_T4_S.at(i));

            hist_M6_T1_S_500->Fill(pedcor_M6_T1_S.at(i));
            hist_M6_T2_S_500->Fill(pedcor_M6_T2_S.at(i));
            hist_M6_T3_S_500->Fill(pedcor_M6_T3_S.at(i));
            hist_M6_T4_S_500->Fill(pedcor_M6_T4_S.at(i));

            hist_M7_T1_S_500->Fill(pedcor_M7_T1_S.at(i));
            hist_M7_T2_S_500->Fill(pedcor_M7_T2_S.at(i));
            hist_M7_T3_S_500->Fill(pedcor_M7_T3_S.at(i));
            hist_M7_T4_S_500->Fill(pedcor_M7_T4_S.at(i));

            hist_M8_T1_S_500->Fill(pedcor_M8_T1_S.at(i));
            hist_M8_T2_S_500->Fill(pedcor_M8_T2_S.at(i));
            hist_M8_T3_S_500->Fill(pedcor_M8_T3_S.at(i));
            hist_M8_T4_S_500->Fill(pedcor_M8_T4_S.at(i));

            hist_M9_T1_S_500->Fill(pedcor_M9_T1_S.at(i));
            hist_M9_T2_S_500->Fill(pedcor_M9_T2_S.at(i));
            hist_M9_T3_S_500->Fill(pedcor_M9_T3_S.at(i));
            hist_M9_T4_S_500->Fill(pedcor_M9_T4_S.at(i));
	}                                            
        for (int i=501;i<601;i++){ 
            hist_M1_T1_C_600->Fill(pedcor_M1_T1_C.at(i));
            hist_M1_T2_C_600->Fill(pedcor_M1_T2_C.at(i));
            hist_M1_T3_C_600->Fill(pedcor_M1_T3_C.at(i));
            hist_M1_T4_C_600->Fill(pedcor_M1_T4_C.at(i));
            
            hist_M2_T1_C_600->Fill(pedcor_M2_T1_C.at(i));
            hist_M2_T2_C_600->Fill(pedcor_M2_T2_C.at(i));
            hist_M2_T3_C_600->Fill(pedcor_M2_T3_C.at(i));
            hist_M2_T4_C_600->Fill(pedcor_M2_T4_C.at(i));
            
            hist_M3_T1_C_600->Fill(pedcor_M3_T1_C.at(i));
            hist_M3_T2_C_600->Fill(pedcor_M3_T2_C.at(i));
            hist_M3_T3_C_600->Fill(pedcor_M3_T3_C.at(i));
            hist_M3_T4_C_600->Fill(pedcor_M3_T4_C.at(i));
            
            hist_M4_T1_C_600->Fill(pedcor_M4_T1_C.at(i));
            hist_M4_T2_C_600->Fill(pedcor_M4_T2_C.at(i));
            hist_M4_T3_C_600->Fill(pedcor_M4_T3_C.at(i));
            hist_M4_T4_C_600->Fill(pedcor_M4_T4_C.at(i));
            
            hist_M5_T1_C_600->Fill(pedcor_M5_T1_C.at(i));
            hist_M5_T2_C_600->Fill(pedcor_M5_T2_C.at(i));
            hist_M5_T3_C_600->Fill(pedcor_M5_T3_C.at(i));
            hist_M5_T4_C_600->Fill(pedcor_M5_T4_C.at(i));
            
            hist_M6_T1_C_600->Fill(pedcor_M6_T1_C.at(i));
            hist_M6_T2_C_600->Fill(pedcor_M6_T2_C.at(i));
            hist_M6_T3_C_600->Fill(pedcor_M6_T3_C.at(i));
            hist_M6_T4_C_600->Fill(pedcor_M6_T4_C.at(i));
            
            hist_M7_T1_C_600->Fill(pedcor_M7_T1_C.at(i));
            hist_M7_T2_C_600->Fill(pedcor_M7_T2_C.at(i));
            hist_M7_T3_C_600->Fill(pedcor_M7_T3_C.at(i));
            hist_M7_T4_C_600->Fill(pedcor_M7_T4_C.at(i));
            
            hist_M8_T1_C_600->Fill(pedcor_M8_T1_C.at(i));
            hist_M8_T2_C_600->Fill(pedcor_M8_T2_C.at(i));
            hist_M8_T3_C_600->Fill(pedcor_M8_T3_C.at(i));
            hist_M8_T4_C_600->Fill(pedcor_M8_T4_C.at(i));

            hist_M9_T1_C_600->Fill(pedcor_M9_T1_C.at(i));
            hist_M9_T2_C_600->Fill(pedcor_M9_T2_C.at(i));
            hist_M9_T3_C_600->Fill(pedcor_M9_T3_C.at(i));
            hist_M9_T4_C_600->Fill(pedcor_M9_T4_C.at(i));

            hist_M1_T1_S_600->Fill(pedcor_M1_T1_S.at(i));
            hist_M1_T2_S_600->Fill(pedcor_M1_T2_S.at(i));
            hist_M1_T3_S_600->Fill(pedcor_M1_T3_S.at(i));
            hist_M1_T4_S_600->Fill(pedcor_M1_T4_S.at(i));

            hist_M2_T1_S_600->Fill(pedcor_M2_T1_S.at(i));
            hist_M2_T2_S_600->Fill(pedcor_M2_T2_S.at(i));
            hist_M2_T3_S_600->Fill(pedcor_M2_T3_S.at(i));
            hist_M2_T4_S_600->Fill(pedcor_M2_T4_S.at(i));

            hist_M3_T1_S_600->Fill(pedcor_M3_T1_S.at(i));
            hist_M3_T2_S_600->Fill(pedcor_M3_T2_S.at(i));
            hist_M3_T3_S_600->Fill(pedcor_M3_T3_S.at(i));
            hist_M3_T4_S_600->Fill(pedcor_M3_T4_S.at(i));

            hist_M4_T1_S_600->Fill(pedcor_M4_T1_S.at(i));
            hist_M4_T2_S_600->Fill(pedcor_M4_T2_S.at(i));
            hist_M4_T3_S_600->Fill(pedcor_M4_T3_S.at(i));
            hist_M4_T4_S_600->Fill(pedcor_M4_T4_S.at(i));

            hist_M5_T1_S_600->Fill(pedcor_M5_T1_S.at(i));
            hist_M5_T2_S_600->Fill(pedcor_M5_T2_S.at(i));
            hist_M5_T3_S_600->Fill(pedcor_M5_T3_S.at(i));
            hist_M5_T4_S_600->Fill(pedcor_M5_T4_S.at(i));

            hist_M6_T1_S_600->Fill(pedcor_M6_T1_S.at(i));
            hist_M6_T2_S_600->Fill(pedcor_M6_T2_S.at(i));
            hist_M6_T3_S_600->Fill(pedcor_M6_T3_S.at(i));
            hist_M6_T4_S_600->Fill(pedcor_M6_T4_S.at(i));

            hist_M7_T1_S_600->Fill(pedcor_M7_T1_S.at(i));
            hist_M7_T2_S_600->Fill(pedcor_M7_T2_S.at(i));
            hist_M7_T3_S_600->Fill(pedcor_M7_T3_S.at(i));
            hist_M7_T4_S_600->Fill(pedcor_M7_T4_S.at(i));

            hist_M8_T1_S_600->Fill(pedcor_M8_T1_S.at(i));
            hist_M8_T2_S_600->Fill(pedcor_M8_T2_S.at(i));
            hist_M8_T3_S_600->Fill(pedcor_M8_T3_S.at(i));
            hist_M8_T4_S_600->Fill(pedcor_M8_T4_S.at(i));

            hist_M9_T1_S_600->Fill(pedcor_M9_T1_S.at(i));
            hist_M9_T2_S_600->Fill(pedcor_M9_T2_S.at(i));
            hist_M9_T3_S_600->Fill(pedcor_M9_T3_S.at(i));
            hist_M9_T4_S_600->Fill(pedcor_M9_T4_S.at(i));
	}                                            
        for (int i=601;i<701;i++){ 
            hist_M1_T1_C_700->Fill(pedcor_M1_T1_C.at(i));
            hist_M1_T2_C_700->Fill(pedcor_M1_T2_C.at(i));
            hist_M1_T3_C_700->Fill(pedcor_M1_T3_C.at(i));
            hist_M1_T4_C_700->Fill(pedcor_M1_T4_C.at(i));
            
            hist_M2_T1_C_700->Fill(pedcor_M2_T1_C.at(i));
            hist_M2_T2_C_700->Fill(pedcor_M2_T2_C.at(i));
            hist_M2_T3_C_700->Fill(pedcor_M2_T3_C.at(i));
            hist_M2_T4_C_700->Fill(pedcor_M2_T4_C.at(i));
            
            hist_M3_T1_C_700->Fill(pedcor_M3_T1_C.at(i));
            hist_M3_T2_C_700->Fill(pedcor_M3_T2_C.at(i));
            hist_M3_T3_C_700->Fill(pedcor_M3_T3_C.at(i));
            hist_M3_T4_C_700->Fill(pedcor_M3_T4_C.at(i));
            
            hist_M4_T1_C_700->Fill(pedcor_M4_T1_C.at(i));
            hist_M4_T2_C_700->Fill(pedcor_M4_T2_C.at(i));
            hist_M4_T3_C_700->Fill(pedcor_M4_T3_C.at(i));
            hist_M4_T4_C_700->Fill(pedcor_M4_T4_C.at(i));
            
            hist_M5_T1_C_700->Fill(pedcor_M5_T1_C.at(i));
            hist_M5_T2_C_700->Fill(pedcor_M5_T2_C.at(i));
            hist_M5_T3_C_700->Fill(pedcor_M5_T3_C.at(i));
            hist_M5_T4_C_700->Fill(pedcor_M5_T4_C.at(i));
            
            hist_M6_T1_C_700->Fill(pedcor_M6_T1_C.at(i));
            hist_M6_T2_C_700->Fill(pedcor_M6_T2_C.at(i));
            hist_M6_T3_C_700->Fill(pedcor_M6_T3_C.at(i));
            hist_M6_T4_C_700->Fill(pedcor_M6_T4_C.at(i));
            
            hist_M7_T1_C_700->Fill(pedcor_M7_T1_C.at(i));
            hist_M7_T2_C_700->Fill(pedcor_M7_T2_C.at(i));
            hist_M7_T3_C_700->Fill(pedcor_M7_T3_C.at(i));
            hist_M7_T4_C_700->Fill(pedcor_M7_T4_C.at(i));
            
            hist_M8_T1_C_700->Fill(pedcor_M8_T1_C.at(i));
            hist_M8_T2_C_700->Fill(pedcor_M8_T2_C.at(i));
            hist_M8_T3_C_700->Fill(pedcor_M8_T3_C.at(i));
            hist_M8_T4_C_700->Fill(pedcor_M8_T4_C.at(i));

            hist_M9_T1_C_700->Fill(pedcor_M9_T1_C.at(i));
            hist_M9_T2_C_700->Fill(pedcor_M9_T2_C.at(i));
            hist_M9_T3_C_700->Fill(pedcor_M9_T3_C.at(i));
            hist_M9_T4_C_700->Fill(pedcor_M9_T4_C.at(i));

            hist_M1_T1_S_700->Fill(pedcor_M1_T1_S.at(i));
            hist_M1_T2_S_700->Fill(pedcor_M1_T2_S.at(i));
            hist_M1_T3_S_700->Fill(pedcor_M1_T3_S.at(i));
            hist_M1_T4_S_700->Fill(pedcor_M1_T4_S.at(i));

            hist_M2_T1_S_700->Fill(pedcor_M2_T1_S.at(i));
            hist_M2_T2_S_700->Fill(pedcor_M2_T2_S.at(i));
            hist_M2_T3_S_700->Fill(pedcor_M2_T3_S.at(i));
            hist_M2_T4_S_700->Fill(pedcor_M2_T4_S.at(i));

            hist_M3_T1_S_700->Fill(pedcor_M3_T1_S.at(i));
            hist_M3_T2_S_700->Fill(pedcor_M3_T2_S.at(i));
            hist_M3_T3_S_700->Fill(pedcor_M3_T3_S.at(i));
            hist_M3_T4_S_700->Fill(pedcor_M3_T4_S.at(i));

            hist_M4_T1_S_700->Fill(pedcor_M4_T1_S.at(i));
            hist_M4_T2_S_700->Fill(pedcor_M4_T2_S.at(i));
            hist_M4_T3_S_700->Fill(pedcor_M4_T3_S.at(i));
            hist_M4_T4_S_700->Fill(pedcor_M4_T4_S.at(i));

            hist_M5_T1_S_700->Fill(pedcor_M5_T1_S.at(i));
            hist_M5_T2_S_700->Fill(pedcor_M5_T2_S.at(i));
            hist_M5_T3_S_700->Fill(pedcor_M5_T3_S.at(i));
            hist_M5_T4_S_700->Fill(pedcor_M5_T4_S.at(i));

            hist_M6_T1_S_700->Fill(pedcor_M6_T1_S.at(i));
            hist_M6_T2_S_700->Fill(pedcor_M6_T2_S.at(i));
            hist_M6_T3_S_700->Fill(pedcor_M6_T3_S.at(i));
            hist_M6_T4_S_700->Fill(pedcor_M6_T4_S.at(i));

            hist_M7_T1_S_700->Fill(pedcor_M7_T1_S.at(i));
            hist_M7_T2_S_700->Fill(pedcor_M7_T2_S.at(i));
            hist_M7_T3_S_700->Fill(pedcor_M7_T3_S.at(i));
            hist_M7_T4_S_700->Fill(pedcor_M7_T4_S.at(i));

            hist_M8_T1_S_700->Fill(pedcor_M8_T1_S.at(i));
            hist_M8_T2_S_700->Fill(pedcor_M8_T2_S.at(i));
            hist_M8_T3_S_700->Fill(pedcor_M8_T3_S.at(i));
            hist_M8_T4_S_700->Fill(pedcor_M8_T4_S.at(i));

            hist_M9_T1_S_700->Fill(pedcor_M9_T1_S.at(i));
            hist_M9_T2_S_700->Fill(pedcor_M9_T2_S.at(i));
            hist_M9_T3_S_700->Fill(pedcor_M9_T3_S.at(i));
            hist_M9_T4_S_700->Fill(pedcor_M9_T4_S.at(i));
	}                                            
        for (int i=701;i<801;i++){ 
            hist_M1_T1_C_800->Fill(pedcor_M1_T1_C.at(i));
            hist_M1_T2_C_800->Fill(pedcor_M1_T2_C.at(i));
            hist_M1_T3_C_800->Fill(pedcor_M1_T3_C.at(i));
            hist_M1_T4_C_800->Fill(pedcor_M1_T4_C.at(i));
            
            hist_M2_T1_C_800->Fill(pedcor_M2_T1_C.at(i));
            hist_M2_T2_C_800->Fill(pedcor_M2_T2_C.at(i));
            hist_M2_T3_C_800->Fill(pedcor_M2_T3_C.at(i));
            hist_M2_T4_C_800->Fill(pedcor_M2_T4_C.at(i));
            
            hist_M3_T1_C_800->Fill(pedcor_M3_T1_C.at(i));
            hist_M3_T2_C_800->Fill(pedcor_M3_T2_C.at(i));
            hist_M3_T3_C_800->Fill(pedcor_M3_T3_C.at(i));
            hist_M3_T4_C_800->Fill(pedcor_M3_T4_C.at(i));
            
            hist_M4_T1_C_800->Fill(pedcor_M4_T1_C.at(i));
            hist_M4_T2_C_800->Fill(pedcor_M4_T2_C.at(i));
            hist_M4_T3_C_800->Fill(pedcor_M4_T3_C.at(i));
            hist_M4_T4_C_800->Fill(pedcor_M4_T4_C.at(i));
            
            hist_M5_T1_C_800->Fill(pedcor_M5_T1_C.at(i));
            hist_M5_T2_C_800->Fill(pedcor_M5_T2_C.at(i));
            hist_M5_T3_C_800->Fill(pedcor_M5_T3_C.at(i));
            hist_M5_T4_C_800->Fill(pedcor_M5_T4_C.at(i));
            
            hist_M6_T1_C_800->Fill(pedcor_M6_T1_C.at(i));
            hist_M6_T2_C_800->Fill(pedcor_M6_T2_C.at(i));
            hist_M6_T3_C_800->Fill(pedcor_M6_T3_C.at(i));
            hist_M6_T4_C_800->Fill(pedcor_M6_T4_C.at(i));
            
            hist_M7_T1_C_800->Fill(pedcor_M7_T1_C.at(i));
            hist_M7_T2_C_800->Fill(pedcor_M7_T2_C.at(i));
            hist_M7_T3_C_800->Fill(pedcor_M7_T3_C.at(i));
            hist_M7_T4_C_800->Fill(pedcor_M7_T4_C.at(i));
            
            hist_M8_T1_C_800->Fill(pedcor_M8_T1_C.at(i));
            hist_M8_T2_C_800->Fill(pedcor_M8_T2_C.at(i));
            hist_M8_T3_C_800->Fill(pedcor_M8_T3_C.at(i));
            hist_M8_T4_C_800->Fill(pedcor_M8_T4_C.at(i));

            hist_M9_T1_C_800->Fill(pedcor_M9_T1_C.at(i));
            hist_M9_T2_C_800->Fill(pedcor_M9_T2_C.at(i));
            hist_M9_T3_C_800->Fill(pedcor_M9_T3_C.at(i));
            hist_M9_T4_C_800->Fill(pedcor_M9_T4_C.at(i));

            hist_M1_T1_S_800->Fill(pedcor_M1_T1_S.at(i));
            hist_M1_T2_S_800->Fill(pedcor_M1_T2_S.at(i));
            hist_M1_T3_S_800->Fill(pedcor_M1_T3_S.at(i));
            hist_M1_T4_S_800->Fill(pedcor_M1_T4_S.at(i));

            hist_M2_T1_S_800->Fill(pedcor_M2_T1_S.at(i));
            hist_M2_T2_S_800->Fill(pedcor_M2_T2_S.at(i));
            hist_M2_T3_S_800->Fill(pedcor_M2_T3_S.at(i));
            hist_M2_T4_S_800->Fill(pedcor_M2_T4_S.at(i));

            hist_M3_T1_S_800->Fill(pedcor_M3_T1_S.at(i));
            hist_M3_T2_S_800->Fill(pedcor_M3_T2_S.at(i));
            hist_M3_T3_S_800->Fill(pedcor_M3_T3_S.at(i));
            hist_M3_T4_S_800->Fill(pedcor_M3_T4_S.at(i));

            hist_M4_T1_S_800->Fill(pedcor_M4_T1_S.at(i));
            hist_M4_T2_S_800->Fill(pedcor_M4_T2_S.at(i));
            hist_M4_T3_S_800->Fill(pedcor_M4_T3_S.at(i));
            hist_M4_T4_S_800->Fill(pedcor_M4_T4_S.at(i));

            hist_M5_T1_S_800->Fill(pedcor_M5_T1_S.at(i));
            hist_M5_T2_S_800->Fill(pedcor_M5_T2_S.at(i));
            hist_M5_T3_S_800->Fill(pedcor_M5_T3_S.at(i));
            hist_M5_T4_S_800->Fill(pedcor_M5_T4_S.at(i));

            hist_M6_T1_S_800->Fill(pedcor_M6_T1_S.at(i));
            hist_M6_T2_S_800->Fill(pedcor_M6_T2_S.at(i));
            hist_M6_T3_S_800->Fill(pedcor_M6_T3_S.at(i));
            hist_M6_T4_S_800->Fill(pedcor_M6_T4_S.at(i));

            hist_M7_T1_S_800->Fill(pedcor_M7_T1_S.at(i));
            hist_M7_T2_S_800->Fill(pedcor_M7_T2_S.at(i));
            hist_M7_T3_S_800->Fill(pedcor_M7_T3_S.at(i));
            hist_M7_T4_S_800->Fill(pedcor_M7_T4_S.at(i));

            hist_M8_T1_S_800->Fill(pedcor_M8_T1_S.at(i));
            hist_M8_T2_S_800->Fill(pedcor_M8_T2_S.at(i));
            hist_M8_T3_S_800->Fill(pedcor_M8_T3_S.at(i));
            hist_M8_T4_S_800->Fill(pedcor_M8_T4_S.at(i));

            hist_M9_T1_S_800->Fill(pedcor_M9_T1_S.at(i));
            hist_M9_T2_S_800->Fill(pedcor_M9_T2_S.at(i));
            hist_M9_T3_S_800->Fill(pedcor_M9_T3_S.at(i));
            hist_M9_T4_S_800->Fill(pedcor_M9_T4_S.at(i));
	}                                            
        for (int i=801;i<901;i++){ 
            hist_M1_T1_C_900->Fill(pedcor_M1_T1_C.at(i));
            hist_M1_T2_C_900->Fill(pedcor_M1_T2_C.at(i));
            hist_M1_T3_C_900->Fill(pedcor_M1_T3_C.at(i));
            hist_M1_T4_C_900->Fill(pedcor_M1_T4_C.at(i));
            
            hist_M2_T1_C_900->Fill(pedcor_M2_T1_C.at(i));
            hist_M2_T2_C_900->Fill(pedcor_M2_T2_C.at(i));
            hist_M2_T3_C_900->Fill(pedcor_M2_T3_C.at(i));
            hist_M2_T4_C_900->Fill(pedcor_M2_T4_C.at(i));
            
            hist_M3_T1_C_900->Fill(pedcor_M3_T1_C.at(i));
            hist_M3_T2_C_900->Fill(pedcor_M3_T2_C.at(i));
            hist_M3_T3_C_900->Fill(pedcor_M3_T3_C.at(i));
            hist_M3_T4_C_900->Fill(pedcor_M3_T4_C.at(i));
            
            hist_M4_T1_C_900->Fill(pedcor_M4_T1_C.at(i));
            hist_M4_T2_C_900->Fill(pedcor_M4_T2_C.at(i));
            hist_M4_T3_C_900->Fill(pedcor_M4_T3_C.at(i));
            hist_M4_T4_C_900->Fill(pedcor_M4_T4_C.at(i));
            
            hist_M5_T1_C_900->Fill(pedcor_M5_T1_C.at(i));
            hist_M5_T2_C_900->Fill(pedcor_M5_T2_C.at(i));
            hist_M5_T3_C_900->Fill(pedcor_M5_T3_C.at(i));
            hist_M5_T4_C_900->Fill(pedcor_M5_T4_C.at(i));
            
            hist_M6_T1_C_900->Fill(pedcor_M6_T1_C.at(i));
            hist_M6_T2_C_900->Fill(pedcor_M6_T2_C.at(i));
            hist_M6_T3_C_900->Fill(pedcor_M6_T3_C.at(i));
            hist_M6_T4_C_900->Fill(pedcor_M6_T4_C.at(i));
            
            hist_M7_T1_C_900->Fill(pedcor_M7_T1_C.at(i));
            hist_M7_T2_C_900->Fill(pedcor_M7_T2_C.at(i));
            hist_M7_T3_C_900->Fill(pedcor_M7_T3_C.at(i));
            hist_M7_T4_C_900->Fill(pedcor_M7_T4_C.at(i));
            
            hist_M8_T1_C_900->Fill(pedcor_M8_T1_C.at(i));
            hist_M8_T2_C_900->Fill(pedcor_M8_T2_C.at(i));
            hist_M8_T3_C_900->Fill(pedcor_M8_T3_C.at(i));
            hist_M8_T4_C_900->Fill(pedcor_M8_T4_C.at(i));

            hist_M9_T1_C_900->Fill(pedcor_M9_T1_C.at(i));
            hist_M9_T2_C_900->Fill(pedcor_M9_T2_C.at(i));
            hist_M9_T3_C_900->Fill(pedcor_M9_T3_C.at(i));
            hist_M9_T4_C_900->Fill(pedcor_M9_T4_C.at(i));

            hist_M1_T1_S_900->Fill(pedcor_M1_T1_S.at(i));
            hist_M1_T2_S_900->Fill(pedcor_M1_T2_S.at(i));
            hist_M1_T3_S_900->Fill(pedcor_M1_T3_S.at(i));
            hist_M1_T4_S_900->Fill(pedcor_M1_T4_S.at(i));

            hist_M2_T1_S_900->Fill(pedcor_M2_T1_S.at(i));
            hist_M2_T2_S_900->Fill(pedcor_M2_T2_S.at(i));
            hist_M2_T3_S_900->Fill(pedcor_M2_T3_S.at(i));
            hist_M2_T4_S_900->Fill(pedcor_M2_T4_S.at(i));

            hist_M3_T1_S_900->Fill(pedcor_M3_T1_S.at(i));
            hist_M3_T2_S_900->Fill(pedcor_M3_T2_S.at(i));
            hist_M3_T3_S_900->Fill(pedcor_M3_T3_S.at(i));
            hist_M3_T4_S_900->Fill(pedcor_M3_T4_S.at(i));

            hist_M4_T1_S_900->Fill(pedcor_M4_T1_S.at(i));
            hist_M4_T2_S_900->Fill(pedcor_M4_T2_S.at(i));
            hist_M4_T3_S_900->Fill(pedcor_M4_T3_S.at(i));
            hist_M4_T4_S_900->Fill(pedcor_M4_T4_S.at(i));

            hist_M5_T1_S_900->Fill(pedcor_M5_T1_S.at(i));
            hist_M5_T2_S_900->Fill(pedcor_M5_T2_S.at(i));
            hist_M5_T3_S_900->Fill(pedcor_M5_T3_S.at(i));
            hist_M5_T4_S_900->Fill(pedcor_M5_T4_S.at(i));

            hist_M6_T1_S_900->Fill(pedcor_M6_T1_S.at(i));
            hist_M6_T2_S_900->Fill(pedcor_M6_T2_S.at(i));
            hist_M6_T3_S_900->Fill(pedcor_M6_T3_S.at(i));
            hist_M6_T4_S_900->Fill(pedcor_M6_T4_S.at(i));

            hist_M7_T1_S_900->Fill(pedcor_M7_T1_S.at(i));
            hist_M7_T2_S_900->Fill(pedcor_M7_T2_S.at(i));
            hist_M7_T3_S_900->Fill(pedcor_M7_T3_S.at(i));
            hist_M7_T4_S_900->Fill(pedcor_M7_T4_S.at(i));

            hist_M8_T1_S_900->Fill(pedcor_M8_T1_S.at(i));
            hist_M8_T2_S_900->Fill(pedcor_M8_T2_S.at(i));
            hist_M8_T3_S_900->Fill(pedcor_M8_T3_S.at(i));
            hist_M8_T4_S_900->Fill(pedcor_M8_T4_S.at(i));

            hist_M9_T1_S_900->Fill(pedcor_M9_T1_S.at(i));
            hist_M9_T2_S_900->Fill(pedcor_M9_T2_S.at(i));
            hist_M9_T3_S_900->Fill(pedcor_M9_T3_S.at(i));
            hist_M9_T4_S_900->Fill(pedcor_M9_T4_S.at(i));
	}                                            
        for (int i=901;i<1001;i++){ 
            hist_M1_T1_C_1000->Fill(pedcor_M1_T1_C.at(i));
            hist_M1_T2_C_1000->Fill(pedcor_M1_T2_C.at(i));
            hist_M1_T3_C_1000->Fill(pedcor_M1_T3_C.at(i));
            hist_M1_T4_C_1000->Fill(pedcor_M1_T4_C.at(i));
            
            hist_M2_T1_C_1000->Fill(pedcor_M2_T1_C.at(i));
            hist_M2_T2_C_1000->Fill(pedcor_M2_T2_C.at(i));
            hist_M2_T3_C_1000->Fill(pedcor_M2_T3_C.at(i));
            hist_M2_T4_C_1000->Fill(pedcor_M2_T4_C.at(i));
            
            hist_M3_T1_C_1000->Fill(pedcor_M3_T1_C.at(i));
            hist_M3_T2_C_1000->Fill(pedcor_M3_T2_C.at(i));
            hist_M3_T3_C_1000->Fill(pedcor_M3_T3_C.at(i));
            hist_M3_T4_C_1000->Fill(pedcor_M3_T4_C.at(i));
            
            hist_M4_T1_C_1000->Fill(pedcor_M4_T1_C.at(i));
            hist_M4_T2_C_1000->Fill(pedcor_M4_T2_C.at(i));
            hist_M4_T3_C_1000->Fill(pedcor_M4_T3_C.at(i));
            hist_M4_T4_C_1000->Fill(pedcor_M4_T4_C.at(i));
            
            hist_M5_T1_C_1000->Fill(pedcor_M5_T1_C.at(i));
            hist_M5_T2_C_1000->Fill(pedcor_M5_T2_C.at(i));
            hist_M5_T3_C_1000->Fill(pedcor_M5_T3_C.at(i));
            hist_M5_T4_C_1000->Fill(pedcor_M5_T4_C.at(i));
            
            hist_M6_T1_C_1000->Fill(pedcor_M6_T1_C.at(i));
            hist_M6_T2_C_1000->Fill(pedcor_M6_T2_C.at(i));
            hist_M6_T3_C_1000->Fill(pedcor_M6_T3_C.at(i));
            hist_M6_T4_C_1000->Fill(pedcor_M6_T4_C.at(i));
            
            hist_M7_T1_C_1000->Fill(pedcor_M7_T1_C.at(i));
            hist_M7_T2_C_1000->Fill(pedcor_M7_T2_C.at(i));
            hist_M7_T3_C_1000->Fill(pedcor_M7_T3_C.at(i));
            hist_M7_T4_C_1000->Fill(pedcor_M7_T4_C.at(i));
            
            hist_M8_T1_C_1000->Fill(pedcor_M8_T1_C.at(i));
            hist_M8_T2_C_1000->Fill(pedcor_M8_T2_C.at(i));
            hist_M8_T3_C_1000->Fill(pedcor_M8_T3_C.at(i));
            hist_M8_T4_C_1000->Fill(pedcor_M8_T4_C.at(i));

            hist_M9_T1_C_1000->Fill(pedcor_M9_T1_C.at(i));
            hist_M9_T2_C_1000->Fill(pedcor_M9_T2_C.at(i));
            hist_M9_T3_C_1000->Fill(pedcor_M9_T3_C.at(i));
            hist_M9_T4_C_1000->Fill(pedcor_M9_T4_C.at(i));

            hist_M1_T1_S_1000->Fill(pedcor_M1_T1_S.at(i));
            hist_M1_T2_S_1000->Fill(pedcor_M1_T2_S.at(i));
            hist_M1_T3_S_1000->Fill(pedcor_M1_T3_S.at(i));
            hist_M1_T4_S_1000->Fill(pedcor_M1_T4_S.at(i));

            hist_M2_T1_S_1000->Fill(pedcor_M2_T1_S.at(i));
            hist_M2_T2_S_1000->Fill(pedcor_M2_T2_S.at(i));
            hist_M2_T3_S_1000->Fill(pedcor_M2_T3_S.at(i));
            hist_M2_T4_S_1000->Fill(pedcor_M2_T4_S.at(i));

            hist_M3_T1_S_1000->Fill(pedcor_M3_T1_S.at(i));
            hist_M3_T2_S_1000->Fill(pedcor_M3_T2_S.at(i));
            hist_M3_T3_S_1000->Fill(pedcor_M3_T3_S.at(i));
            hist_M3_T4_S_1000->Fill(pedcor_M3_T4_S.at(i));

            hist_M4_T1_S_1000->Fill(pedcor_M4_T1_S.at(i));
            hist_M4_T2_S_1000->Fill(pedcor_M4_T2_S.at(i));
            hist_M4_T3_S_1000->Fill(pedcor_M4_T3_S.at(i));
            hist_M4_T4_S_1000->Fill(pedcor_M4_T4_S.at(i));

            hist_M5_T1_S_1000->Fill(pedcor_M5_T1_S.at(i));
            hist_M5_T2_S_1000->Fill(pedcor_M5_T2_S.at(i));
            hist_M5_T3_S_1000->Fill(pedcor_M5_T3_S.at(i));
            hist_M5_T4_S_1000->Fill(pedcor_M5_T4_S.at(i));

            hist_M6_T1_S_1000->Fill(pedcor_M6_T1_S.at(i));
            hist_M6_T2_S_1000->Fill(pedcor_M6_T2_S.at(i));
            hist_M6_T3_S_1000->Fill(pedcor_M6_T3_S.at(i));
            hist_M6_T4_S_1000->Fill(pedcor_M6_T4_S.at(i));

            hist_M7_T1_S_1000->Fill(pedcor_M7_T1_S.at(i));
            hist_M7_T2_S_1000->Fill(pedcor_M7_T2_S.at(i));
            hist_M7_T3_S_1000->Fill(pedcor_M7_T3_S.at(i));
            hist_M7_T4_S_1000->Fill(pedcor_M7_T4_S.at(i));

            hist_M8_T1_S_1000->Fill(pedcor_M8_T1_S.at(i));
            hist_M8_T2_S_1000->Fill(pedcor_M8_T2_S.at(i));
            hist_M8_T3_S_1000->Fill(pedcor_M8_T3_S.at(i));
            hist_M8_T4_S_1000->Fill(pedcor_M8_T4_S.at(i));

            hist_M9_T1_S_1000->Fill(pedcor_M9_T1_S.at(i));
            hist_M9_T2_S_1000->Fill(pedcor_M9_T2_S.at(i));
            hist_M9_T3_S_1000->Fill(pedcor_M9_T3_S.at(i));
            hist_M9_T4_S_1000->Fill(pedcor_M9_T4_S.at(i));
	}                                            
        for (int i=1;i<1001;i++){ 
            hist_M1_T1_C->Fill(pedcor_M1_T1_C.at(i));
            hist_M1_T2_C->Fill(pedcor_M1_T2_C.at(i));
            hist_M1_T3_C->Fill(pedcor_M1_T3_C.at(i));
            hist_M1_T4_C->Fill(pedcor_M1_T4_C.at(i));
            
            hist_M2_T1_C->Fill(pedcor_M2_T1_C.at(i));
            hist_M2_T2_C->Fill(pedcor_M2_T2_C.at(i));
            hist_M2_T3_C->Fill(pedcor_M2_T3_C.at(i));
            hist_M2_T4_C->Fill(pedcor_M2_T4_C.at(i));
            
            hist_M3_T1_C->Fill(pedcor_M3_T1_C.at(i));
            hist_M3_T2_C->Fill(pedcor_M3_T2_C.at(i));
            hist_M3_T3_C->Fill(pedcor_M3_T3_C.at(i));
            hist_M3_T4_C->Fill(pedcor_M3_T4_C.at(i));
            
            hist_M4_T1_C->Fill(pedcor_M4_T1_C.at(i));
            hist_M4_T2_C->Fill(pedcor_M4_T2_C.at(i));
            hist_M4_T3_C->Fill(pedcor_M4_T3_C.at(i));
            hist_M4_T4_C->Fill(pedcor_M4_T4_C.at(i));
            
            hist_M5_T1_C->Fill(pedcor_M5_T1_C.at(i));
            hist_M5_T2_C->Fill(pedcor_M5_T2_C.at(i));
            hist_M5_T3_C->Fill(pedcor_M5_T3_C.at(i));
            hist_M5_T4_C->Fill(pedcor_M5_T4_C.at(i));
            
            hist_M6_T1_C->Fill(pedcor_M6_T1_C.at(i));
            hist_M6_T2_C->Fill(pedcor_M6_T2_C.at(i));
            hist_M6_T3_C->Fill(pedcor_M6_T3_C.at(i));
            hist_M6_T4_C->Fill(pedcor_M6_T4_C.at(i));
            
            hist_M7_T1_C->Fill(pedcor_M7_T1_C.at(i));
            hist_M7_T2_C->Fill(pedcor_M7_T2_C.at(i));
            hist_M7_T3_C->Fill(pedcor_M7_T3_C.at(i));
            hist_M7_T4_C->Fill(pedcor_M7_T4_C.at(i));
            
            hist_M8_T1_C->Fill(pedcor_M8_T1_C.at(i));
            hist_M8_T2_C->Fill(pedcor_M8_T2_C.at(i));
            hist_M8_T3_C->Fill(pedcor_M8_T3_C.at(i));
            hist_M8_T4_C->Fill(pedcor_M8_T4_C.at(i));
                                           
            hist_M9_T1_C->Fill(pedcor_M9_T1_C.at(i));
            hist_M9_T2_C->Fill(pedcor_M9_T2_C.at(i));
            hist_M9_T3_C->Fill(pedcor_M9_T3_C.at(i));
            hist_M9_T4_C->Fill(pedcor_M9_T4_C.at(i));
                                           
            hist_M1_T1_S->Fill(pedcor_M1_T1_S.at(i));
            hist_M1_T2_S->Fill(pedcor_M1_T2_S.at(i));
            hist_M1_T3_S->Fill(pedcor_M1_T3_S.at(i));
            hist_M1_T4_S->Fill(pedcor_M1_T4_S.at(i));
                                           
            hist_M2_T1_S->Fill(pedcor_M2_T1_S.at(i));
            hist_M2_T2_S->Fill(pedcor_M2_T2_S.at(i));
            hist_M2_T3_S->Fill(pedcor_M2_T3_S.at(i));
            hist_M2_T4_S->Fill(pedcor_M2_T4_S.at(i));
                                           
            hist_M3_T1_S->Fill(pedcor_M3_T1_S.at(i));
            hist_M3_T2_S->Fill(pedcor_M3_T2_S.at(i));
            hist_M3_T3_S->Fill(pedcor_M3_T3_S.at(i));
            hist_M3_T4_S->Fill(pedcor_M3_T4_S.at(i));
                                           
            hist_M4_T1_S->Fill(pedcor_M4_T1_S.at(i));
            hist_M4_T2_S->Fill(pedcor_M4_T2_S.at(i));
            hist_M4_T3_S->Fill(pedcor_M4_T3_S.at(i));
            hist_M4_T4_S->Fill(pedcor_M4_T4_S.at(i));
                                        
            hist_M5_T1_S->Fill(pedcor_M5_T1_S.at(i));
            hist_M5_T2_S->Fill(pedcor_M5_T2_S.at(i));
            hist_M5_T3_S->Fill(pedcor_M5_T3_S.at(i));
            hist_M5_T4_S->Fill(pedcor_M5_T4_S.at(i));
                                           
            hist_M6_T1_S->Fill(pedcor_M6_T1_S.at(i));
            hist_M6_T2_S->Fill(pedcor_M6_T2_S.at(i));
            hist_M6_T3_S->Fill(pedcor_M6_T3_S.at(i));
            hist_M6_T4_S->Fill(pedcor_M6_T4_S.at(i));
                                           
            hist_M7_T1_S->Fill(pedcor_M7_T1_S.at(i));
            hist_M7_T2_S->Fill(pedcor_M7_T2_S.at(i));
            hist_M7_T3_S->Fill(pedcor_M7_T3_S.at(i));
            hist_M7_T4_S->Fill(pedcor_M7_T4_S.at(i));
                                           
            hist_M8_T1_S->Fill(pedcor_M8_T1_S.at(i));
            hist_M8_T2_S->Fill(pedcor_M8_T2_S.at(i));
            hist_M8_T3_S->Fill(pedcor_M8_T3_S.at(i));
            hist_M8_T4_S->Fill(pedcor_M8_T4_S.at(i));
                                           
            hist_M9_T1_S->Fill(pedcor_M9_T1_S.at(i));
            hist_M9_T2_S->Fill(pedcor_M9_T2_S.at(i));
            hist_M9_T3_S->Fill(pedcor_M9_T3_S.at(i));
            hist_M9_T4_S->Fill(pedcor_M9_T4_S.at(i));
	}                                            
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
        if ( signal_PS < cut_PS1 || signal_PS > cut_PS2 ) continue; // Select above 3 mip peak
        if ( signal_MC < cut_MC ) continue; // Select only pedestals
        
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
    std::string outFile = "./DRC_ntuple/DRC_Run_" + std::to_string(fRunNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();
    
    fit_scale_C->Write();
    fit_scale_S->Write();
    
    fit_reso_C->Write();
    fit_reso_S->Write();
    fit_reso_Comb->Write();
    
    hist_M1_T1_C_100->Write();
    hist_M1_T2_C_100->Write();
    hist_M1_T3_C_100->Write();
    hist_M1_T4_C_100->Write();
    
    hist_M2_T1_C_100->Write();
    hist_M2_T2_C_100->Write();
    hist_M2_T3_C_100->Write();
    hist_M2_T4_C_100->Write();
    
    hist_M3_T1_C_100->Write();
    hist_M3_T2_C_100->Write();
    hist_M3_T3_C_100->Write();
    hist_M3_T4_C_100->Write();
    
    hist_M4_T1_C_100->Write();
    hist_M4_T2_C_100->Write();
    hist_M4_T3_C_100->Write();
    hist_M4_T4_C_100->Write();
    
    hist_M5_T1_C_100->Write();
    hist_M5_T2_C_100->Write();
    hist_M5_T3_C_100->Write();
    hist_M5_T4_C_100->Write();
    
    hist_M6_T1_C_100->Write();
    hist_M6_T2_C_100->Write();
    hist_M6_T3_C_100->Write();
    hist_M6_T4_C_100->Write();
    
    hist_M7_T1_C_100->Write();
    hist_M7_T2_C_100->Write();
    hist_M7_T3_C_100->Write();
    hist_M7_T4_C_100->Write();
    
    hist_M8_T1_C_100->Write();
    hist_M8_T2_C_100->Write();
    hist_M8_T3_C_100->Write();
    hist_M8_T4_C_100->Write();
    
    hist_M9_T1_C_100->Write();
    hist_M9_T2_C_100->Write();
    hist_M9_T3_C_100->Write();
    hist_M9_T4_C_100->Write();
    
    hist_M1_T1_S_100->Write();
    hist_M1_T2_S_100->Write();
    hist_M1_T3_S_100->Write();
    hist_M1_T4_S_100->Write();
    
    hist_M2_T1_S_100->Write();
    hist_M2_T2_S_100->Write();
    hist_M2_T3_S_100->Write();
    hist_M2_T4_S_100->Write();
    
    hist_M3_T1_S_100->Write();
    hist_M3_T2_S_100->Write();
    hist_M3_T3_S_100->Write();
    hist_M3_T4_S_100->Write();
    
    hist_M4_T1_S_100->Write();
    hist_M4_T2_S_100->Write();
    hist_M4_T3_S_100->Write();
    hist_M4_T4_S_100->Write();
    
    hist_M5_T1_S_100->Write();
    hist_M5_T2_S_100->Write();
    hist_M5_T3_S_100->Write();
    hist_M5_T4_S_100->Write();
    
    hist_M6_T1_S_100->Write();
    hist_M6_T2_S_100->Write();
    hist_M6_T3_S_100->Write();
    hist_M6_T4_S_100->Write();
    
    hist_M7_T1_S_100->Write();
    hist_M7_T2_S_100->Write();
    hist_M7_T3_S_100->Write();
    hist_M7_T4_S_100->Write();
    
    hist_M8_T1_S_100->Write();
    hist_M8_T2_S_100->Write();
    hist_M8_T3_S_100->Write();
    hist_M8_T4_S_100->Write();
    
    hist_M9_T1_S_100->Write();
    hist_M9_T2_S_100->Write();
    hist_M9_T3_S_100->Write();
    hist_M9_T4_S_100->Write();
    
    hist_M1_T1_C_200->Write();
    hist_M1_T2_C_200->Write();
    hist_M1_T3_C_200->Write();
    hist_M1_T4_C_200->Write();
    
    hist_M2_T1_C_200->Write();
    hist_M2_T2_C_200->Write();
    hist_M2_T3_C_200->Write();
    hist_M2_T4_C_200->Write();
    
    hist_M3_T1_C_200->Write();
    hist_M3_T2_C_200->Write();
    hist_M3_T3_C_200->Write();
    hist_M3_T4_C_200->Write();
    
    hist_M4_T1_C_200->Write();
    hist_M4_T2_C_200->Write();
    hist_M4_T3_C_200->Write();
    hist_M4_T4_C_200->Write();
    
    hist_M5_T1_C_200->Write();
    hist_M5_T2_C_200->Write();
    hist_M5_T3_C_200->Write();
    hist_M5_T4_C_200->Write();
    
    hist_M6_T1_C_200->Write();
    hist_M6_T2_C_200->Write();
    hist_M6_T3_C_200->Write();
    hist_M6_T4_C_200->Write();
    
    hist_M7_T1_C_200->Write();
    hist_M7_T2_C_200->Write();
    hist_M7_T3_C_200->Write();
    hist_M7_T4_C_200->Write();
    
    hist_M8_T1_C_200->Write();
    hist_M8_T2_C_200->Write();
    hist_M8_T3_C_200->Write();
    hist_M8_T4_C_200->Write();
    
    hist_M9_T1_C_200->Write();
    hist_M9_T2_C_200->Write();
    hist_M9_T3_C_200->Write();
    hist_M9_T4_C_200->Write();
    
    hist_M1_T1_S_200->Write();
    hist_M1_T2_S_200->Write();
    hist_M1_T3_S_200->Write();
    hist_M1_T4_S_200->Write();
    
    hist_M2_T1_S_200->Write();
    hist_M2_T2_S_200->Write();
    hist_M2_T3_S_200->Write();
    hist_M2_T4_S_200->Write();
    
    hist_M3_T1_S_200->Write();
    hist_M3_T2_S_200->Write();
    hist_M3_T3_S_200->Write();
    hist_M3_T4_S_200->Write();
    
    hist_M4_T1_S_200->Write();
    hist_M4_T2_S_200->Write();
    hist_M4_T3_S_200->Write();
    hist_M4_T4_S_200->Write();
    
    hist_M5_T1_S_200->Write();
    hist_M5_T2_S_200->Write();
    hist_M5_T3_S_200->Write();
    hist_M5_T4_S_200->Write();
    
    hist_M6_T1_S_200->Write();
    hist_M6_T2_S_200->Write();
    hist_M6_T3_S_200->Write();
    hist_M6_T4_S_200->Write();
    
    hist_M7_T1_S_200->Write();
    hist_M7_T2_S_200->Write();
    hist_M7_T3_S_200->Write();
    hist_M7_T4_S_200->Write();
    
    hist_M8_T1_S_200->Write();
    hist_M8_T2_S_200->Write();
    hist_M8_T3_S_200->Write();
    hist_M8_T4_S_200->Write();
    
    hist_M9_T1_S_200->Write();
    hist_M9_T2_S_200->Write();
    hist_M9_T3_S_200->Write();
    hist_M9_T4_S_200->Write();
    
    hist_M1_T1_C_300->Write();
    hist_M1_T2_C_300->Write();
    hist_M1_T3_C_300->Write();
    hist_M1_T4_C_300->Write();
    
    hist_M2_T1_C_300->Write();
    hist_M2_T2_C_300->Write();
    hist_M2_T3_C_300->Write();
    hist_M2_T4_C_300->Write();
    
    hist_M3_T1_C_300->Write();
    hist_M3_T2_C_300->Write();
    hist_M3_T3_C_300->Write();
    hist_M3_T4_C_300->Write();
    
    hist_M4_T1_C_300->Write();
    hist_M4_T2_C_300->Write();
    hist_M4_T3_C_300->Write();
    hist_M4_T4_C_300->Write();
    
    hist_M5_T1_C_300->Write();
    hist_M5_T2_C_300->Write();
    hist_M5_T3_C_300->Write();
    hist_M5_T4_C_300->Write();
    
    hist_M6_T1_C_300->Write();
    hist_M6_T2_C_300->Write();
    hist_M6_T3_C_300->Write();
    hist_M6_T4_C_300->Write();
    
    hist_M7_T1_C_300->Write();
    hist_M7_T2_C_300->Write();
    hist_M7_T3_C_300->Write();
    hist_M7_T4_C_300->Write();
    
    hist_M8_T1_C_300->Write();
    hist_M8_T2_C_300->Write();
    hist_M8_T3_C_300->Write();
    hist_M8_T4_C_300->Write();
    
    hist_M9_T1_C_300->Write();
    hist_M9_T2_C_300->Write();
    hist_M9_T3_C_300->Write();
    hist_M9_T4_C_300->Write();
    
    hist_M1_T1_S_300->Write();
    hist_M1_T2_S_300->Write();
    hist_M1_T3_S_300->Write();
    hist_M1_T4_S_300->Write();
    
    hist_M2_T1_S_300->Write();
    hist_M2_T2_S_300->Write();
    hist_M2_T3_S_300->Write();
    hist_M2_T4_S_300->Write();
    
    hist_M3_T1_S_300->Write();
    hist_M3_T2_S_300->Write();
    hist_M3_T3_S_300->Write();
    hist_M3_T4_S_300->Write();
    
    hist_M4_T1_S_300->Write();
    hist_M4_T2_S_300->Write();
    hist_M4_T3_S_300->Write();
    hist_M4_T4_S_300->Write();
    
    hist_M5_T1_S_300->Write();
    hist_M5_T2_S_300->Write();
    hist_M5_T3_S_300->Write();
    hist_M5_T4_S_300->Write();
    
    hist_M6_T1_S_300->Write();
    hist_M6_T2_S_300->Write();
    hist_M6_T3_S_300->Write();
    hist_M6_T4_S_300->Write();
    
    hist_M7_T1_S_300->Write();
    hist_M7_T2_S_300->Write();
    hist_M7_T3_S_300->Write();
    hist_M7_T4_S_300->Write();
    
    hist_M8_T1_S_300->Write();
    hist_M8_T2_S_300->Write();
    hist_M8_T3_S_300->Write();
    hist_M8_T4_S_300->Write();
    
    hist_M9_T1_S_300->Write();
    hist_M9_T2_S_300->Write();
    hist_M9_T3_S_300->Write();
    hist_M9_T4_S_300->Write();
    
    hist_M1_T1_C_400->Write();
    hist_M1_T2_C_400->Write();
    hist_M1_T3_C_400->Write();
    hist_M1_T4_C_400->Write();
    
    hist_M2_T1_C_400->Write();
    hist_M2_T2_C_400->Write();
    hist_M2_T3_C_400->Write();
    hist_M2_T4_C_400->Write();
    
    hist_M3_T1_C_400->Write();
    hist_M3_T2_C_400->Write();
    hist_M3_T3_C_400->Write();
    hist_M3_T4_C_400->Write();
    
    hist_M4_T1_C_400->Write();
    hist_M4_T2_C_400->Write();
    hist_M4_T3_C_400->Write();
    hist_M4_T4_C_400->Write();
    
    hist_M5_T1_C_400->Write();
    hist_M5_T2_C_400->Write();
    hist_M5_T3_C_400->Write();
    hist_M5_T4_C_400->Write();
    
    hist_M6_T1_C_400->Write();
    hist_M6_T2_C_400->Write();
    hist_M6_T3_C_400->Write();
    hist_M6_T4_C_400->Write();
    
    hist_M7_T1_C_400->Write();
    hist_M7_T2_C_400->Write();
    hist_M7_T3_C_400->Write();
    hist_M7_T4_C_400->Write();
    
    hist_M8_T1_C_400->Write();
    hist_M8_T2_C_400->Write();
    hist_M8_T3_C_400->Write();
    hist_M8_T4_C_400->Write();
    
    hist_M9_T1_C_400->Write();
    hist_M9_T2_C_400->Write();
    hist_M9_T3_C_400->Write();
    hist_M9_T4_C_400->Write();
    
    hist_M1_T1_S_400->Write();
    hist_M1_T2_S_400->Write();
    hist_M1_T3_S_400->Write();
    hist_M1_T4_S_400->Write();
    
    hist_M2_T1_S_400->Write();
    hist_M2_T2_S_400->Write();
    hist_M2_T3_S_400->Write();
    hist_M2_T4_S_400->Write();
    
    hist_M3_T1_S_400->Write();
    hist_M3_T2_S_400->Write();
    hist_M3_T3_S_400->Write();
    hist_M3_T4_S_400->Write();
    
    hist_M4_T1_S_400->Write();
    hist_M4_T2_S_400->Write();
    hist_M4_T3_S_400->Write();
    hist_M4_T4_S_400->Write();
    
    hist_M5_T1_S_400->Write();
    hist_M5_T2_S_400->Write();
    hist_M5_T3_S_400->Write();
    hist_M5_T4_S_400->Write();
    
    hist_M6_T1_S_400->Write();
    hist_M6_T2_S_400->Write();
    hist_M6_T3_S_400->Write();
    hist_M6_T4_S_400->Write();
    
    hist_M7_T1_S_400->Write();
    hist_M7_T2_S_400->Write();
    hist_M7_T3_S_400->Write();
    hist_M7_T4_S_400->Write();
    
    hist_M8_T1_S_400->Write();
    hist_M8_T2_S_400->Write();
    hist_M8_T3_S_400->Write();
    hist_M8_T4_S_400->Write();
    
    hist_M9_T1_S_400->Write();
    hist_M9_T2_S_400->Write();
    hist_M9_T3_S_400->Write();
    hist_M9_T4_S_400->Write();
    
    hist_M1_T1_C_500->Write();
    hist_M1_T2_C_500->Write();
    hist_M1_T3_C_500->Write();
    hist_M1_T4_C_500->Write();
    
    hist_M2_T1_C_500->Write();
    hist_M2_T2_C_500->Write();
    hist_M2_T3_C_500->Write();
    hist_M2_T4_C_500->Write();
    
    hist_M3_T1_C_500->Write();
    hist_M3_T2_C_500->Write();
    hist_M3_T3_C_500->Write();
    hist_M3_T4_C_500->Write();
    
    hist_M4_T1_C_500->Write();
    hist_M4_T2_C_500->Write();
    hist_M4_T3_C_500->Write();
    hist_M4_T4_C_500->Write();
    
    hist_M5_T1_C_500->Write();
    hist_M5_T2_C_500->Write();
    hist_M5_T3_C_500->Write();
    hist_M5_T4_C_500->Write();
    
    hist_M6_T1_C_500->Write();
    hist_M6_T2_C_500->Write();
    hist_M6_T3_C_500->Write();
    hist_M6_T4_C_500->Write();
    
    hist_M7_T1_C_500->Write();
    hist_M7_T2_C_500->Write();
    hist_M7_T3_C_500->Write();
    hist_M7_T4_C_500->Write();
    
    hist_M8_T1_C_500->Write();
    hist_M8_T2_C_500->Write();
    hist_M8_T3_C_500->Write();
    hist_M8_T4_C_500->Write();
    
    hist_M9_T1_C_500->Write();
    hist_M9_T2_C_500->Write();
    hist_M9_T3_C_500->Write();
    hist_M9_T4_C_500->Write();
    
    hist_M1_T1_S_500->Write();
    hist_M1_T2_S_500->Write();
    hist_M1_T3_S_500->Write();
    hist_M1_T4_S_500->Write();
    
    hist_M2_T1_S_500->Write();
    hist_M2_T2_S_500->Write();
    hist_M2_T3_S_500->Write();
    hist_M2_T4_S_500->Write();
    
    hist_M3_T1_S_500->Write();
    hist_M3_T2_S_500->Write();
    hist_M3_T3_S_500->Write();
    hist_M3_T4_S_500->Write();
    
    hist_M4_T1_S_500->Write();
    hist_M4_T2_S_500->Write();
    hist_M4_T3_S_500->Write();
    hist_M4_T4_S_500->Write();
    
    hist_M5_T1_S_500->Write();
    hist_M5_T2_S_500->Write();
    hist_M5_T3_S_500->Write();
    hist_M5_T4_S_500->Write();
    
    hist_M6_T1_S_500->Write();
    hist_M6_T2_S_500->Write();
    hist_M6_T3_S_500->Write();
    hist_M6_T4_S_500->Write();
    
    hist_M7_T1_S_500->Write();
    hist_M7_T2_S_500->Write();
    hist_M7_T3_S_500->Write();
    hist_M7_T4_S_500->Write();
    
    hist_M8_T1_S_500->Write();
    hist_M8_T2_S_500->Write();
    hist_M8_T3_S_500->Write();
    hist_M8_T4_S_500->Write();
    
    hist_M9_T1_S_500->Write();
    hist_M9_T2_S_500->Write();
    hist_M9_T3_S_500->Write();
    hist_M9_T4_S_500->Write();
    
    hist_M1_T1_C_600->Write();
    hist_M1_T2_C_600->Write();
    hist_M1_T3_C_600->Write();
    hist_M1_T4_C_600->Write();
    
    hist_M2_T1_C_600->Write();
    hist_M2_T2_C_600->Write();
    hist_M2_T3_C_600->Write();
    hist_M2_T4_C_600->Write();
    
    hist_M3_T1_C_600->Write();
    hist_M3_T2_C_600->Write();
    hist_M3_T3_C_600->Write();
    hist_M3_T4_C_600->Write();
    
    hist_M4_T1_C_600->Write();
    hist_M4_T2_C_600->Write();
    hist_M4_T3_C_600->Write();
    hist_M4_T4_C_600->Write();
    
    hist_M5_T1_C_600->Write();
    hist_M5_T2_C_600->Write();
    hist_M5_T3_C_600->Write();
    hist_M5_T4_C_600->Write();
    
    hist_M6_T1_C_600->Write();
    hist_M6_T2_C_600->Write();
    hist_M6_T3_C_600->Write();
    hist_M6_T4_C_600->Write();
    
    hist_M7_T1_C_600->Write();
    hist_M7_T2_C_600->Write();
    hist_M7_T3_C_600->Write();
    hist_M7_T4_C_600->Write();
    
    hist_M8_T1_C_600->Write();
    hist_M8_T2_C_600->Write();
    hist_M8_T3_C_600->Write();
    hist_M8_T4_C_600->Write();
    
    hist_M9_T1_C_600->Write();
    hist_M9_T2_C_600->Write();
    hist_M9_T3_C_600->Write();
    hist_M9_T4_C_600->Write();
    
    hist_M1_T1_S_600->Write();
    hist_M1_T2_S_600->Write();
    hist_M1_T3_S_600->Write();
    hist_M1_T4_S_600->Write();
    
    hist_M2_T1_S_600->Write();
    hist_M2_T2_S_600->Write();
    hist_M2_T3_S_600->Write();
    hist_M2_T4_S_600->Write();
    
    hist_M3_T1_S_600->Write();
    hist_M3_T2_S_600->Write();
    hist_M3_T3_S_600->Write();
    hist_M3_T4_S_600->Write();
    
    hist_M4_T1_S_600->Write();
    hist_M4_T2_S_600->Write();
    hist_M4_T3_S_600->Write();
    hist_M4_T4_S_600->Write();
    
    hist_M5_T1_S_600->Write();
    hist_M5_T2_S_600->Write();
    hist_M5_T3_S_600->Write();
    hist_M5_T4_S_600->Write();
    
    hist_M6_T1_S_600->Write();
    hist_M6_T2_S_600->Write();
    hist_M6_T3_S_600->Write();
    hist_M6_T4_S_600->Write();
    
    hist_M7_T1_S_600->Write();
    hist_M7_T2_S_600->Write();
    hist_M7_T3_S_600->Write();
    hist_M7_T4_S_600->Write();
    
    hist_M8_T1_S_600->Write();
    hist_M8_T2_S_600->Write();
    hist_M8_T3_S_600->Write();
    hist_M8_T4_S_600->Write();
    
    hist_M9_T1_S_600->Write();
    hist_M9_T2_S_600->Write();
    hist_M9_T3_S_600->Write();
    hist_M9_T4_S_600->Write();
    
    hist_M1_T1_C_700->Write();
    hist_M1_T2_C_700->Write();
    hist_M1_T3_C_700->Write();
    hist_M1_T4_C_700->Write();
    
    hist_M2_T1_C_700->Write();
    hist_M2_T2_C_700->Write();
    hist_M2_T3_C_700->Write();
    hist_M2_T4_C_700->Write();
    
    hist_M3_T1_C_700->Write();
    hist_M3_T2_C_700->Write();
    hist_M3_T3_C_700->Write();
    hist_M3_T4_C_700->Write();
    
    hist_M4_T1_C_700->Write();
    hist_M4_T2_C_700->Write();
    hist_M4_T3_C_700->Write();
    hist_M4_T4_C_700->Write();
    
    hist_M5_T1_C_700->Write();
    hist_M5_T2_C_700->Write();
    hist_M5_T3_C_700->Write();
    hist_M5_T4_C_700->Write();
    
    hist_M6_T1_C_700->Write();
    hist_M6_T2_C_700->Write();
    hist_M6_T3_C_700->Write();
    hist_M6_T4_C_700->Write();
    
    hist_M7_T1_C_700->Write();
    hist_M7_T2_C_700->Write();
    hist_M7_T3_C_700->Write();
    hist_M7_T4_C_700->Write();
    
    hist_M8_T1_C_700->Write();
    hist_M8_T2_C_700->Write();
    hist_M8_T3_C_700->Write();
    hist_M8_T4_C_700->Write();
    
    hist_M9_T1_C_700->Write();
    hist_M9_T2_C_700->Write();
    hist_M9_T3_C_700->Write();
    hist_M9_T4_C_700->Write();
    
    hist_M1_T1_S_700->Write();
    hist_M1_T2_S_700->Write();
    hist_M1_T3_S_700->Write();
    hist_M1_T4_S_700->Write();
    
    hist_M2_T1_S_700->Write();
    hist_M2_T2_S_700->Write();
    hist_M2_T3_S_700->Write();
    hist_M2_T4_S_700->Write();
    
    hist_M3_T1_S_700->Write();
    hist_M3_T2_S_700->Write();
    hist_M3_T3_S_700->Write();
    hist_M3_T4_S_700->Write();
    
    hist_M4_T1_S_700->Write();
    hist_M4_T2_S_700->Write();
    hist_M4_T3_S_700->Write();
    hist_M4_T4_S_700->Write();
    
    hist_M5_T1_S_700->Write();
    hist_M5_T2_S_700->Write();
    hist_M5_T3_S_700->Write();
    hist_M5_T4_S_700->Write();
    
    hist_M6_T1_S_700->Write();
    hist_M6_T2_S_700->Write();
    hist_M6_T3_S_700->Write();
    hist_M6_T4_S_700->Write();
    
    hist_M7_T1_S_700->Write();
    hist_M7_T2_S_700->Write();
    hist_M7_T3_S_700->Write();
    hist_M7_T4_S_700->Write();
    
    hist_M8_T1_S_700->Write();
    hist_M8_T2_S_700->Write();
    hist_M8_T3_S_700->Write();
    hist_M8_T4_S_700->Write();
    
    hist_M9_T1_S_700->Write();
    hist_M9_T2_S_700->Write();
    hist_M9_T3_S_700->Write();
    hist_M9_T4_S_700->Write();
    
    hist_M1_T1_C_800->Write();
    hist_M1_T2_C_800->Write();
    hist_M1_T3_C_800->Write();
    hist_M1_T4_C_800->Write();
    
    hist_M2_T1_C_800->Write();
    hist_M2_T2_C_800->Write();
    hist_M2_T3_C_800->Write();
    hist_M2_T4_C_800->Write();
    
    hist_M3_T1_C_800->Write();
    hist_M3_T2_C_800->Write();
    hist_M3_T3_C_800->Write();
    hist_M3_T4_C_800->Write();
    
    hist_M4_T1_C_800->Write();
    hist_M4_T2_C_800->Write();
    hist_M4_T3_C_800->Write();
    hist_M4_T4_C_800->Write();
    
    hist_M5_T1_C_800->Write();
    hist_M5_T2_C_800->Write();
    hist_M5_T3_C_800->Write();
    hist_M5_T4_C_800->Write();
    
    hist_M6_T1_C_800->Write();
    hist_M6_T2_C_800->Write();
    hist_M6_T3_C_800->Write();
    hist_M6_T4_C_800->Write();
    
    hist_M7_T1_C_800->Write();
    hist_M7_T2_C_800->Write();
    hist_M7_T3_C_800->Write();
    hist_M7_T4_C_800->Write();
    
    hist_M8_T1_C_800->Write();
    hist_M8_T2_C_800->Write();
    hist_M8_T3_C_800->Write();
    hist_M8_T4_C_800->Write();
    
    hist_M9_T1_C_800->Write();
    hist_M9_T2_C_800->Write();
    hist_M9_T3_C_800->Write();
    hist_M9_T4_C_800->Write();
    
    hist_M1_T1_S_800->Write();
    hist_M1_T2_S_800->Write();
    hist_M1_T3_S_800->Write();
    hist_M1_T4_S_800->Write();
    
    hist_M2_T1_S_800->Write();
    hist_M2_T2_S_800->Write();
    hist_M2_T3_S_800->Write();
    hist_M2_T4_S_800->Write();
    
    hist_M3_T1_S_800->Write();
    hist_M3_T2_S_800->Write();
    hist_M3_T3_S_800->Write();
    hist_M3_T4_S_800->Write();
    
    hist_M4_T1_S_800->Write();
    hist_M4_T2_S_800->Write();
    hist_M4_T3_S_800->Write();
    hist_M4_T4_S_800->Write();
    
    hist_M5_T1_S_800->Write();
    hist_M5_T2_S_800->Write();
    hist_M5_T3_S_800->Write();
    hist_M5_T4_S_800->Write();
    
    hist_M6_T1_S_800->Write();
    hist_M6_T2_S_800->Write();
    hist_M6_T3_S_800->Write();
    hist_M6_T4_S_800->Write();
    
    hist_M7_T1_S_800->Write();
    hist_M7_T2_S_800->Write();
    hist_M7_T3_S_800->Write();
    hist_M7_T4_S_800->Write();
    
    hist_M8_T1_S_800->Write();
    hist_M8_T2_S_800->Write();
    hist_M8_T3_S_800->Write();
    hist_M8_T4_S_800->Write();
    
    hist_M9_T1_S_800->Write();
    hist_M9_T2_S_800->Write();
    hist_M9_T3_S_800->Write();
    hist_M9_T4_S_800->Write();
    
    hist_M1_T1_C_900->Write();
    hist_M1_T2_C_900->Write();
    hist_M1_T3_C_900->Write();
    hist_M1_T4_C_900->Write();
    
    hist_M2_T1_C_900->Write();
    hist_M2_T2_C_900->Write();
    hist_M2_T3_C_900->Write();
    hist_M2_T4_C_900->Write();
    
    hist_M3_T1_C_900->Write();
    hist_M3_T2_C_900->Write();
    hist_M3_T3_C_900->Write();
    hist_M3_T4_C_900->Write();
    
    hist_M4_T1_C_900->Write();
    hist_M4_T2_C_900->Write();
    hist_M4_T3_C_900->Write();
    hist_M4_T4_C_900->Write();
    
    hist_M5_T1_C_900->Write();
    hist_M5_T2_C_900->Write();
    hist_M5_T3_C_900->Write();
    hist_M5_T4_C_900->Write();
    
    hist_M6_T1_C_900->Write();
    hist_M6_T2_C_900->Write();
    hist_M6_T3_C_900->Write();
    hist_M6_T4_C_900->Write();
    
    hist_M7_T1_C_900->Write();
    hist_M7_T2_C_900->Write();
    hist_M7_T3_C_900->Write();
    hist_M7_T4_C_900->Write();
    
    hist_M8_T1_C_900->Write();
    hist_M8_T2_C_900->Write();
    hist_M8_T3_C_900->Write();
    hist_M8_T4_C_900->Write();
    
    hist_M9_T1_C_900->Write();
    hist_M9_T2_C_900->Write();
    hist_M9_T3_C_900->Write();
    hist_M9_T4_C_900->Write();
    
    hist_M1_T1_S_900->Write();
    hist_M1_T2_S_900->Write();
    hist_M1_T3_S_900->Write();
    hist_M1_T4_S_900->Write();
    
    hist_M2_T1_S_900->Write();
    hist_M2_T2_S_900->Write();
    hist_M2_T3_S_900->Write();
    hist_M2_T4_S_900->Write();
    
    hist_M3_T1_S_900->Write();
    hist_M3_T2_S_900->Write();
    hist_M3_T3_S_900->Write();
    hist_M3_T4_S_900->Write();
    
    hist_M4_T1_S_900->Write();
    hist_M4_T2_S_900->Write();
    hist_M4_T3_S_900->Write();
    hist_M4_T4_S_900->Write();
    
    hist_M5_T1_S_900->Write();
    hist_M5_T2_S_900->Write();
    hist_M5_T3_S_900->Write();
    hist_M5_T4_S_900->Write();
    
    hist_M6_T1_S_900->Write();
    hist_M6_T2_S_900->Write();
    hist_M6_T3_S_900->Write();
    hist_M6_T4_S_900->Write();
    
    hist_M7_T1_S_900->Write();
    hist_M7_T2_S_900->Write();
    hist_M7_T3_S_900->Write();
    hist_M7_T4_S_900->Write();
    
    hist_M8_T1_S_900->Write();
    hist_M8_T2_S_900->Write();
    hist_M8_T3_S_900->Write();
    hist_M8_T4_S_900->Write();
    
    hist_M9_T1_S_900->Write();
    hist_M9_T2_S_900->Write();
    hist_M9_T3_S_900->Write();
    hist_M9_T4_S_900->Write();
    
    hist_M1_T1_C_1000->Write();
    hist_M1_T2_C_1000->Write();
    hist_M1_T3_C_1000->Write();
    hist_M1_T4_C_1000->Write();
    
    hist_M2_T1_C_1000->Write();
    hist_M2_T2_C_1000->Write();
    hist_M2_T3_C_1000->Write();
    hist_M2_T4_C_1000->Write();
    
    hist_M3_T1_C_1000->Write();
    hist_M3_T2_C_1000->Write();
    hist_M3_T3_C_1000->Write();
    hist_M3_T4_C_1000->Write();
    
    hist_M4_T1_C_1000->Write();
    hist_M4_T2_C_1000->Write();
    hist_M4_T3_C_1000->Write();
    hist_M4_T4_C_1000->Write();
    
    hist_M5_T1_C_1000->Write();
    hist_M5_T2_C_1000->Write();
    hist_M5_T3_C_1000->Write();
    hist_M5_T4_C_1000->Write();
    
    hist_M6_T1_C_1000->Write();
    hist_M6_T2_C_1000->Write();
    hist_M6_T3_C_1000->Write();
    hist_M6_T4_C_1000->Write();
    
    hist_M7_T1_C_1000->Write();
    hist_M7_T2_C_1000->Write();
    hist_M7_T3_C_1000->Write();
    hist_M7_T4_C_1000->Write();
    
    hist_M8_T1_C_1000->Write();
    hist_M8_T2_C_1000->Write();
    hist_M8_T3_C_1000->Write();
    hist_M8_T4_C_1000->Write();
    
    hist_M9_T1_C_1000->Write();
    hist_M9_T2_C_1000->Write();
    hist_M9_T3_C_1000->Write();
    hist_M9_T4_C_1000->Write();
    
    hist_M1_T1_S_1000->Write();
    hist_M1_T2_S_1000->Write();
    hist_M1_T3_S_1000->Write();
    hist_M1_T4_S_1000->Write();
    
    hist_M2_T1_S_1000->Write();
    hist_M2_T2_S_1000->Write();
    hist_M2_T3_S_1000->Write();
    hist_M2_T4_S_1000->Write();
    
    hist_M3_T1_S_1000->Write();
    hist_M3_T2_S_1000->Write();
    hist_M3_T3_S_1000->Write();
    hist_M3_T4_S_1000->Write();
    
    hist_M4_T1_S_1000->Write();
    hist_M4_T2_S_1000->Write();
    hist_M4_T3_S_1000->Write();
    hist_M4_T4_S_1000->Write();
    
    hist_M5_T1_S_1000->Write();
    hist_M5_T2_S_1000->Write();
    hist_M5_T3_S_1000->Write();
    hist_M5_T4_S_1000->Write();
    
    hist_M6_T1_S_1000->Write();
    hist_M6_T2_S_1000->Write();
    hist_M6_T3_S_1000->Write();
    hist_M6_T4_S_1000->Write();
    
    hist_M7_T1_S_1000->Write();
    hist_M7_T2_S_1000->Write();
    hist_M7_T3_S_1000->Write();
    hist_M7_T4_S_1000->Write();
    
    hist_M8_T1_S_1000->Write();
    hist_M8_T2_S_1000->Write();
    hist_M8_T3_S_1000->Write();
    hist_M8_T4_S_1000->Write();
    
    hist_M9_T1_S_1000->Write();
    hist_M9_T2_S_1000->Write();
    hist_M9_T3_S_1000->Write();
    hist_M9_T4_S_1000->Write();
    
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
