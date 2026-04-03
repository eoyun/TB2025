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
    float cut_PS1 = 50.; // PID cut for PS (PeakADC)
    float cut_PS2 = 600.; // PID cut for PS (PeakADC)
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
    
    fs::path dir("./Waveform");   
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
    
    TBcid cid_CC1 = util.GetCID("CC1"); // Tail catcher
    TBcid cid_CC2 = util.GetCID("CC2"); // Tail catcher

    TBcid cid_DWC1_R = util.GetCID("DWC1R"); // DWC 1
    TBcid cid_DWC1_L = util.GetCID("DWC1L"); // DWC 1
    TBcid cid_DWC1_U = util.GetCID("DWC1U"); // DWC 1
    TBcid cid_DWC1_D = util.GetCID("DWC1D"); // DWC 1
    
    TBcid cid_DWC2_R = util.GetCID("DWC2R"); // DWC 2
    TBcid cid_DWC2_L = util.GetCID("DWC2L"); // DWC 2
    TBcid cid_DWC2_U = util.GetCID("DWC2U"); // DWC 2
    TBcid cid_DWC2_D = util.GetCID("DWC2D"); // DWC 2
    
    TH1D* hist_wave_uncor_S [100];
    TH1D* hist_wave_cor_S [100];
    TH1D* hist_wave_uncor_C [100];
    TH1D* hist_wave_cor_C [100];

    for (int i=0;i<100;i++){
      hist_wave_uncor_S[i] = new TH1D(Form("uncor_%d_evt_S",i),"",1000,0,1000);
      hist_wave_cor_S[i] = new TH1D(Form("cor_%d_evt_S",i),"",1000,0,1000);
      hist_wave_uncor_C[i] = new TH1D(Form("uncor_%d_evt_C",i),"",1000,0,1000);
      hist_wave_cor_C[i] = new TH1D(Form("cor_%d_evt_C",i),"",1000,0,1000);
    }

    TH1F* hist_CC1 = new TH1F("CC1", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_CC2 = new TH1F("CC2", ";peakADC;Events", 1024, 0, 4096);
    
    TH1F* hist_PS = new TH1F("PS" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_MC = new TH1F("MC" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_TC = new TH1F("TC" , ";peakADC;nEvents", 1024, 0, 4096);
    // TH1F* hist_PS = new TH1F("PS" , ";intADC;nEvents", 320, -20000, 300000);
    // TH1F* hist_MC = new TH1F("MC" , ";intADC;nEvents", 320, -20000, 300000);
    // TH1F* hist_TC = new TH1F("TC" , ";intADC;nEvents", 320, -20000, 300000);
    
    TH2D* hist_DWC1_pos_corrected   = new TH2D("DWC1_pos_corrected",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_corrected   = new TH2D("DWC2_pos_corrected",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_corrected = new TH2D("DWC_x_corr_corrected", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_corrected = new TH2D("DWC_y_corr_corrected", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);
    
    // Histograms after PID
    TH1F* hist_PS_after = new TH1F("PS_after" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_MC_after = new TH1F("MC_after" , ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_TC_after = new TH1F("TC_after" , ";peakADC;nEvents", 1024, 0, 4096);
    
    TH1F* hist_CC1_after = new TH1F("CC1_after", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_CC2_after = new TH1F("CC2_after", ";peakADC;Events", 1024, 0, 4096);
    
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
    int count = 0;
    for (int iEvt = 0; iEvt < fMaxEvent; iEvt++) {
        printProgress(iEvt, fMaxEvent);
        // Load event
        TBevt<TBwaveform> anEvt = readerWave.GetAnEvent();
        
        // Get data of certain channel we want to use
        // Get pedestal corrected waveform
        // TBwaveform::pedcorrectedWaveform(float ped): when using external (e.g. run pedestal)
        // TBwaveform::pedcorrectedWaveform(): when using event-by-event pedestal (ped = average of first 100 bin except 0th bin)
        
        // Get waveform for DWCs
	std::vector<short> waveform_PS = (anEvt.GetData(cid_PS)).waveform();	
	std::vector<short> waveform_MC = (anEvt.GetData(cid_MC)).waveform();	
	std::vector<short> waveform_TC = (anEvt.GetData(cid_TC)).waveform();	
	std::vector<short> waveform_CC1 = (anEvt.GetData(cid_CC1)).waveform();	
	std::vector<short> waveform_CC2 = (anEvt.GetData(cid_CC2)).waveform();	

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
        
	double signal_PS = GetPeak(waveform_PS, PS_first, PS_last); // PeakADC
        double signal_MC = GetPeak(waveform_MC, MC_first, MC_last); // PeakADC
        double signal_TC = GetPeak(waveform_TC, TC_first, TC_last); // PeakADC
        double signal_CC1 = GetPeak(waveform_CC1, CC1_peak_first, CC1_peak_last); // PeakADC
        double signal_CC2 = GetPeak(waveform_CC2, CC2_peak_first, CC2_peak_last); // PeakADC

        hist_CC1->Fill(signal_CC1);
        hist_CC2->Fill(signal_CC2);
        
        hist_PS->Fill(signal_PS);
        hist_MC->Fill(signal_MC);
        hist_TC->Fill(signal_TC);
        
        hist_DWC1_pos_corrected  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_corrected  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1)); 
        hist_DWC_x_corr_corrected->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_corrected->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));
        
        // Event selection here!!
        // Require C1, C2 to have intADC larger than cut
        // if ( !((cut_CC1 < signal_CC1) && (cut_CC2 < signal_CC2)) ) continue;
        if ( !(dwcCorrelationCut(DWC1_corrected_pos, DWC2_corrected_pos, cut_DWC)) ) continue;
        //if ( signal_PS < cut_PS1 || signal_PS > cut_PS2 ) continue; // Select above 3 mip peak
        if ( signal_PS < cut_PS2 ) continue; // Select above 3 mip peak
        if ( signal_MC > cut_MC ) continue; // Select only pedestals
        
	std::vector<short> waveuncor_S = (anEvt.GetData(cid_M7_C.at(2))).waveform();
	std::vector<float> wavecor_S = (anEvt.GetData(cid_M7_C.at(2))).ADCcorrectedWaveformF();
	std::vector<short> waveuncor_C = (anEvt.GetData(cid_M7_C.at(2))).waveform();
	std::vector<float> wavecor_C = (anEvt.GetData(cid_M7_C.at(2))).ADCcorrectedWaveformF();
        if (count < 100){
	  for (int i = 1; i<1001;i++){
	    hist_wave_uncor_S[count]->Fill(i-1,waveuncor_S.at(i));
	    hist_wave_cor_S[count]->Fill(i-1,wavecor_S.at(i));
	    hist_wave_uncor_C[count]->Fill(i-1,waveuncor_C.at(i));
	    hist_wave_cor_C[count]->Fill(i-1,wavecor_C.at(i));
	  }
	}
	else break;

	count++;

	hist_CC1_after->Fill(signal_CC1);
        hist_CC2_after->Fill(signal_CC2);
        
        hist_PS_after->Fill(signal_PS);
        hist_MC_after->Fill(signal_MC);
        hist_TC_after->Fill(signal_TC);
        
        hist_DWC1_pos_after  ->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_after  ->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1));
        hist_DWC_x_corr_after->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_after->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));


    }
    
    // Output file
    std::string outFile = "./Waveform/waveform_Run_" + std::to_string(fRunNum) +"_" +correctionMode +".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();
    hist_CC1->Write();
    hist_CC2->Write();
    
    hist_PS->Write();
    hist_MC->Write();
    hist_TC->Write();
    
    hist_DWC1_pos_corrected  ->Write();
    hist_DWC2_pos_corrected  ->Write();
    hist_DWC_x_corr_corrected->Write();
    hist_DWC_y_corr_corrected->Write();

    hist_CC1_after->Write();
    hist_CC2_after->Write();
    
    hist_PS_after->Write();
    hist_MC_after->Write();
    hist_TC_after->Write();
    
    hist_DWC1_pos_after  ->Write();
    hist_DWC2_pos_after  ->Write();
    hist_DWC_x_corr_after->Write();
    hist_DWC_y_corr_after->Write();

    for (int i=0;i<100;i++){
      hist_wave_uncor_S[i]->Write();
      hist_wave_cor_S[i]->Write();
      hist_wave_uncor_C[i]->Write();
      hist_wave_cor_C[i]->Write();
    }
    
    outputRoot->Close();
    
    return 0;
}
