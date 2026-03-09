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

    
    
    fs::path dir("./test");   
    if (!(fs::exists(dir))) fs::create_directory(dir);
    std::string outFile = "./test/drs_stop_Run_" + std::to_string(fRunNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");

    // initialize the utility class
    TButility util = TButility();
    util.LoadMapping("../mapping/mapping_KEK.root");

    std::vector<TBcid> S_collector;
    std::vector<TBcid> C_collector;
    TH2D *S_wave_hist[9];
    TH2D *C_wave_hist[9];
    TH2D *S_hist[9];
    TH2D *C_hist[9];
    TH2D *S_00_hist[9];
    TH2D *C_00_hist[9];
    TH2D *S_01_hist[9];
    TH2D *C_01_hist[9];
    TH2D *S_02_hist[9];
    TH2D *C_02_hist[9];
    TH2D *S_03_hist[9];
    TH2D *C_03_hist[9];
    TH2D *S_04_hist[9];
    TH2D *C_04_hist[9];
    TH2D *S_05_hist[9];
    TH2D *C_05_hist[9];
    TH2D *S_06_hist[9];
    TH2D *C_06_hist[9];
    TH2D *S_07_hist[9];
    TH2D *C_07_hist[9];
    TH2D *S_08_hist[9];
    TH2D *C_08_hist[9];
    TH2D *S_09_hist[9];
    TH2D *C_09_hist[9];
    TH2D *S_10_hist[9];
    TH2D *C_10_hist[9];
    for (int i=0; i< 9; i++){
      TBcid cid_tmp_S = util.GetCID(Form("T%d-S",i + 1));
      TBcid cid_tmp_C = util.GetCID(Form("T%d-C",i + 1));
      S_collector.push_back(cid_tmp_S);
      C_collector.push_back(cid_tmp_C);
      S_wave_hist[i] = new TH2D(Form("T%d_S_wave",i + 1),"",1024,0,1024,4096,0,4096);
      C_wave_hist[i] = new TH2D(Form("T%d_C_wave",i + 1),"",1024,0,1024,4096,0,4096);
      S_hist[i] = new TH2D(Form("T%d_S",i + 1),"",1024,0,1024,4096,0,4096);
      C_hist[i] = new TH2D(Form("T%d_C",i + 1),"",1024,0,1024,4096,0,4096);
      S_00_hist[i] = new TH2D(Form("T%d_S_00",i + 1),"",1024,0,1024,200,-100,100);
      C_00_hist[i] = new TH2D(Form("T%d_C_00",i + 1),"",1024,0,1024,200,-100,100);
      S_01_hist[i] = new TH2D(Form("T%d_S_01",i + 1),"",1024,0,1024,200,-100,100);
      C_01_hist[i] = new TH2D(Form("T%d_C_01",i + 1),"",1024,0,1024,200,-100,100);
      S_02_hist[i] = new TH2D(Form("T%d_S_02",i + 1),"",1024,0,1024,200,-100,100);
      C_02_hist[i] = new TH2D(Form("T%d_C_02",i + 1),"",1024,0,1024,200,-100,100);
      S_03_hist[i] = new TH2D(Form("T%d_S_03",i + 1),"",1024,0,1024,200,-100,100);
      C_03_hist[i] = new TH2D(Form("T%d_C_03",i + 1),"",1024,0,1024,200,-100,100);
      S_04_hist[i] = new TH2D(Form("T%d_S_04",i + 1),"",1024,0,1024,200,-100,100);
      C_04_hist[i] = new TH2D(Form("T%d_C_04",i + 1),"",1024,0,1024,200,-100,100);
      S_05_hist[i] = new TH2D(Form("T%d_S_05",i + 1),"",1024,0,1024,200,-100,100);
      C_05_hist[i] = new TH2D(Form("T%d_C_05",i + 1),"",1024,0,1024,200,-100,100);
      S_06_hist[i] = new TH2D(Form("T%d_S_06",i + 1),"",1024,0,1024,200,-100,100);
      C_06_hist[i] = new TH2D(Form("T%d_C_06",i + 1),"",1024,0,1024,200,-100,100);
      S_07_hist[i] = new TH2D(Form("T%d_S_07",i + 1),"",1024,0,1024,200,-100,100);
      C_07_hist[i] = new TH2D(Form("T%d_C_07",i + 1),"",1024,0,1024,200,-100,100);
      S_08_hist[i] = new TH2D(Form("T%d_S_08",i + 1),"",1024,0,1024,200,-100,100);
      C_08_hist[i] = new TH2D(Form("T%d_C_08",i + 1),"",1024,0,1024,200,-100,100);
      S_09_hist[i] = new TH2D(Form("T%d_S_09",i + 1),"",1024,0,1024,200,-100,100);
      C_09_hist[i] = new TH2D(Form("T%d_C_09",i + 1),"",1024,0,1024,200,-100,100);
      S_10_hist[i] = new TH2D(Form("T%d_S_10",i + 1),"",1024,0,1024,200,-100,100);
      C_10_hist[i] = new TH2D(Form("T%d_C_10",i + 1),"",1024,0,1024,200,-100,100);
    }
    

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

	for (int i=0;i<9;i++){
	  TBwaveform S_tmp = aEvent.GetData(S_collector.at(i));
	  TBwaveform C_tmp = aEvent.GetData(C_collector.at(i));

	  int drs_stop_S = S_tmp.drs_stop();
	  int drs_stop_C = C_tmp.drs_stop();

	  std::vector<short> waveform_S = S_tmp.waveform();
	  std::vector<short> waveform_C = C_tmp.waveform();

	  int p00_S, p00_C;
	  int p01_S, p01_C;
	  int p02_S, p02_C;
	  int p03_S, p03_C;
	  int p04_S, p04_C;
	  int p05_S, p05_C;
	  int p06_S, p06_C;
	  int p07_S, p07_C;
	  int p08_S, p08_C;
	  int p09_S, p09_C;
	  int p10_S, p10_C;
	  int point[11] = {1,94,187,280,373,466,560,653,746,839,932};
	  p00_S = 1024 - drs_stop_S;
	  p00_C = 1024 - drs_stop_C;
	  
	  if (drs_stop_S > point[1]) p01_S = 1024 + point[1] - drs_stop_S;
	  else p01_S = point[1] - drs_stop_S;
	  if (drs_stop_C > point[1]) p01_C = 1024 + point[1] - drs_stop_C;
	  else p01_C = point[1] - drs_stop_C;

	  if (drs_stop_S > point[2]) p02_S = 1024 + point[2] - drs_stop_S;
	  else p02_S = point[2] - drs_stop_S;               
	  if (drs_stop_C > point[2]) p02_C = 1024 + point[2] - drs_stop_C;
	  else p02_C = point[2] - drs_stop_C;
	  
	  if (drs_stop_S > point[3]) p03_S = 1024 + point[3] - drs_stop_S;
	  else p03_S = point[3] - drs_stop_S;               
	  if (drs_stop_C > point[3]) p03_C = 1024 + point[3] - drs_stop_C;
	  else p03_C = point[3] - drs_stop_C;
	  
	  if (drs_stop_S > point[4]) p04_S = 1024 + point[4] - drs_stop_S;
	  else p04_S = point[4] - drs_stop_S;               
	  if (drs_stop_C > point[4]) p04_C = 1024 + point[4] - drs_stop_C;
	  else p04_C = point[4] - drs_stop_C;
	  
	  if (drs_stop_S > point[5]) p05_S = 1024 + point[5] - drs_stop_S;
	  else p05_S = point[5] - drs_stop_S;               
	  if (drs_stop_C > point[5]) p05_C = 1024 + point[5] - drs_stop_C;
	  else p05_C = point[5] - drs_stop_C;
	  
	  if (drs_stop_S > point[6]) p06_S = 1024 + point[6] - drs_stop_S;
	  else p06_S = point[6] - drs_stop_S;               
	  if (drs_stop_C > point[6]) p06_C = 1024 + point[6] - drs_stop_C;
	  else p06_C = point[6] - drs_stop_C;
	  
	  if (drs_stop_S > point[7]) p07_S = 1024 + point[7] - drs_stop_S;
	  else p07_S = point[7] - drs_stop_S;               
	  if (drs_stop_C > point[7]) p07_C = 1024 + point[7] - drs_stop_C;
	  else p07_C = point[7] - drs_stop_C;
	  
	  if (drs_stop_S > point[8]) p08_S = 1024 + point[8] - drs_stop_S;
	  else p08_S = point[8] - drs_stop_S;               
	  if (drs_stop_C > point[8]) p08_C = 1024 + point[8] - drs_stop_C;
	  else p08_C = point[8] - drs_stop_C;
	  
	  if (drs_stop_S > point[9]) p09_S = 1024 + point[9] - drs_stop_S;
	  else p09_S = point[9] - drs_stop_S;               
	  if (drs_stop_C > point[9]) p09_C = 1024 + point[9] - drs_stop_C;
	  else p09_C = point[9] - drs_stop_C;
	  
	  if (drs_stop_S > point[10]) p10_S = 1024 + point[10] - drs_stop_S;
	  else p10_S = point[10] - drs_stop_S;               
	  if (drs_stop_C > point[10]) p10_C = 1024 + point[10] - drs_stop_C;
	  else p10_C = point[10] - drs_stop_C;
	  
	  for (int j = 1; j<301;j++){
	    int bin_S;
	    int bin_C;
	    if (j + drs_stop_S + 1<1024) bin_S = j + drs_stop_S + 1; 
	    else bin_S = j + drs_stop_S + 1 - 1024; 
	    if (j + drs_stop_C + 1<1024) bin_C = j + drs_stop_C + 1; 
	    else bin_C = j + drs_stop_C + 1 - 1024; 

	    S_wave_hist[i]->Fill(j + 1,waveform_S.at(j));
	    C_wave_hist[i]->Fill(j + 1,waveform_C.at(j));
	    S_hist[i]->Fill(bin_S,waveform_S.at(j));
	    C_hist[i]->Fill(bin_C,waveform_C.at(j));
	    if ( p00_S > 0 && p00_S < 1001 ) S_00_hist[i]->Fill(bin_S,mean_range(waveform_S,p00_S,p01_S)-waveform_S.at(j));
	    if ( p00_C > 0 && p00_C < 1001 ) C_00_hist[i]->Fill(bin_C,mean_range(waveform_C,p00_C,p01_C)-waveform_C.at(j));
	    if ( p01_S > 0 && p01_S < 1001 ) S_01_hist[i]->Fill(bin_S,mean_range(waveform_S,p01_S,p02_S)-waveform_S.at(j));
	    if ( p01_C > 0 && p01_C < 1001 ) C_01_hist[i]->Fill(bin_C,mean_range(waveform_C,p01_C,p02_C)-waveform_C.at(j));
	    if ( p02_S > 0 && p02_S < 1001 ) S_02_hist[i]->Fill(bin_S,mean_range(waveform_S,p02_S,p03_S)-waveform_S.at(j));
	    if ( p02_C > 0 && p02_C < 1001 ) C_02_hist[i]->Fill(bin_C,mean_range(waveform_C,p02_C,p03_C)-waveform_C.at(j));
	    if ( p03_S > 0 && p03_S < 1001 ) S_03_hist[i]->Fill(bin_S,mean_range(waveform_S,p03_S,p04_S)-waveform_S.at(j));
	    if ( p03_C > 0 && p03_C < 1001 ) C_03_hist[i]->Fill(bin_C,mean_range(waveform_C,p03_C,p04_C)-waveform_C.at(j));
	    if ( p04_S > 0 && p04_S < 1001 ) S_04_hist[i]->Fill(bin_S,mean_range(waveform_S,p04_S,p05_S)-waveform_S.at(j));
	    if ( p04_C > 0 && p04_C < 1001 ) C_04_hist[i]->Fill(bin_C,mean_range(waveform_C,p04_C,p05_C)-waveform_C.at(j));
	    if ( p05_S > 0 && p05_S < 1001 ) S_05_hist[i]->Fill(bin_S,mean_range(waveform_S,p05_S,p06_S)-waveform_S.at(j));
	    if ( p05_C > 0 && p05_C < 1001 ) C_05_hist[i]->Fill(bin_C,mean_range(waveform_C,p05_C,p06_C)-waveform_C.at(j));
	    if ( p06_S > 0 && p06_S < 1001 ) S_06_hist[i]->Fill(bin_S,mean_range(waveform_S,p06_S,p07_S)-waveform_S.at(j));
	    if ( p06_C > 0 && p06_C < 1001 ) C_06_hist[i]->Fill(bin_C,mean_range(waveform_C,p06_C,p07_C)-waveform_C.at(j));
	    if ( p07_S > 0 && p07_S < 1001 ) S_07_hist[i]->Fill(bin_S,mean_range(waveform_S,p07_S,p08_S)-waveform_S.at(j));
	    if ( p07_C > 0 && p07_C < 1001 ) C_07_hist[i]->Fill(bin_C,mean_range(waveform_C,p07_C,p08_C)-waveform_C.at(j));
	    if ( p08_S > 0 && p08_S < 1001 ) S_08_hist[i]->Fill(bin_S,mean_range(waveform_S,p08_S,p09_S)-waveform_S.at(j));
	    if ( p08_C > 0 && p08_C < 1001 ) C_08_hist[i]->Fill(bin_C,mean_range(waveform_C,p08_C,p09_C)-waveform_C.at(j));
	    if ( p09_S > 0 && p09_S < 1001 ) S_09_hist[i]->Fill(bin_S,mean_range(waveform_S,p09_S,p10_S)-waveform_S.at(j));
	    if ( p09_C > 0 && p09_C < 1001 ) C_09_hist[i]->Fill(bin_C,mean_range(waveform_C,p09_C,p10_C)-waveform_C.at(j));
	    if ( p10_S > 0 && p10_S < 1001 ) S_10_hist[i]->Fill(bin_S,mean_range(waveform_S,p10_S,p00_S)-waveform_S.at(j));
	    if ( p10_C > 0 && p10_C < 1001 ) C_10_hist[i]->Fill(bin_C,mean_range(waveform_C,p10_C,p00_C)-waveform_C.at(j));
	  }
	}

	
	//TBwaveform wave_M1_T1_S = aEvent.GetData(cid_M1_T1_S);
	//int drs_stop = wave_M1_T1_S.drs_stop();
	//std::cout<<"test : "<<iEvt<<" | "<<drs_stop<<std::endl;

    }
    outputRoot->cd();
    
    for (int i=0; i< 9; i++){
      S_wave_hist[i]->Write();
      C_wave_hist[i]->Write();
      S_hist[i]->Write();
      C_hist[i]->Write();
      S_00_hist[i]->Write();
      C_00_hist[i]->Write();
      S_01_hist[i]->Write();
      C_01_hist[i]->Write();
      S_02_hist[i]->Write();
      C_02_hist[i]->Write();
      S_03_hist[i]->Write();
      C_03_hist[i]->Write();
      S_04_hist[i]->Write();
      C_04_hist[i]->Write();
      S_05_hist[i]->Write();
      C_05_hist[i]->Write();
      S_06_hist[i]->Write();
      C_06_hist[i]->Write();
      S_07_hist[i]->Write();
      C_07_hist[i]->Write();
      S_08_hist[i]->Write();
      C_08_hist[i]->Write();
      S_09_hist[i]->Write();
      C_09_hist[i]->Write();
      S_10_hist[i]->Write();
      C_10_hist[i]->Write();
    }
    outputRoot->Close();

}

