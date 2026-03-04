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

    fs::path dir("./Avg");   
    if (!(fs::exists(dir))) fs::create_directory(dir);

    // initialize the utility class
    TButility util = TButility();
    util.LoadMapping("../mapping/mapping_TB2025_v1.root");
    
    TFile* fNtuple = TFile::Open((TString)("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_PromptAnalysis/Prompt_ntuple_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    TTreeReader reader("evt", fNtuple);

    // Create TTreeReaderValue for trigger waveform branches only
    TTreeReaderValue<std::vector<short>> wave_T1(reader, "wave_T1"); 
    TTreeReaderValue<std::vector<short>> wave_T2(reader, "wave_T2"); 
    TTreeReaderValue<std::vector<short>> wave_T1NIM(reader, "wave_T1NIM"); 
    TTreeReaderValue<std::vector<short>> wave_T2NIM(reader, "wave_T2NIM"); 
    TTreeReaderValue<std::vector<short>> wave_Coin(reader, "wave_Coin");
    TTreeReaderValue<std::vector<short>> wave_Coin_ref_1(reader, "wave_Coin_ref_1"); 
    TTreeReaderValue<std::vector<short>> wave_Coin_ref_2(reader, "wave_Coin_ref_2"); 
    TTreeReaderValue<std::vector<short>> wave_Coin_ref_3(reader, "wave_Coin_ref_3"); 
    TTreeReaderValue<std::vector<short>> wave_Coin_ref_4(reader, "wave_Coin_ref_4");

    // Prepare histograms for T1, T2, and Coin time structures
    TH1F* hist_T1 = new TH1F("T1", "T1 Average Time Structure;bin;Average ADC", 1000, 0, 1000);
    TH1F* hist_T2 = new TH1F("T2", "T2 Average Time Structure;bin;Average ADC", 1000, 0, 1000);
    TH1F* hist_T1NIM = new TH1F("T1NIM", "T1NIM Average Time Structure;bin;Average ADC", 1000, 0, 1000);
    TH1F* hist_T2NIM = new TH1F("T2NIM", "T2NIM Average Time Structure;bin;Average ADC", 1000, 0, 1000);
    TH1F* hist_Coin = new TH1F("Coin", "Coin Average Time Structure;bin;Average ADC", 1000, 0, 1000);
    TH1F* hist_Coin_ref_1 = new TH1F("Coin_ref_1", "Coin_ref_1 Average Time Structure;bin;Average ADC", 1000, 0, 1000);
    TH1F* hist_Coin_ref_2 = new TH1F("Coin_ref_2", "Coin_ref_2 Average Time Structure;bin;Average ADC", 1000, 0, 1000);
    TH1F* hist_Coin_ref_3 = new TH1F("Coin_ref_3", "Coin_ref_3 Average Time Structure;bin;Average ADC", 1000, 0, 1000);
    TH1F* hist_Coin_ref_4 = new TH1F("Coin_ref_4", "Coin_ref_4 Average Time Structure;bin;Average ADC", 1000, 0, 1000);

    // Set Maximum event
    Long64_t totalEntries = reader.GetEntries();
    if (fMaxEvent == -1 || fMaxEvent > totalEntries)
        fMaxEvent = totalEntries;

    std::cout << "Processing " << fMaxEvent << " events for average time structure analysis of T1, T2, and Coin signals..." << std::endl;

    // Event Loop
    for (int iEvt = 0; iEvt < fMaxEvent; iEvt++) {
        printProgress(iEvt, fMaxEvent);
        reader.SetEntry(iEvt);

        // Fill histograms with averaged waveform data
        // Each bin gets the sum of all events, then divided by total events to get average
        for (int bin = 0; bin < 1000; bin++) {      
            // Fill T1 and T2 trigger signals
            hist_T1->Fill(bin, (float) ((*wave_T1).at(bin)) / (float) (fMaxEvent) );
            hist_T2->Fill(bin, (float) ((*wave_T2).at(bin)) / (float) (fMaxEvent) );
            hist_T1NIM->Fill(bin, (float) ((*wave_T1NIM).at(bin)) / (float) (fMaxEvent) );
            hist_T2NIM->Fill(bin, (float) ((*wave_T2NIM).at(bin)) / (float) (fMaxEvent) );
            
            // Fill Coin and Coin reference signals
            hist_Coin->Fill(bin, (float) ((*wave_Coin).at(bin)) / (float) (fMaxEvent) );
            hist_Coin_ref_1->Fill(bin, (float) ((*wave_Coin_ref_1).at(bin)) / (float) (fMaxEvent) );
            hist_Coin_ref_2->Fill(bin, (float) ((*wave_Coin_ref_2).at(bin)) / (float) (fMaxEvent) );
            hist_Coin_ref_3->Fill(bin, (float) ((*wave_Coin_ref_3).at(bin)) / (float) (fMaxEvent) );
            hist_Coin_ref_4->Fill(bin, (float) ((*wave_Coin_ref_4).at(bin)) / (float) (fMaxEvent) );
        }
    }

    std::cout << std::endl << "Saving average time structure histograms..." << std::endl;

    // Save histograms to output file
    std::string outFile = "./Avg/AvgTimeStruc_T1T2Coin_Run_" + std::to_string(fRunNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();

    // Write trigger histograms
    hist_T1->Write();
    hist_T2->Write();
    hist_T1NIM->Write();
    hist_T2NIM->Write();
    
    // Write Coin histograms
    hist_Coin->Write();
    hist_Coin_ref_1->Write();
    hist_Coin_ref_2->Write();
    hist_Coin_ref_3->Write();
    hist_Coin_ref_4->Write();

    outputRoot->Close();
    
    std::cout << "Average time structure analysis complete!" << std::endl;
    std::cout << "Output saved to: " << outFile << std::endl;
    
    return 0;
}