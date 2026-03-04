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
    
    int fRunNum = std::stoi(argv[1]);
    int fMaxEvent = std::stoi(argv[2]);
    int fMaxFile = -1;

    fs::path dir("./test");   
    if (!(fs::exists(dir))) fs::create_directory(dir);
    std::string outFile = "./test/drs_stop_Run_" + std::to_string(fRunNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    
    TFile* f_DWC = TFile::Open((TString)("./DWC/DWC_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    TH2D* h_DWC1_pos   = (TH2D*) f_DWC->Get("dwc1_pos");
    TH2D* h_DWC2_pos   = (TH2D*) f_DWC->Get("dwc2_pos");
    std::vector<float> DWC1_offset = getDWCoffset(h_DWC1_pos); // DWC1_offset.at(0) == X, DWC1_offset.at(1) == Y
    std::vector<float> DWC2_offset = getDWCoffset(h_DWC2_pos);
    
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

    // initialize the utility class
    TButility util = TButility();
    util.LoadMapping("../mapping/mapping_TB2025_v1.root");
    TBcid cid_CC1 = util.GetCID("CC1");
    TBcid cid_CC2 = util.GetCID("CC2");
    TBcid cid_PS = util.GetCID("PS");
    TBcid cid_MC = util.GetCID("MC");
    TBcid cid_TC = util.GetCID("TC");

    TBcid cid_DWC1_L = util.GetCID("")

    std::vector<TBcid> S_collector;
    std::vector<TBcid> C_collector;
    TH2D *S_hist[36];
    TH2D *C_hist[36];
    TH2D *S_00_hist[36];
    TH2D *C_00_hist[36];
    TH2D *S_01_hist[36];
    TH2D *C_01_hist[36];
    TH2D *S_02_hist[36];
    TH2D *C_02_hist[36];
    TH2D *S_03_hist[36];
    TH2D *C_03_hist[36];
    TH2D *S_04_hist[36];
    TH2D *C_04_hist[36];
    TH2D *S_05_hist[36];
    TH2D *C_05_hist[36];
    TH2D *S_06_hist[36];
    TH2D *C_06_hist[36];
    TH2D *S_07_hist[36];
    TH2D *C_07_hist[36];
    TH2D *S_08_hist[36];
    TH2D *C_08_hist[36];
    TH2D *S_09_hist[36];
    TH2D *C_09_hist[36];
    TH2D *S_10_hist[36];
    TH2D *C_10_hist[36];
    for (int i=0; i< 36; i++){
      TBcid cid_tmp_S = util.GetCID(Form("M%d-T%d-S",i%9 + 1,i/9 + 1));
      TBcid cid_tmp_C = util.GetCID(Form("M%d-T%d-C",i%9 + 1,i/9 + 1));
      S_collector.push_back(cid_tmp_S);
      C_collector.push_back(cid_tmp_C);
      S_hist[i] = new TH2D(Form("M%d_T%d_S",i%9 + 1,i/9 + 1),"",1024,0,1024,4096,0,4096);
      C_hist[i] = new TH2D(Form("M%d_T%d_C",i%9 + 1,i/9 + 1),"",1024,0,1024,4096,0,4096);
      S_00_hist[i] = new TH2D(Form("M%d_T%d_S_00",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      C_00_hist[i] = new TH2D(Form("M%d_T%d_C_00",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      S_01_hist[i] = new TH2D(Form("M%d_T%d_S_01",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      C_01_hist[i] = new TH2D(Form("M%d_T%d_C_01",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      S_02_hist[i] = new TH2D(Form("M%d_T%d_S_02",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      C_02_hist[i] = new TH2D(Form("M%d_T%d_C_02",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      S_03_hist[i] = new TH2D(Form("M%d_T%d_S_03",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      C_03_hist[i] = new TH2D(Form("M%d_T%d_C_03",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      S_04_hist[i] = new TH2D(Form("M%d_T%d_S_04",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      C_04_hist[i] = new TH2D(Form("M%d_T%d_C_04",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      S_05_hist[i] = new TH2D(Form("M%d_T%d_S_05",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      C_05_hist[i] = new TH2D(Form("M%d_T%d_C_05",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      S_06_hist[i] = new TH2D(Form("M%d_T%d_S_06",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      C_06_hist[i] = new TH2D(Form("M%d_T%d_C_06",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      S_07_hist[i] = new TH2D(Form("M%d_T%d_S_07",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      C_07_hist[i] = new TH2D(Form("M%d_T%d_C_07",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      S_08_hist[i] = new TH2D(Form("M%d_T%d_S_08",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      C_08_hist[i] = new TH2D(Form("M%d_T%d_C_08",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      S_09_hist[i] = new TH2D(Form("M%d_T%d_S_09",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      C_09_hist[i] = new TH2D(Form("M%d_T%d_C_09",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      S_10_hist[i] = new TH2D(Form("M%d_T%d_S_10",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
      C_10_hist[i] = new TH2D(Form("M%d_T%d_C_10",i%9 + 1,i/9 + 1),"",1024,0,1024,200,-100,100);
    }
    

    // MID: 3-7: PMT modules, MID 9: LC, MID 10: Aux(CC1, CC2, PS, TC, MC), MID 12: Triggers (T1, T2, T1NIM, T2NIM, Coin), MID 14-17: MCP micro, MID 18: DWC
    // TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, "/Volumes/Macintosh HD-1/Users/yhep/scratch/YUdaq", {3, 4, 5, 6, 7, 9, 10, 12, 18});
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, false, "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_Data/", {3, 4, 5, 6, 7, 10, 12});
    // Set Maximum event
    if (fMaxEvent == -1)
      fMaxEvent = readerWave.GetMaxEvent();
  
    if (fMaxEvent > readerWave.GetMaxEvent())
      fMaxEvent = readerWave.GetMaxEvent();
  

    // Evt Loop

    for (int iEvt = 0; iEvt < fMaxEvent; iEvt++) {
	printProgress(iEvt, fMaxEvent);
	TBevt<TBwaveform> aEvent = readerWave.GetAnEvent();
	for (int i=0;i<36;i++){
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
	  p00_S = 1024 - drs_stop_S;
	  p00_C = 1024 - drs_stop_C;
	  
	  if (drs_stop_S > 93) p01_S = 1024 + 93 - drs_stop_S;
	  else p01_S = 93 - drs_stop_S;
	  if (drs_stop_C > 93) p01_C = 1024 + 93 - drs_stop_C;
	  else p01_C = 93 - drs_stop_C;

	  if (drs_stop_S > 187) p02_S = 1024 + 187 - drs_stop_S;
	  else p02_S = 187 - drs_stop_S;
	  if (drs_stop_C > 187) p02_C = 1024 + 187 - drs_stop_C;
	  else p02_C = 187 - drs_stop_C;
	  
	  if (drs_stop_S > 280) p03_S = 1024 + 280 - drs_stop_S;
	  else p03_S = 280 - drs_stop_S;
	  if (drs_stop_C > 280) p03_C = 1024 + 280 - drs_stop_C;
	  else p03_C = 280 - drs_stop_C;
	  
	  if (drs_stop_S > 373) p04_S = 1024 + 373 - drs_stop_S;
	  else p04_S = 373 - drs_stop_S;
	  if (drs_stop_C > 373) p04_C = 1024 + 373 - drs_stop_C;
	  else p04_C = 373 - drs_stop_C;
	  
	  if (drs_stop_S > 466) p05_S = 1024 + 466 - drs_stop_S;
	  else p05_S = 466 - drs_stop_S;
	  if (drs_stop_C > 466) p05_C = 1024 + 466 - drs_stop_C;
	  else p05_C = 466 - drs_stop_C;
	  
	  if (drs_stop_S > 560) p06_S = 1024 + 560 - drs_stop_S;
	  else p06_S = 560 - drs_stop_S;
	  if (drs_stop_C > 560) p06_C = 1024 + 560 - drs_stop_C;
	  else p06_C = 560 - drs_stop_C;
	  
	  if (drs_stop_S > 653) p07_S = 1024 + 653 - drs_stop_S;
	  else p07_S = 653 - drs_stop_S;
	  if (drs_stop_C > 653) p07_C = 1024 + 653 - drs_stop_C;
	  else p07_C = 653 - drs_stop_C;
	  
	  if (drs_stop_S > 746) p08_S = 1024 + 746 - drs_stop_S;
	  else p08_S = 746 - drs_stop_S;
	  if (drs_stop_C > 746) p08_C = 1024 + 746 - drs_stop_C;
	  else p08_C = 746 - drs_stop_C;
	  
	  if (drs_stop_S > 839) p09_S = 1024 + 839 - drs_stop_S;
	  else p09_S = 839 - drs_stop_S;
	  if (drs_stop_C > 839) p09_C = 1024 + 839 - drs_stop_C;
	  else p09_C = 839 - drs_stop_C;
	  
	  if (drs_stop_S > 932) p10_S = 1024 + 932 - drs_stop_S;
	  else p10_S = 932 - drs_stop_S;
	  if (drs_stop_C > 932) p10_C = 1024 + 932 - drs_stop_C;
	  else p10_C = 932 - drs_stop_C;
	  
	  for (int j = 1; j<1001;j++){
	    int bin_S;
	    int bin_C;
	    if (j + drs_stop_S + 1<1024) bin_S = j + drs_stop_S + 1; 
	    else bin_S = j + drs_stop_S + 1 - 1024; 
	    if (j + drs_stop_C + 1<1024) bin_C = j + drs_stop_C + 1; 
	    else bin_C = j + drs_stop_C + 1 - 1024; 

	    S_hist[i]->Fill(bin_S,waveform_S.at(j));
	    C_hist[i]->Fill(bin_C,waveform_C.at(j));
	    if ( p00_S > 0 && p00_S < 1001 ) S_00_hist[i]->Fill(bin_S,waveform_S.at(p00_S)-waveform_S.at(j));
	    if ( p00_C > 0 && p00_C < 1001 ) C_00_hist[i]->Fill(bin_C,waveform_C.at(p00_C)-waveform_C.at(j));
	    if ( p01_S > 0 && p01_S < 1001 ) S_01_hist[i]->Fill(bin_S,waveform_S.at(p01_S)-waveform_S.at(j));
	    if ( p01_C > 0 && p01_C < 1001 ) C_01_hist[i]->Fill(bin_C,waveform_C.at(p01_C)-waveform_C.at(j));
	    if ( p02_S > 0 && p02_S < 1001 ) S_02_hist[i]->Fill(bin_S,waveform_S.at(p02_S)-waveform_S.at(j));
	    if ( p02_C > 0 && p02_C < 1001 ) C_02_hist[i]->Fill(bin_C,waveform_C.at(p02_C)-waveform_C.at(j));
	    if ( p03_S > 0 && p03_S < 1001 ) S_03_hist[i]->Fill(bin_S,waveform_S.at(p03_S)-waveform_S.at(j));
	    if ( p03_C > 0 && p03_C < 1001 ) C_03_hist[i]->Fill(bin_C,waveform_C.at(p03_C)-waveform_C.at(j));
	    if ( p04_S > 0 && p04_S < 1001 ) S_04_hist[i]->Fill(bin_S,waveform_S.at(p04_S)-waveform_S.at(j));
	    if ( p04_C > 0 && p04_C < 1001 ) C_04_hist[i]->Fill(bin_C,waveform_C.at(p04_C)-waveform_C.at(j));
	    if ( p05_S > 0 && p05_S < 1001 ) S_05_hist[i]->Fill(bin_S,waveform_S.at(p05_S)-waveform_S.at(j));
	    if ( p05_C > 0 && p05_C < 1001 ) C_05_hist[i]->Fill(bin_C,waveform_C.at(p05_C)-waveform_C.at(j));
	    if ( p06_S > 0 && p06_S < 1001 ) S_06_hist[i]->Fill(bin_S,waveform_S.at(p06_S)-waveform_S.at(j));
	    if ( p06_C > 0 && p06_C < 1001 ) C_06_hist[i]->Fill(bin_C,waveform_C.at(p06_C)-waveform_C.at(j));
	    if ( p07_S > 0 && p07_S < 1001 ) S_07_hist[i]->Fill(bin_S,waveform_S.at(p07_S)-waveform_S.at(j));
	    if ( p07_C > 0 && p07_C < 1001 ) C_07_hist[i]->Fill(bin_C,waveform_C.at(p07_C)-waveform_C.at(j));
	    if ( p08_S > 0 && p08_S < 1001 ) S_08_hist[i]->Fill(bin_S,waveform_S.at(p08_S)-waveform_S.at(j));
	    if ( p08_C > 0 && p08_C < 1001 ) C_08_hist[i]->Fill(bin_C,waveform_C.at(p08_C)-waveform_C.at(j));
	    if ( p09_S > 0 && p09_S < 1001 ) S_09_hist[i]->Fill(bin_S,waveform_S.at(p09_S)-waveform_S.at(j));
	    if ( p09_C > 0 && p09_C < 1001 ) C_09_hist[i]->Fill(bin_C,waveform_C.at(p09_C)-waveform_C.at(j));
	    if ( p10_S > 0 && p10_S < 1001 ) S_10_hist[i]->Fill(bin_S,waveform_S.at(p10_S)-waveform_S.at(j));
	    if ( p10_C > 0 && p10_C < 1001 ) C_10_hist[i]->Fill(bin_C,waveform_C.at(p10_C)-waveform_C.at(j));
	  }
	}

	
	//TBwaveform wave_M1_T1_S = aEvent.GetData(cid_M1_T1_S);
	//int drs_stop = wave_M1_T1_S.drs_stop();
	//std::cout<<"test : "<<iEvt<<" | "<<drs_stop<<std::endl;

    }

    for (int i=0; i< 36; i++){
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

