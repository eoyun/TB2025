#include "TBread.h"
#include "TButility.h"

#include <vector>
#include <iostream>
#include <filesystem>
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
#include "TH2.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "function.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    int fRunNum = std::stoi(argv[1]);
    int fMaxEvent = std::stoi(argv[2]);
    int fMaxFile  = -1;

    fs::path dir("./DWC");   
    if (!(fs::exists(dir))) fs::create_directory(dir);

    // Load mapping
    TButility util = TButility();
    util.LoadMapping("../mapping/mapping_TB2025_v1.root");

    TFile* fNtuple = TFile::Open((TString)("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_PromptAnalysis/Prompt_ntuple_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    
    // Create TTreeReader
    TTreeReader reader("evt", fNtuple);
    
    // Create TTreeReaderValue for each DWC branch
    TTreeReaderValue<std::vector<short>> wave_DWC1_R(reader, "wave_DWC1_R");
    TTreeReaderValue<std::vector<short>> wave_DWC1_L(reader, "wave_DWC1_L");
    TTreeReaderValue<std::vector<short>> wave_DWC1_U(reader, "wave_DWC1_U");
    TTreeReaderValue<std::vector<short>> wave_DWC1_D(reader, "wave_DWC1_D");
    TTreeReaderValue<std::vector<short>> wave_DWC2_R(reader, "wave_DWC2_R");
    TTreeReaderValue<std::vector<short>> wave_DWC2_L(reader, "wave_DWC2_L");
    TTreeReaderValue<std::vector<short>> wave_DWC2_U(reader, "wave_DWC2_U");
    TTreeReaderValue<std::vector<short>> wave_DWC2_D(reader, "wave_DWC2_D");

    // Preparing 2D histograms for DWC 1 & 2 position, correlation plots
    TH2D* dwc1_pos = new TH2D("dwc1_pos", "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc2_pos = new TH2D("dwc2_pos", "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc_x_corr = new TH2D("dwc_x_corr", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* dwc_y_corr = new TH2D("dwc_y_corr", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);
    // Set Maximum event
    Long64_t totalEntries = reader.GetEntries();
    if (fMaxEvent == -1 || fMaxEvent > totalEntries)
        fMaxEvent = totalEntries;

    // Start event loop using TTreeReader with for loop
    for (int iEvt = 0; iEvt < fMaxEvent; iEvt++) {
        reader.SetEntry(iEvt);

        // Get DWC 1 R, L, U, D peak timing from their waveforms
        float time_DWC1_R = getLeadingEdgeTime_interpolated800(*wave_DWC1_R, 0.4, 1, 1000);
        float time_DWC1_L = getLeadingEdgeTime_interpolated800(*wave_DWC1_L, 0.4, 1, 1000);
        float time_DWC1_U = getLeadingEdgeTime_interpolated800(*wave_DWC1_U, 0.4, 1, 1000);
        float time_DWC1_D = getLeadingEdgeTime_interpolated800(*wave_DWC1_D, 0.4, 1, 1000);

        // Get DWC2 R, L, U, D peak timing from their waveforms
        float time_DWC2_R = getLeadingEdgeTime_interpolated800(*wave_DWC2_R, 0.4, 1, 1000);
        float time_DWC2_L = getLeadingEdgeTime_interpolated800(*wave_DWC2_L, 0.4, 1, 1000);
        float time_DWC2_U = getLeadingEdgeTime_interpolated800(*wave_DWC2_U, 0.4, 1, 1000);
        float time_DWC2_D = getLeadingEdgeTime_interpolated800(*wave_DWC2_D, 0.4, 1, 1000);


        // TB2025 result (2800V, leading edge 40%)
        float dwc1_horizontal_Slope  = 0.180654; // This is calib. const. for upstream -> downstream view
        float dwc1_horizontal_Offset = 0.217961; // This is calib. const. for upstream -> downstream view
        float dwc1_Vertical_Slope    = -0.180342;    // This is calib. const. for upstream -> downstream view
        float dwc1_Vertical_Offset   = -0.0994697;  // This is calib. const. for upstream -> downstream view
        // float dwc1_horizontal_Slope_downstream = -0.180654;
        // float dwc1_horizontal_Offset_downstream = -0.217961;
                
        float dwc1_x_position = ( (float) (time_DWC1_R - time_DWC1_L ) * dwc1_horizontal_Slope) + dwc1_horizontal_Offset;
        float dwc1_y_position = ( (float) (time_DWC1_U - time_DWC1_D ) * dwc1_Vertical_Slope)   + dwc1_Vertical_Offset;

        // // TB2025 result (2800V, leading edge 40%)
        float dwc2_horizontal_Slope  = 0.181416; // This is calib. const. for upstream -> downstream view
        float dwc2_horizontal_Offset = -0.00911072; // This is calib. const. for upstream -> downstream view
        float dwc2_Vertical_Slope    = -0.17822;    // This is calib. const. for upstream -> downstream view
        float dwc2_Vertical_Offset   = -0.0489771;  // This is calib. const. for upstream -> downstream view
        // float dwc2_horizontal_Slope_downstream = -0.181416;
        // float dwc2_horizontal_Offset_downstream = 0.00911072;


        float dwc2_x_position = ( (float) (time_DWC2_R - time_DWC2_L) * dwc2_horizontal_Slope) + dwc2_horizontal_Offset;
        float dwc2_y_position = ( (float) (time_DWC2_U - time_DWC2_D) * dwc2_Vertical_Slope)   + dwc2_Vertical_Offset;

        // std::cout << "dwc2_x_position : " << dwc2_x_position << std::endl;
        // std::cout << "dwc2_y_position : " << dwc2_y_position << std::endl;

        // Filling 2D histogram to plot DWC 1 and 2 positions
        dwc1_pos->Fill(dwc1_x_position, dwc1_y_position);
        dwc2_pos->Fill(dwc2_x_position, dwc2_y_position); 
        // Filling DWC 1 and 2 X positions to plot DWC 1 & 2 x-position correlation plot
        dwc_x_corr->Fill(dwc1_x_position, dwc2_x_position);
        dwc_y_corr->Fill(dwc1_y_position, dwc2_y_position);

        printProgress(iEvt + 1, fMaxEvent);
    }

    // Saving the DWC position & correlation plots in root file in ./dwc directory.
    std::string outFile = "./DWC/DWC_Run_" + std::to_string(fRunNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();

    dwc1_pos->Write();
    dwc2_pos->Write();
    dwc_x_corr->Write();
    dwc_y_corr->Write();

    outputRoot->Close();
}
