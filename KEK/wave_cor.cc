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

    bool isKEK = true;
    if (argc > 3)
        isKEK = (std::stoi(argv[3]) != 0);
    fs::path dir("./wave_Cor");   
    if (!(fs::exists(dir))) fs::create_directory(dir);
    std::string outFile = "./wave_Cor/wave_Cor_Run_" + std::to_string(fRunNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    
    const std::string mappingPath = isKEK ? "../mapping/mapping_KEK.root" : "../mapping/mapping_TB2025_v1.root";
    const std::string correctionCSVPath = isKEK ? "../kek_means.csv" : "../th2d_means.csv";
    const std::string dataPath = isKEK ? "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_KEK_TB_Data"
                                       : "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_Data";

    // initialize the utility class
    TButility util = TButility();
    //util.LoadMapping("../mapping/mapping_KEK.root");
    util.LoadMapping(mappingPath);
    TBwaveform::SetCorrectionCSVPath(correctionCSVPath);

    TBcid cid_T2_S = util.GetCID("T2-S");

    TH2D* wave_uncor = new TH2D("wave_uncor","",1000,0,1000,200,-100,100);
    TH2D* wave_cor = new TH2D("wave_cor","",1000,0,1000,200,-100,100);


    // MID: 3-7: PMT modules, MID 9: LC, MID 10: Aux(CC1, CC2, PS, TC, MC), MID 12: Triggers (T1, T2, T1NIM, T2NIM, Coin), MID 14-17: MCP micro, MID 18: DWC
    // TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, "/Volumes/Macintosh HD-1/Users/yhep/scratch/YUdaq", {3, 4, 5, 6, 7, 9, 10, 12, 18});
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, dataPath, {8, 9, 13});
    // Set Maximum event
    if (fMaxEvent == -1)
      fMaxEvent = readerWave.GetMaxEvent();
  
    if (fMaxEvent > readerWave.GetMaxEvent())
      fMaxEvent = readerWave.GetMaxEvent();
  

    // Evt Loop

    for (int iEvt = 0; iEvt < fMaxEvent; iEvt++) {
	printProgress(iEvt, fMaxEvent);
	TBevt<TBwaveform> aEvent = readerWave.GetAnEvent();

	TBwaveform T2_S_wave = aEvent.GetData(cid_T2_S);

	std::vector<float> waveuncor = T2_S_wave.pedcorrectedWaveform();	
	std::vector<float> wavecor = T2_S_wave.ADCpedcorrectedWaveform();	

	for (int i =1; i < 1001; i++){
	  //std::cout<<" wave value : " <<waveuncor.at(i)<<" | "<<wavecor.at(i)<<std::endl;
	  wave_uncor->Fill(i-1,waveuncor.at(i));
	  wave_cor->Fill(i-1,wavecor.at(i));
	}


    }
    outputRoot->cd();
    
    wave_uncor->Write();
    wave_cor->Write();

    outputRoot->Close();

}

