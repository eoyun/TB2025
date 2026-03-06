#include "TBmid.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include "stdio.h"
#include <numeric>
#include <iostream>
#include <fstream>
#include <vector>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>

bool LoadCorrectionFactors(const TString& filename,
                           const TString& targetName,
                           std::vector<double>& factors)
{
    std::ifstream fin(filename.Data());
    if (!fin.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }

    factors.clear();

    std::string line_std;
    while (std::getline(fin, line_std)) {

        TString line(line_std);

        // comma 기준 분리
        TObjArray* tokens = line.Tokenize(",");

        if (tokens->GetEntries() < 2) {
            delete tokens;
            continue;
        }

        TString name = ((TObjString*)tokens->At(0))->GetString();

        if (name == targetName) {

            for (int i = 1; i < tokens->GetEntries(); ++i) {
                TString valStr = ((TObjString*)tokens->At(i))->GetString();
                factors.push_back(valStr.Atof());
            }

            delete tokens;
            return true;
        }

        delete tokens;
    }

    return false;
}

TBwaveform::TBwaveform()
    : channel_(-1), waveform_(0),drs_stop_(-1),name_("") {}

void TBwaveform::init()
{
  waveform_.resize(1024, 0);
}

std::vector<float> TBwaveform::pedcorrectedWaveform(float ped) const
{
  std::vector<float> result;
  result.reserve(waveform_.size());

  for (unsigned idx = 0; idx < waveform_.size(); idx++)
  {
    float abin = static_cast<float>(ped) - static_cast<float>(waveform_.at(idx));
    result.emplace_back(abin);
  }

  return std::move(result);
}

std::vector<float> TBwaveform::pedcorrectedWaveform() const
{
  std::vector<float> result;
  result.reserve(waveform_.size());

  float ped = 0;
  for (int i = 1; i < 101; i++)
    ped += static_cast<float>(waveform_.at(i)) / 100.;

  for (unsigned idx = 0; idx < waveform_.size(); idx++)
  {
    float abin = ped - static_cast<float>(waveform_.at(idx));
    result.emplace_back(abin);
  }

  return std::move(result);
}

float TBwaveform::pedcorrectedADC(float ped, int buffer) const
{
  auto corrected = pedcorrectedWaveform(ped);

  return std::accumulate(corrected.rbegin() + buffer, corrected.rend(), 0.);
}

float TBwaveform::emulfastADC(int rise, int width, int buffer) const
{
  auto waveform_eff = waveform_;

  for (int i = 0; i < buffer; ++i) // remove last 24 bins
    waveform_eff.pop_back();

  waveform_eff.erase(waveform_eff.begin()); // remove 0th bin

  auto result = std::min_element(waveform_eff.begin(), waveform_eff.end());

  int idx_min = result - waveform_eff.begin();

  auto waveform_shfted = waveform_eff;

  std::rotate(waveform_shfted.begin(), waveform_shfted.begin() + idx_min, waveform_shfted.end());
  std::rotate(waveform_shfted.rbegin(), waveform_shfted.rbegin() + (rise + width), waveform_shfted.rend());

  float adc_sig = 0.0;
  float adc_ped = 0.0;

  for (int i = 0; i < width; ++i)
  {
    adc_ped += 4096.0 - waveform_shfted.at(i);
    adc_sig += 4096.0 - waveform_shfted.at(i + width);
  }

  return adc_sig - adc_ped;
}

std::vector<float> TBwaveform::ADCcorrectedWaveform() const
{
  std::vector<float> result;
  int point[11] = {1,94,187,280,373,466,560,653,746,839,932};
  int min_diff = abs(drs_stop_ - point[0]);
  int index = 0;
  for (int i=1;i<11;i++){
    int diff = abs(drs_stop_ - point[i]);
    if (min_diff > diff){
      index = i;
      min_diff = diff;
    }
  }
  TFile * f_ADC_calib = new TFile("../drs_stop_Run_12341.root","read");
  TString name = name_;   // copy
  name.ReplaceAll("-", "_");
  //std::cout<<"test "<<" | "<<Form("%s_%02d",name.Data(),index)<<std::endl;
  TH2D* h = (TH2D*)f_ADC_calib->Get(Form("%s_%02d",name.Data(),index));
  //std::vector<double> corr;
  
  //LoadCorrectionFactors("../th2d_means.csv",  Form("%d_%02d",name.Data(),index), corr);

  for (int j = 0; j < (int)waveform_.size(); j++){
    int bin;
    if (j + drs_stop_ + 1 < 1024) bin = j + drs_stop_ + 1;
    else bin = j + drs_stop_ + 1 - 1024;
    //double mean = corr.at(bin);
    TH1D* proy = h->ProjectionY("proy",bin,bin,"");
    double mean = proy->GetMean();;
    result.push_back(waveform_.at(j) + mean);
  }
  f_ADC_calib->Close();
  return std::move(result);
}

std::vector<float> TBwaveform::ADCpedcorrectedWaveform() const
{
  std::vector<float> result;
  int point[11] = {1,94,187,280,373,466,560,653,746,839,932};
  int min_diff = abs(drs_stop_ - point[0]);
  int index = 0;
  for (int i=1;i<11;i++){
    int diff = abs(drs_stop_ - point[i]);
    if (min_diff > diff){
      index = i;
      min_diff = diff;
    }
  }
  TString name = name_;   // copy
  TFile * f_ADC_calib = new TFile("../drs_stop_Run_12341.root","read");
  name.ReplaceAll("-", "_");
  //std::vector<double> corr;
  TH2D* h = (TH2D*)f_ADC_calib->Get(Form("%s_%02d",name.Data(),index));
  
  //LoadCorrectionFactors("../th2d_means.csv", Form("%d_%02d",name.Data(),index), corr);
  for (int j = 0; j < (int)waveform_.size(); j++){
    int bin;
    if (j + drs_stop_ + 1 < 1024) bin = j + drs_stop_ + 1;
    else bin = j + drs_stop_ + 1 - 1024;
    //double mean = corr.at(bin);
    TH1D* proy = h->ProjectionY("proy",bin,bin,"");
    double mean = proy->GetMean();;
    result.push_back(waveform_.at(j) + mean);
  }
  f_ADC_calib->Close();

  std::vector<float> pedresult;
  result.reserve(waveform_.size());
  float ped = 0;
  for (int i = 1; i < 101; i++)
    ped += static_cast<float>(result.at(i)) / 100.;

  for (unsigned idx = 0; idx < result.size(); idx++)
  {
    float abin = ped - static_cast<float>(result.at(idx));
    pedresult.emplace_back(abin);
  }

  return std::move(pedresult);
}

TBfastmode::TBfastmode()
    : channel_(-1), adc_(0), timing_(0), name_("") {}

TBmidbase::TBmidbase()
    : evt_(0), run_(0), mid_(0),
      tcb_trig_type_(0),
      tcb_trig_number_(0),
      tcb_trig_time_(0),
      local_trig_number_(0),
      local_trigger_pattern_(0),
      local_trig_time_(0),
      channelsize_(0) {}

TBmidbase::TBmidbase(int ev, int ru, int mi)
    : evt_(ev), run_(ru), mid_(mi),
      tcb_trig_type_(0),
      tcb_trig_number_(0),
      tcb_trig_time_(0),
      local_trig_number_(0),
      local_trigger_pattern_(0),
      local_trig_time_(0),
      channelsize_(0) {}

void TBmidbase::setTCB(int ty, int nu, long long ti)
{
  tcb_trig_type_ = ty;
  tcb_trig_number_ = nu;
  tcb_trig_time_ = ti;
}

void TBmidbase::setLocal(int nu, int pa, long long ti)
{
  local_trig_number_ = nu;
  local_trigger_pattern_ = pa;
  local_trig_time_ = ti;
}

void TBmidbase::setDRSStop (std::vector<int> drsStop)
{
  drs_stop_ = drsStop;
}

void TBmidbase::print()
{
  long long diff_time = local_trig_time_ - tcb_trig_time_;
  printf("evt = %d, run # = %d, mid = %d\n", tcb_trig_number_, run_, mid_);
  printf("trigger type = %X, local trigger pattern = %X\n", tcb_trig_type_, local_trigger_pattern_);
  printf("TCB trigger # = %d, local trigger # = %d\n", tcb_trig_number_, local_trig_number_);
  printf("TCB trigger time = %lld, local trigger time = %lld, difference = %lld\n", tcb_trig_time_, local_trig_time_, diff_time);
  printf("-----------------------------------------------------------------------\n");
}

template <typename T>
TBmid<T>::TBmid(int ev, int ru, int mi)
    : TBmidbase(ev, ru, mi), channels_(0) {}

template <typename T>
TBmid<T>::TBmid()
    : TBmidbase(), channels_(0) {}

template <typename T>
TBmid<T>::TBmid(const TBmidbase &base)
    : TBmidbase(base), channels_(0) {}

template <typename T>
void TBmid<T>::setChannels(std::vector<T> ch)
{
  channels_ = ch;
  channelsize_ = static_cast<int>(channels_.size());
}

template class TBmid<TBwaveform>;
template class TBmid<TBfastmode>;
