#include "TBread.h"
#include "TButility.h"

#include <filesystem>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>

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
#include "TDirectory.h"

#include "function.h"

namespace fs = std::filesystem;

namespace {

constexpr int kNPatch = 11;
constexpr int kWaveFirst = 1;
constexpr int kWaveLastExclusive = 951;

// Easy-to-edit DRS stop bin edges.
// Current setup: [0,100), [100,200), ..., [900,1024)
const std::vector<int> kDrsStopEdges = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1024};

std::string SafeName(std::string name) {
    std::replace(name.begin(), name.end(), '-', '_');
    std::replace(name.begin(), name.end(), '/', '_');
    std::replace(name.begin(), name.end(), ' ', '_');
    return name;
}

std::string StopBinLabel(int low, int high) {
    return Form("stop_%d_%d", low, high);
}

int SampleToDrsCell(int sampleIdx, int drsStop) {
    int cell = sampleIdx + drsStop + 1;
    while (cell >= 1024) cell -= 1024;
    while (cell < 0) cell += 1024;
    return cell;
}

int FindStopBin(int drsStop, const std::vector<int>& edges) {
    for (size_t i = 0; i + 1 < edges.size(); ++i) {
        const int low = edges[i];
        const int high = edges[i + 1];
        if (drsStop >= low && drsStop < high) return static_cast<int>(i);
    }
    return -1;
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

struct StopBinPlots {
    int low = 0;
    int high = 0;
    std::string safe;
    std::array<TH2D*, kNPatch> drs_cor{};
};

struct ChannelPlots {
    std::string label;
    std::string safe;
    TBcid cid;
    std::vector<StopBinPlots> stopBins;
};

ChannelPlots MakeChannelPlots(const std::string& label, const TBcid& cid, const std::vector<int>& stopEdges) {
    ChannelPlots ch;
    ch.label = label;
    ch.safe = SafeName(label);
    ch.cid = cid;

    const int nStopBins = static_cast<int>(stopEdges.size()) - 1;
    ch.stopBins.resize(nStopBins);

    for (int sb = 0; sb < nStopBins; ++sb) {
        StopBinPlots sbp;
        sbp.low = stopEdges[sb];
        sbp.high = stopEdges[sb + 1];
        sbp.safe = StopBinLabel(sbp.low, sbp.high);

        for (int patch = 0; patch < kNPatch; ++patch) {
            sbp.drs_cor[patch] = new TH2D(
                Form("%s_patch_%02d_%s", ch.safe.c_str(), patch, sbp.safe.c_str()),
                Form("%s patch %d, DRS stop [%d,%d);DRS cell;ADC",
                     ch.label.c_str(), patch, sbp.low, sbp.high),
                1024, 0, 1024, 200, -100, 100);
        }
        ch.stopBins[sb] = sbp;
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

    // cuts
    float cut_PS1 = 50.;
    float cut_PS2 = 300.;
    float cut_MC  = 38.;
    float cut_DWC = 4.;

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <RunNum> <MaxEvent>" << std::endl;
        return 1;
    }

    if (kDrsStopEdges.size() < 2) {
        std::cerr << "DRS stop bin edge vector must contain at least two values." << std::endl;
        return 1;
    }
    for (size_t i = 1; i < kDrsStopEdges.size(); ++i) {
        if (kDrsStopEdges[i] <= kDrsStopEdges[i - 1]) {
            std::cerr << "DRS stop bin edges must be strictly increasing." << std::endl;
            return 1;
        }
    }
    if (kDrsStopEdges.front() != 0 || kDrsStopEdges.back() != 1024) {
        std::cerr << "Expected DRS stop edges to cover [0,1024]." << std::endl;
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
    fs::path plotBaseDir = baseDir / ("plots_stopbinned_Run_" + std::to_string(fRunNum));
    EnsureDir(plotBaseDir);

    std::string outFile = (baseDir / ("wave_Cor_stopbinned_Run_" + std::to_string(fRunNum) + ".root")).string();
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");

    TH1F* hist_CC1 = new TH1F("CC1", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_CC2 = new TH1F("CC2", ";peakADC;Events", 1024, 0, 4096);
    TH1F* hist_PS  = new TH1F("PS",  ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_MC  = new TH1F("MC",  ";peakADC;nEvents", 1024, 0, 4096);
    TH1F* hist_TC  = new TH1F("TC",  ";peakADC;nEvents", 1024, 0, 4096);

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
                towerChannels.emplace_back(MakeChannelPlots(label, util.GetCID(label), kDrsStopEdges));
            }
        }
    }

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
            const int drs_stop = towerWave.drs_stop();
            const int stopBinIdx = FindStopBin(drs_stop, kDrsStopEdges);
            if (stopBinIdx < 0) continue;

            std::array<std::vector<float>, kNPatch> wavecor_all_patch;
            for (int patch = 0; patch < kNPatch; ++patch) {
                wavecor_all_patch[patch] = towerWave.ADCpedcorrectedWaveform(patch);
            }

            StopBinPlots& stopPlots = ch.stopBins[stopBinIdx];
            for (int sample = kWaveFirst; sample < kWaveLastExclusive; ++sample) {
                const int drsCell = SampleToDrsCell(sample, drs_stop);
                for (int patch = 0; patch < kNPatch; ++patch) {
                    stopPlots.drs_cor[patch]->Fill(drsCell, wavecor_all_patch[patch].at(sample));
                }
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

        for (size_t sb = 0; sb < ch.stopBins.size(); ++sb) {
            StopBinPlots& stopPlots = ch.stopBins[sb];
            TDirectory* stopDir = chDir->mkdir(stopPlots.safe.c_str());
            fs::path stopPlotDir = chPlotDir / stopPlots.safe;
            EnsureDir(stopPlotDir);

            for (int patch = 0; patch < kNPatch; ++patch) {
                Save2DAndProfile(stopPlots.drs_cor[patch], stopDir, stopPlotDir);
            }
        }
    }

    outputRoot->Close();
    return 0;
}
