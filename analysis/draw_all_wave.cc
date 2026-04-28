#include "TBread.h"
#include "TButility.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "TFile.h"
#include "TH2.h"

#include "function.h"

namespace fs = std::filesystem;

namespace {
constexpr int kWaveformBins = 1024;
constexpr int kAdcBins = 1000;
constexpr double kAdcMin = -500.0;
constexpr double kAdcMax = 500.0;

struct ChannelInfo {
    std::string name;
    TBcid cid;
    std::string type;
};

struct SelectionConfig {
    int CC1_peak_first = 650;
    int CC1_peak_last = 750;
    int CC2_peak_first = 620;
    int CC2_peak_last = 850;
    int PS_first = 200;
    int PS_last = 320;
    int MC_first = 650;
    int MC_last = 850;

    float cut_PS1 = 50.f;
    float cut_PS2 = 300.f;
    float cut_MC = 38.f;
    float cut_DWC = 4.f;
};

struct RuntimeConfig {
    int run = -1;
    int maxEvent = -1;
    int maxFile = -1;
    bool isKEK = false;
    std::string correctionMode = "FixRefLine";
    std::string correctionSource = "ROOT";
    std::string correctionPath = "../correction_entire.root";
    std::string mappingPath = "../mapping/mapping_TB2025_v1.root";
    std::string dataPath = "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_Data/";
    std::string outRootPath;
};

RuntimeConfig ParseArgs(int argc, char** argv)
{
    if (argc < 4) {
        throw std::runtime_error(
            "Usage: ./draw_all_waveforms_TH2D <run> <maxEvent> <isKEK(0/1)>");
    }

    RuntimeConfig cfg;
    cfg.run = std::stoi(argv[1]);
    cfg.maxEvent = std::stoi(argv[2]);
    cfg.isKEK = (std::stoi(argv[3]) != 0);

    if (cfg.isKEK) {
        if (cfg.mappingPath == "../mapping/mapping_TB2025_v1.root")
            cfg.mappingPath = "../mapping/mapping_KEK_v1.root";
        if (cfg.dataPath == "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2025_DRC_TB_Data/")
            cfg.dataPath = "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/KEK_DRC_TB_Data/";
        if (cfg.correctionPath == "../correction_entire.root")
            cfg.correctionPath = "../kek_mean.csv";
    }

    cfg.outRootPath = Form("./draw_wave/draw_wave_run_%d.root", cfg.run);

    return cfg;
}

std::vector<float> GetWaveformWithOptionalCorrection(const TBwaveform& wave, bool applyCorrection, bool& usedFallback)
{
    usedFallback = false;
    if (applyCorrection) {
        try {
            return wave.ADCpedcorrectedWaveformF();
        } catch (const std::exception&) {
            usedFallback = true;
        }
    }

    const std::vector<short> raw = wave.waveform();
    return std::vector<float>(raw.begin(), raw.end());
}

bool IsValidCID(const TBcid& cid)
{
    return (cid.mid() >= 0 && cid.channel() > 0);
}

std::vector<ChannelInfo> BuildChannels(TButility& util)
{
    std::vector<ChannelInfo> channels;

    for (int module = 1; module <= 9; ++module) {
        for (int tower = 1; tower <= 4; ++tower) {
            const std::string sName = Form("M%d_T%d_S", module, tower);
            const std::string cName = Form("M%d_T%d_C", module, tower);
            channels.push_back(ChannelInfo{sName, util.GetCID(Form("M%d-T%d-S", module, tower)), "MxTxS"});
            channels.push_back(ChannelInfo{cName, util.GetCID(Form("M%d-T%d-C", module, tower)), "MxTxC"});
        }
    }

    for (int idx = 1; idx <= 64; ++idx) {
        channels.push_back(ChannelInfo{Form("S%d", idx), util.GetCID(Form("S%d", idx)), "S"});
        channels.push_back(ChannelInfo{Form("C%d", idx), util.GetCID(Form("C%d", idx)), "C"});
    }

    // Source order from analysis/get_ADC_factor_LC.cc (LC_num array).
    const std::vector<int> lcOrderFromSource = {11, 12, 13, 19, 2, 4, 8, 10, 3, 5, 7, 9, 14, 15, 16, 20};
    std::set<int> seen;
    for (int lc : lcOrderFromSource) {
        if (lc < 2 || lc > 20) continue;
        channels.push_back(ChannelInfo{Form("LC%d", lc), util.GetCID(Form("LC%d", lc)), "LC"});
        seen.insert(lc);
    }
    for (int lc = 2; lc <= 20; ++lc) {
        if (seen.count(lc) == 0) {
            channels.push_back(ChannelInfo{Form("LC%d", lc), util.GetCID(Form("LC%d", lc)), "LC"});
        }
    }

    return channels;
}

} // namespace

int main(int argc, char** argv)
{
    RuntimeConfig cfg;
    try {
        cfg = ParseArgs(argc, argv);
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    SelectionConfig sel;

    fs::create_directories(fs::path(cfg.outRootPath).parent_path());
    fs::create_directories("./draw_wave");

    TButility util;
    util.LoadMapping(cfg.mappingPath);

    TBwaveform::SetCorrectionMode(cfg.correctionMode);
    TBwaveform::SetCorrectionSource(cfg.correctionSource);
    TBwaveform::SetCorrectionPathForMode(cfg.correctionMode, cfg.correctionPath);
    TBwaveform::SetCorrectionCSVPathForMode("FixRefLine", cfg.correctionPath);

    TFile* fDWC = TFile::Open(Form("./DWC/DWC_Run_%d.root", cfg.run), "READ");
    if (!fDWC || fDWC->IsZombie()) {
        std::cerr << "[ERROR] Failed to open DWC file for run " << cfg.run << std::endl;
        return 2;
    }
    TH2D* hDWC1 = static_cast<TH2D*>(fDWC->Get("dwc1_pos"));
    TH2D* hDWC2 = static_cast<TH2D*>(fDWC->Get("dwc2_pos"));
    if (!hDWC1 || !hDWC2) {
        std::cerr << "[ERROR] DWC histograms dwc1_pos/dwc2_pos are missing." << std::endl;
        fDWC->Close();
        return 3;
    }
    const std::vector<float> DWC1_offset = getDWCoffset(hDWC1);
    const std::vector<float> DWC2_offset = getDWCoffset(hDWC2);
    fDWC->Close();

    std::vector<ChannelInfo> channels = BuildChannels(util);
    std::unordered_map<std::string, TH2D*> histMap;
    histMap.reserve(channels.size());

    for (const ChannelInfo& ch : channels) {
        TH2D* h2 = new TH2D(
            Form("h2_wf_%s", ch.name.c_str()),
            Form("h2_wf_%s;sample index;ADC", ch.name.c_str()),
            kWaveformBins, 0, kWaveformBins,
            kAdcBins, kAdcMin, kAdcMax);
        histMap[ch.name] = h2;
    }

    TBcid cidPS = util.GetCID("PS");
    TBcid cidMC = util.GetCID("MC");
    TBcid cidCC1 = util.GetCID("CC1");
    TBcid cidCC2 = util.GetCID("CC2");

    TBcid cidDWC1L = util.GetCID("DWC1L");
    TBcid cidDWC1R = util.GetCID("DWC1R");
    TBcid cidDWC1U = util.GetCID("DWC1U");
    TBcid cidDWC1D = util.GetCID("DWC1D");
    TBcid cidDWC2L = util.GetCID("DWC2L");
    TBcid cidDWC2R = util.GetCID("DWC2R");
    TBcid cidDWC2U = util.GetCID("DWC2U");
    TBcid cidDWC2D = util.GetCID("DWC2D");

    // MID list follows existing analysis code (get_ADC_factor/get_ADC_factor_LC/calib_DRC_ADC_mode_example).
    TBread<TBwaveform> readerWave(cfg.run, cfg.maxEvent, cfg.maxFile, false, cfg.dataPath, {3, 4, 5, 6, 7, 9, 10, 12, 14, 15, 16, 17, 18});

    if (cfg.maxEvent == -1 || cfg.maxEvent > readerWave.GetMaxEvent())
        cfg.maxEvent = readerWave.GetMaxEvent();

    long long nTotal = 0;
    long long nPass = 0;
    long long nFailDWC = 0;
    long long nFailPS = 0;
    long long nFailMC = 0;

    std::set<std::string> warnedMissingCorrection;
    std::set<std::string> warnedInvalidCID;

    for (int iEvt = 0; iEvt < cfg.maxEvent; ++iEvt) {
        printProgress(iEvt, cfg.maxEvent);
        ++nTotal;

        TBevt<TBwaveform> evt = readerWave.GetAnEvent();

        std::vector<short> wavePS = evt.GetData(cidPS).waveform();
        std::vector<short> waveMC = evt.GetData(cidMC).waveform();
        std::vector<short> waveCC1 = evt.GetData(cidCC1).waveform();
        std::vector<short> waveCC2 = evt.GetData(cidCC2).waveform();

        std::vector<short> waveDWC1R = evt.GetData(cidDWC1R).waveform();
        std::vector<short> waveDWC1L = evt.GetData(cidDWC1L).waveform();
        std::vector<short> waveDWC1U = evt.GetData(cidDWC1U).waveform();
        std::vector<short> waveDWC1D = evt.GetData(cidDWC1D).waveform();
        std::vector<short> waveDWC2R = evt.GetData(cidDWC2R).waveform();
        std::vector<short> waveDWC2L = evt.GetData(cidDWC2L).waveform();
        std::vector<short> waveDWC2U = evt.GetData(cidDWC2U).waveform();
        std::vector<short> waveDWC2D = evt.GetData(cidDWC2D).waveform();

        std::vector<float> DWC1_time;
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveDWC1R, 0.4, 1, 1000));
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveDWC1L, 0.4, 1, 1000));
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveDWC1U, 0.4, 1, 1000));
        DWC1_time.emplace_back(getLeadingEdgeTime_interpolated800(waveDWC1D, 0.4, 1, 1000));

        std::vector<float> DWC2_time;
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveDWC2R, 0.4, 1, 1000));
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveDWC2L, 0.4, 1, 1000));
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveDWC2U, 0.4, 1, 1000));
        DWC2_time.emplace_back(getLeadingEdgeTime_interpolated800(waveDWC2D, 0.4, 1, 1000));

        std::vector<float> DWC1_pos = getDWC1position(DWC1_time, DWC1_offset);
        std::vector<float> DWC2_pos = getDWC2position(DWC2_time, DWC2_offset);

        const double signalPS = GetPeak(wavePS, sel.PS_first, sel.PS_last);
        const double signalMC = GetPeak(waveMC, sel.MC_first, sel.MC_last);
        const double signalCC1 = GetPeak(waveCC1, sel.CC1_peak_first, sel.CC1_peak_last);
        const double signalCC2 = GetPeak(waveCC2, sel.CC2_peak_first, sel.CC2_peak_last);
        (void)signalCC1;
        (void)signalCC2;

        if (!dwcCorrelationCut(DWC1_pos, DWC2_pos, sel.cut_DWC)) {
            ++nFailDWC;
            continue;
        }
        if (signalPS < sel.cut_PS1 || signalPS > sel.cut_PS2) {
            ++nFailPS;
            continue;
        }
        if (signalMC < sel.cut_MC) {
            ++nFailMC;
            continue;
        }

        ++nPass;

        for (const ChannelInfo& ch : channels) {
            if (!IsValidCID(ch.cid)) {
                if (warnedInvalidCID.insert(ch.name).second) {
                    std::cerr << "[WARN] Invalid CID for channel " << ch.name << " (mapping lookup failed). Skipping this channel." << std::endl;
                }
                continue;
            }

            TBwaveform wave = evt.GetData(ch.cid);
            bool usedFallback = false;
            std::vector<float> wf = GetWaveformWithOptionalCorrection(wave, true, usedFallback);

            if (usedFallback && warnedMissingCorrection.insert(ch.name).second) {
                std::cerr << "[WARN] Missing/invalid correction factors for " << ch.name
                          << ". Falling back to raw waveform() for this channel." << std::endl;
            }

            TH2D* h2 = histMap.at(ch.name);
            const int nSample = static_cast<int>(wf.size());
            for (int i = 0; i < nSample; ++i) {
                h2->Fill(i, wf[i]);
            }
        }
    }

    TFile* out = TFile::Open(cfg.outRootPath.c_str(), "RECREATE");
    for (const auto& kv : histMap) kv.second->Write();
    out->Close();

    std::cout << "\n========== Selection Summary ==========" << std::endl;
    std::cout << "Total events processed : " << nTotal << std::endl;
    std::cout << "Pass (DWC+PID)         : " << nPass << std::endl;
    std::cout << "Fail DWC cut           : " << nFailDWC << std::endl;
    std::cout << "Fail PS cut            : " << nFailPS << std::endl;
    std::cout << "Fail MC cut            : " << nFailMC << std::endl;
    std::cout << "Histograms written     : " << histMap.size() << std::endl;
    std::cout << "Output ROOT            : " << cfg.outRootPath << std::endl;

    return 0;
}

