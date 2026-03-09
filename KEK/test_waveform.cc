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

double mean_range(std::vector<short> waveform, int p00, int p01)
{
    int sum = 0;
    int count = 0;
    if (p00 > 1023) p00 = p00 - 1024;
    if (p01 > 1023) p01 = p01 - 1024;

    int idx = p00;
    int test =0;
    while (1) {
        test ++;
        if (idx >= 1 && idx <= 1000) {
            sum += waveform.at(idx);
            count++;
        }
        if (test % 200 == 0) std::cout<<"infinity loop : "<<test <<" : "<< idx<< " | p00 "<<p00<<" | p01 "<<p01<<" | "<<waveform.size() <<std::endl;
        if (idx == p01) break;

        idx++;
        if (idx == 1024) idx = 0;  // circular wrap
    }

    if (count == 0) return 0;

    return (double) sum / count;
}

int main(int argc, char** argv) {

    
    int fRunNum = std::stoi(argv[1]);
    int fMaxEvent = std::stoi(argv[2]);
    int fMaxFile = -1;

    
    
    fs::path dir("./wave");   
    if (!(fs::exists(dir))) fs::create_directory(dir);
    std::string outFile = "./wave/drs_stop_Run_" + std::to_string(fRunNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");

    // initialize the utility class
    TButility util = TButility();
    util.LoadMapping("../mapping/mapping_KEK.root");

    std::vector<TBcid> cid_collector;
    TH2D *wave_hist[96];
    int mid_list[3] = {8, 9, 13};
    TH1D *drs_stop_hist[96];
    for (int i=0; i< 96; i++){
      TBcid cid_tmp = TBcid(mid_list[i/32] ,i%32 + 1);
      cid_collector.push_back(cid_tmp);
      wave_hist[i] = new TH2D(Form("M%d_C%d_wave",mid_list[i/32],i%32 + 1),"",1024,0,1024,4096,0,4096);
      drs_stop_hist[i] = new TH1D(Form("drs_stop_M%d_C%d",mid_list[i/32],i%32 + 1),"",1024,0,1024);
    }
    TBcid trig1_cid = util.GetCID("trg1");
    TBcid trig2_cid = util.GetCID("trg2");
    
    TH2D* trig1_hist = new TH2D("trig1","",1024,0,1024,4096,0,4096);
    TH2D* trig2_hist = new TH2D("trig2","",1024,0,1024,4096,0,4096);

    // MID: 3-7: PMT modules, MID 9: LC, MID 10: Aux(CC1, CC2, PS, TC, MC), MID 12: Triggers (T1, T2, T1NIM, T2NIM, Coin), MID 14-17: MCP micro, MID 18: DWC
    // TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, "/Volumes/Macintosh HD-1/Users/yhep/scratch/YUdaq", {3, 4, 5, 6, 7, 9, 10, 12, 18});
    //TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_Data/", {3, 4, 5, 6, 7, 10, 12, 18});
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_KEK_TB_Data", {8, 9, 13});

    // Set Maximum event
    if (fMaxEvent == -1)
      fMaxEvent = readerWave.GetMaxEvent();
  
    if (fMaxEvent > readerWave.GetMaxEvent())
      fMaxEvent = readerWave.GetMaxEvent();
  

    // Evt Loop
    

    for (int iEvt = 0; iEvt < fMaxEvent; iEvt++) {
	printProgress(iEvt, fMaxEvent);
	TBevt<TBwaveform> aEvent = readerWave.GetAnEvent();

	TBwaveform trig1_wave = aEvent.GetData(trig1_cid);
	TBwaveform trig2_wave = aEvent.GetData(trig2_cid);

	std::vector<short> trig1_waveform = trig1_wave.waveform();
	std::vector<short> trig2_waveform = trig2_wave.waveform();
	std::cout<<trig1_waveform.size()<<" : test"<<std::endl;
	for (int i=1; i<1001;i++){
	  trig1_hist->Fill(i+1,trig1_waveform.at(i));
	  trig2_hist->Fill(i+1,trig2_waveform.at(i));
	}
	for (int i=0;i<96;i++){
	  TBwaveform wave_tmp = aEvent.GetData(cid_collector.at(i));
	  int drs_stop = wave_tmp.drs_stop();
	  std::vector<short> waveform = wave_tmp.waveform();
	  
	  drs_stop_hist[i]->Fill(drs_stop);
	  for (int j = 1; j<1001;j++){

	    wave_hist[i]->Fill(j+1,waveform.at(j));
	  }
	}

	
	//TBwaveform wave_M1_T1_S = aEvent.GetData(cid_M1_T1_S);
	//int drs_stop = wave_M1_T1_S.drs_stop();
	//std::cout<<"test : "<<iEvt<<" | "<<drs_stop<<std::endl;

    }
    outputRoot->cd();
    trig1_hist->Write();
    trig2_hist->Write();
    for (int i=0; i< 96; i++){
      wave_hist[i]->Write();
      drs_stop_hist[i]->Write();
    }
    outputRoot->Close();

}

