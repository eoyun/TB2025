#include "TBread.h"
#include "TButility.h"

#include <filesystem>
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <algorithm>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "function.h"

namespace fs = std::filesystem;

namespace {

constexpr int kNPatch = 11;
constexpr int kWaveFirst = 1;
constexpr int kWaveLastExclusive = 951;

std::string SafeName(std::string name) {
    std::replace(name.begin(), name.end(), '-', '_');
    std::replace(name.begin(), name.end(), '/', '_');
    return name;
}

int SampleToDrsCell(int sampleIdx, int drsStop) {
    int cell = sampleIdx + drsStop + 1;
    while (cell >= 1024) cell -= 1024;
    while (cell < 0) cell += 1024;
    return cell;
}

void EnsureDir(const fs::path& dir) {
    if (!fs::exists(dir)) fs::create_directories(dir);
}

void Save2DAndProfile(TH2D* hist, TDirectory* outDir, const fs::path& plotDir) {
    if (hist == nullptr) return;

    outDir->cd();
    hist->Write();

    TProfile* prof = hist->ProfileX((std::string(hist->GetName()) + "_profX").c_str(), 1, -1, "");
    prof->Write();

    TCanvas c2d((std::string(hist->GetName()) + "_c2d").c_str(), "", 1200, 800);
    hist->Draw("COLZ");
    c2d.SaveAs((plotDir / (std::string(hist->GetName()) + ".png")).string().c_str());

    TCanvas c1d((std::string(hist->GetName()) + "_c1d").c_str(), "", 1200, 800);
    prof->SetLineWidth(2);
    prof->Draw();
    c1d.SaveAs((plotDir / (std::string(prof->GetName()) + ".png")).string().c_str());
}

struct ChannelPlots {
    std::string label;
    std::string safe;
    TBcid cid;

    TH2D* wave_uncor = nullptr;
    TH2D* wave_cor_match_patch = nullptr;
    TH2D* drs_uncor = nullptr;
    TH2D* drs_cor_match_patch = nullptr;
    std::array<TH2D*, kNPatch> drs_cor{};
};

ChannelPlots MakeChannelPlots(const std::string& label, const TBcid& cid) {
    ChannelPlots ch;
    ch.label = label;
    ch.safe = SafeName(label);
    ch.cid = cid;

    ch.wave_uncor = new TH2D(
        (ch.safe + "_wave_uncor").c_str(),
        (label + ";Waveform bin;ADC").c_str(),
        1000, 0, 1000, 200, -100, 100);

    ch.wave_cor_match_patch = new TH2D(
        (ch.safe + "_wave_cor_match_patch").c_str(),
        (label + ";Waveform bin;ADC").c_str(),
        1000, 0, 1000, 200, -100, 100);

    ch.drs_uncor = new TH2D(
        (ch.safe + "_drs_uncor").c_str(),
        (label + ";DRS cell;ADC").c_str(),
        1024, 0, 1024, 200, -100, 100);

    ch.drs_cor_match_patch = new TH2D(
        (ch.safe + "_drs_cor_match_patch").c_str(),
        (label + ";DRS cell;ADC").c_str(),
        1024, 0, 1024, 200, -100, 100);

    for (int i = 0; i < kNPatch; ++i) {
        ch.drs_cor[i] = new TH2D(
            Form("%s_drs_cor_patch_%d", ch.safe.c_str(), i),
            Form("%s patch %d;DRS cell;ADC", label.c_str(), i),
            1024, 0, 1024, 200, -100, 100);
    }

    return ch;
}

} // namespace

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

    int LC_first = 400; // LC integration range
    int LC_last  = 600; // LC integration range

    // cuts
    float cut_CC1 = 60.;
    float cut_CC2 = 100.;

    float cut_PS1 = 50.;
    float cut_PS2 = 300.;
    float cut_MC  = 38.;

    float cut_DWC = 4;

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <RunNum> <MaxEvent>" << std::endl;
        return 1;
    }

    int fRunNum   = std::stoi(argv[1]);
    int fMaxEvent = std::stoi(argv[2]);
    int fMaxFile  = -1;

    gROOT->SetBatch(kTRUE);
    gStyle->SetOptStat(0);

    TFile* f_DWC = TFile::Open((TString)("./DWC/DWC_Run_" + std::to_string(fRunNum) + ".root"), "READ");
    TH2D* h_DWC1_pos = (TH2D*) f_DWC->Get("dwc1_pos");
    TH2D* h_DWC2_pos = (TH2D*) f_DWC->Get("dwc2_pos");
    std::vector<float> DWC1_offset = getDWCoffset(h_DWC1_pos);
    std::vector<float> DWC2_offset = getDWCoffset(h_DWC2_pos);
    f_DWC->Close();

    fs::path baseDir("./wave_Cor");
    EnsureDir(baseDir);
    fs::path plotBaseDir = baseDir / ("plots_Run_" + std::to_string(fRunNum));
    EnsureDir(plotBaseDir);

    std::string outFile = (baseDir / ("wave_Cor_Run_" + std::to_string(fRunNum) + ".root")).string();
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");

    TH1F* hist_CC1 = new TH1F("CC1", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_CC2 = new TH1F("CC2", ";peakADC;Events", 1024, 0, 4096);

    TH1F* hist_PS = new TH1F("PS", ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_MC = new TH1F("MC", ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_TC = new TH1F("TC", ";peakADC;nEvents", 1024, 0, 4096);

    TH2D* hist_DWC1_pos_corrected   = new TH2D("DWC1_pos_corrected",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_corrected   = new TH2D("DWC2_pos_corrected",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_corrected = new TH2D("DWC_x_corr_corrected", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_corrected = new TH2D("DWC_y_corr_corrected", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);

    TH1F* hist_PS_after = new TH1F("PS_after", ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_MC_after = new TH1F("MC_after", ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_TC_after = new TH1F("TC_after", ";peakADC;nEvents", 1024, 0, 4096);

    TH1F* hist_CC1_after = new TH1F("CC1_after", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_CC2_after = new TH1F("CC2_after", ";peakADC;Events", 1024, 0, 4096);

    TH2D* hist_DWC1_pos_after   = new TH2D("DWC1_pos_after",   "dwc1_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC2_pos_after   = new TH2D("DWC2_pos_after",   "dwc2_pos;mm;mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_x_corr_after = new TH2D("DWC_x_corr_after", "dwc_x_corr;DWC1_X_mm;DWC2_X_mm;events", 480, -120., 120., 480, -120., 120.);
    TH2D* hist_DWC_y_corr_after = new TH2D("DWC_y_corr_after", "dwc_y_corr;DWC1_Y_mm;DWC2_Y_mm;events", 480, -120., 120., 480, -120., 120.);

    TButility util = TButility();
    util.LoadMapping("../mapping/mapping_TB2025_v1.root");
    TBcid cid_CC1 = util.GetCID("CC1");
    TBcid cid_CC2 = util.GetCID("CC2");
    TBcid cid_PS  = util.GetCID("PS");
    TBcid cid_MC  = util.GetCID("MC");
    TBcid cid_TC  = util.GetCID("TC");

    TBcid cid_DWC1_L = util.GetCID("DWC1L");
    TBcid cid_DWC1_R = util.GetCID("DWC1R");
    TBcid cid_DWC1_U = util.GetCID("DWC1U");
    TBcid cid_DWC1_D = util.GetCID("DWC1D");
    TBcid cid_DWC2_L = util.GetCID("DWC2L");
    TBcid cid_DWC2_R = util.GetCID("DWC2R");
    TBcid cid_DWC2_U = util.GetCID("DWC2U");
    TBcid cid_DWC2_D = util.GetCID("DWC2D");

    std::vector<ChannelPlots> towerChannels;
    towerChannels.reserve(72);
    for (int module = 1; module <= 9; ++module) {
        for (int tower = 1; tower <= 4; ++tower) {
            for (const std::string& sc : {std::string("S"), std::string("C")}) {
                const std::string label = Form("M%d-T%d-%s", module, tower, sc.c_str());
                towerChannels.emplace_back(MakeChannelPlots(label, util.GetCID(label)));
            }
        }
    }

    // MID: 3-7 PMT modules, 10 Aux, 12 Triggers, 18 DWC
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(
        fRunNum,
        fMaxEvent,
        fMaxFile,
        false,
        "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_Data/",
        {3, 4, 5, 6, 7, 10, 12, 18});

    if (fMaxEvent == -1) fMaxEvent = readerWave.GetMaxEvent();
    if (fMaxEvent > readerWave.GetMaxEvent()) fMaxEvent = readerWave.GetMaxEvent();

    for (int iEvt = 0; iEvt < fMaxEvent; ++iEvt) {
        printProgress(iEvt, fMaxEvent);
        TBevt<TBwaveform> aEvent = readerWave.GetAnEvent();

        TBwaveform PS_wave  = aEvent.GetData(cid_PS);
        TBwaveform MC_wave  = aEvent.GetData(cid_MC);
        TBwaveform TC_wave  = aEvent.GetData(cid_TC);
        TBwaveform CC1_wave = aEvent.GetData(cid_CC1);
        TBwaveform CC2_wave = aEvent.GetData(cid_CC2);

        TBwaveform DWC1L_wave = aEvent.GetData(cid_DWC1_L);
        TBwaveform DWC1R_wave = aEvent.GetData(cid_DWC1_R);
        TBwaveform DWC1U_wave = aEvent.GetData(cid_DWC1_U);
        TBwaveform DWC1D_wave = aEvent.GetData(cid_DWC1_D);
        TBwaveform DWC2L_wave = aEvent.GetData(cid_DWC2_L);
        TBwaveform DWC2R_wave = aEvent.GetData(cid_DWC2_R);
        TBwaveform DWC2U_wave = aEvent.GetData(cid_DWC2_U);
        TBwaveform DWC2D_wave = aEvent.GetData(cid_DWC2_D);

        std::vector<short> waveform_PS  = PS_wave.waveform();
        std::vector<short> waveform_MC  = MC_wave.waveform();
        std::vector<short> waveform_TC  = TC_wave.waveform();
        std::vector<short> waveform_CC1 = CC1_wave.waveform();
        std::vector<short> waveform_CC2 = CC2_wave.waveform();

        std::vector<short> waveform_DWC1L = DWC1L_wave.waveform();
        std::vector<short> waveform_DWC1R = DWC1R_wave.waveform();
        std::vector<short> waveform_DWC1U = DWC1U_wave.waveform();
        std::vector<short> waveform_DWC1D = DWC1D_wave.waveform();
        std::vector<short> waveform_DWC2L = DWC2L_wave.waveform();
        std::vector<short> waveform_DWC2R = DWC2R_wave.waveform();
        std::vector<short> waveform_DWC2U = DWC2U_wave.waveform();
        std::vector<short> waveform_DWC2D = DWC2D_wave.waveform();

        std::vector<float> DWC1_time;
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC1R, 0.4, 1, 1000));
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC1L, 0.4, 1, 1000));
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC1U, 0.4, 1, 1000));
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC1D, 0.4, 1, 1000));

        std::vector<float> DWC2_time;
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC2R, 0.4, 1, 1000));
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC2L, 0.4, 1, 1000));
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC2U, 0.4, 1, 1000));
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveform_DWC2D, 0.4, 1, 1000));

        std::vector<float> DWC1_corrected_pos = getDWC1position(DWC1_time, DWC1_offset);
        std::vector<float> DWC2_corrected_pos = getDWC2position(DWC2_time, DWC2_offset);

        double signal_PS  = GetPeak(waveform_PS,  PS_first,  PS_last);
        double signal_MC  = GetPeak(waveform_MC,  MC_first,  MC_last);
        double signal_TC  = GetPeak(waveform_TC,  TC_first,  TC_last);
        double signal_CC1 = GetPeak(waveform_CC1, CC1_peak_first, CC1_peak_last);
        double signal_CC2 = GetPeak(waveform_CC2, CC2_peak_first, CC2_peak_last);

        hist_CC1->Fill(signal_CC1);
        hist_CC2->Fill(signal_CC2);
        hist_PS->Fill(signal_PS);
        hist_MC->Fill(signal_MC);
        hist_TC->Fill(signal_TC);

        hist_DWC1_pos_corrected->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_corrected->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1));
        hist_DWC_x_corr_corrected->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_corrected->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));

        if (!(dwcCorrelationCut(DWC1_corrected_pos, DWC2_corrected_pos, cut_DWC))) continue;
        if (signal_PS < cut_PS1 || signal_PS > cut_PS2) continue;
        if (signal_MC < cut_MC) continue;

        hist_CC1_after->Fill(signal_CC1);
        hist_CC2_after->Fill(signal_CC2);
        hist_PS_after->Fill(signal_PS);
        hist_MC_after->Fill(signal_MC);
        hist_TC_after->Fill(signal_TC);

        hist_DWC1_pos_after->Fill(DWC1_corrected_pos.at(0), DWC1_corrected_pos.at(1));
        hist_DWC2_pos_after->Fill(DWC2_corrected_pos.at(0), DWC2_corrected_pos.at(1));
        hist_DWC_x_corr_after->Fill(DWC1_corrected_pos.at(0), DWC2_corrected_pos.at(0));
        hist_DWC_y_corr_after->Fill(DWC1_corrected_pos.at(1), DWC2_corrected_pos.at(1));

        for (auto& ch : towerChannels) {
            TBwaveform towerWave = aEvent.GetData(ch.cid);
            int drs_stop = towerWave.drs_stop();

            std::vector<float> waveuncor = towerWave.pedcorrectedWaveform();
            std::vector<float> wavecor_match_patch = towerWave.ADCpedcorrectedWaveform();
            std::array<std::vector<float>, kNPatch> wavecor_all_patch;
	    //std::cout<<waveuncor.size()<<" | "<<wavecor_match_patch.size()<<" | "<<ch.label<<std::endl;
            for (int patch = 0; patch < kNPatch; ++patch) {
		//std::cout<<"patch  : "<<patch<<std::endl;
                wavecor_all_patch[patch] = towerWave.ADCpedcorrectedWaveform(patch);
            }
	    //std::cout<<"dbg 1"<<std::endl;

            for (int sample = kWaveFirst; sample < kWaveLastExclusive; ++sample) {
	    //std::cout<<"dbg 2"<<std::endl;
                ch.wave_uncor->Fill(sample - 1, waveuncor.at(sample));
	    //std::cout<<"dbg 3"<<std::endl;
                ch.wave_cor_match_patch->Fill(sample - 1, wavecor_match_patch.at(sample));
	    //std::cout<<"dbg 4"<<std::endl;

	    //std::cout<<"dbg 5"<<std::endl;
                const int drsCell = SampleToDrsCell(sample, drs_stop);
	    //std::cout<<"dbg 6"<<std::endl;
                ch.drs_uncor->Fill(drsCell, waveuncor.at(sample));
	    //std::cout<<"dbg 7"<<std::endl;
                ch.drs_cor_match_patch->Fill(drsCell, wavecor_match_patch.at(sample));
	    //std::cout<<"dbg 8"<<std::endl;
                for (int patch = 0; patch < kNPatch; ++patch) {
                    ch.drs_cor[patch]->Fill(drsCell, wavecor_all_patch[patch].at(sample));
                }
	    //std::cout<<"dbg 9 | "<<ch.label<<std::endl;
            }
        }
    }

    outputRoot->cd();
    hist_CC1->Write();
    hist_CC2->Write();
    hist_PS->Write();
    hist_MC->Write();
    hist_TC->Write();
    hist_DWC1_pos_corrected->Write();
    hist_DWC2_pos_corrected->Write();
    hist_DWC_x_corr_corrected->Write();
    hist_DWC_y_corr_corrected->Write();

    hist_CC1_after->Write();
    hist_CC2_after->Write();
    hist_PS_after->Write();
    hist_MC_after->Write();
    hist_TC_after->Write();
    hist_DWC1_pos_after->Write();
    hist_DWC2_pos_after->Write();
    hist_DWC_x_corr_after->Write();
    hist_DWC_y_corr_after->Write();

    TDirectory* towersDir = outputRoot->mkdir("tower_channels");
    for (auto& ch : towerChannels) {
        TDirectory* chDir = towersDir->mkdir(ch.safe.c_str());
        fs::path chPlotDir = plotBaseDir / ch.safe;
        EnsureDir(chPlotDir);

        Save2DAndProfile(ch.wave_uncor, chDir, chPlotDir);
        Save2DAndProfile(ch.wave_cor_match_patch, chDir, chPlotDir);
        Save2DAndProfile(ch.drs_uncor, chDir, chPlotDir);
        Save2DAndProfile(ch.drs_cor_match_patch, chDir, chPlotDir);
        for (int patch = 0; patch < kNPatch; ++patch) {
            Save2DAndProfile(ch.drs_cor[patch], chDir, chPlotDir);
        }
    }

    outputRoot->Close();
    return 0;
}
