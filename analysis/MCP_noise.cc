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
    
    float cut_PS = 424.; // PID cut for PS (PeakADC)
    float cut_MC = 38.;   // PID cut for MC (PeakADC)
    
    float cut_DWC = 4; // Beam geometry cut for DWC
    
    // Scaled factor for each channel
    // it could be like
    int fRunNum = std::stoi(argv[1]);
    int fMaxEvent = std::stoi(argv[2]);
    int fMaxFile = -1;
    
    fs::path dir("./MCP_ntuple");   
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
    
    // TH1F* hist_PS_after = new TH1F("PS_after" , ";intADC;nEvents", 2000, -1000, 1000);
    // TH1F* hist_MC_after = new TH1F("MC_after" , ";intADC;nEvents", 2000, -1000, 1000);
    // TH1F* hist_TC_after = new TH1F("TC_after" , ";intADC;nEvents", 2000, -1000, 1000);

    // Get Ntuple
    //TFile* fNtuple = TFile::Open((TString)("/Users/yhep/DRC/TB2025/analysis/SW/Prompt_ntuple/Prompt_ntuple_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    TFile* fNtuple = TFile::Open((TString)("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_PromptAnalysis_MCP/Prompt_ntuple_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    // Create TTreeReader
    TTreeReader reader("evt", fNtuple);


    std::vector<TH1F*> hist_C;
    std::vector<TH1F*> hist_S;

    std::vector<TTreeReaderValue<std::vector<short>>*> wave_C_readers;
    std::vector<TTreeReaderValue<std::vector<short>>*> wave_S_readers;

    for ( int idx = 0; idx < 64; idx++ ) {
        std::string branchName_C = "wave_C" + std::to_string(idx + 1);
        std::string branchName_S = "wave_S" + std::to_string(idx + 1);
        wave_C_readers.push_back(new TTreeReaderValue<std::vector<short>>(reader, branchName_C.c_str()));
        wave_S_readers.push_back(new TTreeReaderValue<std::vector<short>>(reader, branchName_S.c_str()));
        hist_C.push_back(new TH1F((TString)(branchName_C), ";ADC;a.u.", 100,-50,50));
        hist_S.push_back(new TH1F((TString)(branchName_S), ";ADC;a.u.", 100,-50,50));
    }
    // Create TTreeReaderValue for all waveform branches
    
    // Set Maximum event
    Long64_t totalEntries = reader.GetEntries();
    if (fMaxEvent == -1 || fMaxEvent > totalEntries)
        fMaxEvent = totalEntries;
    
    for (int iEvt = 0; iEvt < fMaxEvent; iEvt++) {
        printProgress(iEvt, fMaxEvent);
        // Load event using TTreeReader
        reader.SetEntry(iEvt);
        
        
        
        // Event selection here!!
        // Require C1, C2 to have intADC larger than cut
        // if ( !((cut_CC1 < signal_CC1) && (cut_CC2 < signal_CC2)) ) continue;
        
        // Do whatever need to be done after PID
        
        for ( int idx = 0; idx < 64; idx++ ) {
            auto wave_C = *(*wave_C_readers.at(idx));
            //float signal_C = GetInt(wave_C, C_first, C_last);
	    auto wave_C_pedcor = pedcorwave(wave_C,100);

            auto wave_S = *(*wave_S_readers.at(idx));
	    auto wave_S_pedcor = pedcorwave(wave_S,100);
            //float signal_S = GetInt(wave_S, S_first, S_last);
            for (int i =1;i<1001;i++){
		    hist_C.at(idx)->Fill(wave_C_pedcor.at(i));
		    hist_S.at(idx)->Fill(wave_S_pedcor.at(i));
	    }
        }

    }
    
    
    // Output file
    std::string outFile = "./MCP_ntuple/MCP_Run_" + std::to_string(fRunNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();
    
    for (int idx = 0; idx < 64; idx++){
        hist_C.at(idx)->Write();
        hist_S.at(idx)->Write();
    }
    
    outputRoot->Close();
    
    return 0;
}
