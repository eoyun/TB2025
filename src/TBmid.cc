#include "TBmid.h"

#include "TBdetector.h"

#include "stdio.h"
#include <numeric>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

namespace {
std::string gCorrectionCSVPath = "../th2d_means.csv";
bool gCorrectionLoaded = false;

bool IsModuleTowerSCName(const TString &name)
{
  return (name.BeginsWith("M") && name.Contains("-T") && (name.EndsWith("-S") || name.EndsWith("-C"))) ||
         (name.BeginsWith("T") && (name.EndsWith("-S") || name.EndsWith("-C")));
}

int GetPatchIndex(int drsStop)
{
  const int points[11] = {1, 94, 187, 280, 373, 466, 560, 653, 746, 839, 932};

  int minDiff = std::abs(drsStop - points[0]);
  int index = 0;

  for (int i = 1; i < 11; ++i)
  {
    const int diff = std::abs(drsStop - points[i]);
    if (minDiff > diff)
    {
      index = i;
      minDiff = diff;
    }
  }

  return index;
}

bool EnsureCorrectionLoaded()
{
  if (gCorrectionLoaded)
    return true;

  gCorrectionLoaded = TBcid::LoadCorrectionFactorsFromCSV(gCorrectionCSVPath);
  if (!gCorrectionLoaded)
    std::cerr << "TBwaveform - failed to load correction CSV: " << gCorrectionCSVPath << std::endl;

  return gCorrectionLoaded;
}

std::vector<float> BuildADCcorrectedWaveformF(const std::vector<short> &waveform, int drsStop, const TString &name)
{
  if (waveform.empty())
    return std::vector<float>();

  std::vector<float> result(waveform.begin(), waveform.end());

  if (!IsModuleTowerSCName(name))
    return result;

  if (!EnsureCorrectionLoaded())
    return result;

  const int patchIndex = GetPatchIndex(drsStop);
  const std::vector<double> *factors = TBcid::GetCachedCorrectionPtr(name, patchIndex);
  if (factors == nullptr || factors->empty())
    return result;

  for (size_t j = 0; j < waveform.size(); ++j)
  {
    int bin = static_cast<int>(j) + drsStop + 1;
    if (bin >= 1024)
      bin -= 1024;

    if (bin >= 0 && static_cast<size_t>(bin) < factors->size())
      result[j] = static_cast<float>(waveform[j] + (*factors)[static_cast<size_t>(bin)]);
  }

  return result;
}

std::vector<float> BuildADCcorrectedWaveformF(const std::vector<short> &waveform, int drsStop, const TString &name,const int index)
{
  if (waveform.empty())
    return std::vector<float>();

  std::vector<float> result(waveform.begin(), waveform.end());

  if (!IsModuleTowerSCName(name))
    return result;

  if (!EnsureCorrectionLoaded())
    return result;

  const int patchIndex = index;
  const std::vector<double> *factors = TBcid::GetCachedCorrectionPtr(name, patchIndex);
  if (factors == nullptr || factors->empty())
    return result;

  for (size_t j = 0; j < waveform.size(); ++j)
  {
    int bin = static_cast<int>(j) + drsStop + 1;
    if (bin >= 1024)
      bin -= 1024;

    if (bin >= 0 && static_cast<size_t>(bin) < factors->size())
      result[j] = static_cast<float>(waveform[j] + (*factors)[static_cast<size_t>(bin)]);
  }

  return result;
}

std::vector<double> BuildADCcorrectedWaveform(const std::vector<short> &waveform, int drsStop, const TString &name)
{
  if (waveform.empty())
    return std::vector<double>();

  std::vector<double> result(waveform.begin(), waveform.end());

  if (!IsModuleTowerSCName(name))
    return result;

  if (!EnsureCorrectionLoaded())
    return result;

  const int patchIndex = GetPatchIndex(drsStop);
  const std::vector<double> *factors = TBcid::GetCachedCorrectionPtr(name, patchIndex);
  if (factors == nullptr || factors->empty())
    return result;

  for (size_t j = 0; j < waveform.size(); ++j)
  {
    int bin = static_cast<int>(j) + drsStop + 1;
    if (bin >= 1024)
      bin -= 1024;

    if (bin >= 0 && static_cast<size_t>(bin) < factors->size())
      result[j] = static_cast<double>(waveform[j] + (*factors)[static_cast<size_t>(bin)]);
  }

  return result;
}

std::vector<double> BuildADCcorrectedWaveform(const std::vector<short> &waveform, int drsStop, const TString &name,const int index)
{
  if (waveform.empty())
    return std::vector<double>();

  std::vector<double> result(waveform.begin(), waveform.end());

  if (!IsModuleTowerSCName(name))
    return result;

  if (!EnsureCorrectionLoaded())
    return result;

  const int patchIndex = index;
  const std::vector<double> *factors = TBcid::GetCachedCorrectionPtr(name, patchIndex);
  if (factors == nullptr || factors->empty())
    return result;

  for (size_t j = 0; j < waveform.size(); ++j)
  {
    int bin = static_cast<int>(j) + drsStop + 1;
    if (bin >= 1024)
      bin -= 1024;

    if (bin >= 0 && static_cast<size_t>(bin) < factors->size())
      result[j] = static_cast<double>(waveform[j] + (*factors)[static_cast<size_t>(bin)]);
  }

  return result;
}

}

void TBwaveform::SetCorrectionCSVPath(const std::string &csvPath)
{
  gCorrectionCSVPath = csvPath;
  gCorrectionLoaded = false;
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

std::vector<float> TBwaveform::ADCcorrectedWaveformF() const
{
  return BuildADCcorrectedWaveformF(waveform_, drs_stop_, name_);
}

std::vector<float> TBwaveform::ADCcorrectedWaveformF(int index) const
{
  return BuildADCcorrectedWaveformF(waveform_, drs_stop_, name_, index);
}

std::vector<float> TBwaveform::ADCpedcorrectedWaveformF() const
{
  const auto corrected = ADCcorrectedWaveform();

  std::vector<float> pedresult;
  pedresult.reserve(corrected.size());

  float ped = 0;
  for (int i = 1; i < 101; i++)
    ped += static_cast<float>(corrected.at(i)) / 100.;

  for (unsigned idx = 0; idx < corrected.size(); idx++)
  {
    float abin = ped - static_cast<float>(corrected.at(idx));
    pedresult.emplace_back(abin);
  }

  return std::move(pedresult);
}

std::vector<float> TBwaveform::ADCpedcorrectedWaveformF(int index) const
{
  const auto corrected = ADCcorrectedWaveform(index);

  std::vector<float> pedresult;
  pedresult.reserve(corrected.size());

  float ped = 0;
  for (int i = 1; i < 101; i++)
    ped += static_cast<float>(corrected.at(i)) / 100.;

  for (unsigned idx = 0; idx < corrected.size(); idx++)
  {
    float abin = ped - static_cast<float>(corrected.at(idx));
    pedresult.emplace_back(abin);
  }

  return std::move(pedresult);
}

std::vector<double> TBwaveform::ADCcorrectedWaveform() const
{
  return BuildADCcorrectedWaveform(waveform_, drs_stop_, name_);
}

std::vector<double> TBwaveform::ADCcorrectedWaveform(int index) const
{
  return BuildADCcorrectedWaveform(waveform_, drs_stop_, name_, index);
}

std::vector<double> TBwaveform::ADCpedcorrectedWaveform() const
{
  const auto corrected = ADCcorrectedWaveform();

  std::vector<double> pedresult;
  pedresult.reserve(corrected.size());

  double ped = 0;
  for (int i = 1; i < 101; i++)
    ped += static_cast<double>(corrected.at(i)) / 100.;

  for (unsigned idx = 0; idx < corrected.size(); idx++)
  {
    double abin = ped - static_cast<double>(corrected.at(idx));
    pedresult.emplace_back(abin);
  }

  return std::move(pedresult);
}

std::vector<double> TBwaveform::ADCpedcorrectedWaveform(int index) const
{
  const auto corrected = ADCcorrectedWaveform(index);

  std::vector<double> pedresult;
  pedresult.reserve(corrected.size());

  double ped = 0;
  for (int i = 1; i < 101; i++)
    ped += static_cast<double>(corrected.at(i)) / 100.;

  for (unsigned idx = 0; idx < corrected.size(); idx++)
  {
    double abin = ped - static_cast<double>(corrected.at(idx));
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
