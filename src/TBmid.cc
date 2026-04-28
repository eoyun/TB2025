#include "TBmid.h"

#include "TBdetector.h"

#include "stdio.h"
#include <numeric>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <map>
#include <cctype>

namespace {
enum class ADCorrectionMode
{
  None = 0,
  AvgRefLine,
  FixRefLine
};

enum class CorrectionSource
{
  CSV = 0,
  ROOT
};

ADCorrectionMode gCorrectionMode = ADCorrectionMode::None;
std::map<ADCorrectionMode, std::string> gCorrectionCSVPathByMode = {
    {ADCorrectionMode::AvgRefLine, "../correction_entire.csv"},
    {ADCorrectionMode::FixRefLine, "../correction_entire.csv"}};
std::map<ADCorrectionMode, std::string> gCorrectionROOTPathByMode = {
    {ADCorrectionMode::AvgRefLine, "../correction_entire.root"},
    {ADCorrectionMode::FixRefLine, "../correction_entire.root"}};
CorrectionSource gCorrectionSource = CorrectionSource::CSV;
bool gCorrectionLoaded = false;

bool IsModuleTowerSCName(const TString &name)
{
  return TBdetector::IsCorrectionChannelName(name);
}

const char *ModeToName(ADCorrectionMode mode)
{
  switch (mode)
  {
  case ADCorrectionMode::None:
    return "None";
  case ADCorrectionMode::AvgRefLine:
    return "AvgRefLine";
  case ADCorrectionMode::FixRefLine:
    return "FixRefLine";
  }

  return "Unknown";
}

ADCorrectionMode ParseModeName(const std::string &modeName)
{
  if (modeName.empty() || modeName == "None")
    return ADCorrectionMode::None;
  if (modeName == "AvgRefLine")
    return ADCorrectionMode::AvgRefLine;
  if (modeName == "FixRefLine")
    return ADCorrectionMode::FixRefLine;

  throw std::runtime_error("TBwaveform - unknown ADC correction mode: " + modeName);
}

const char *SourceToName(CorrectionSource source)
{
  switch (source)
  {
  case CorrectionSource::CSV:
    return "CSV";
  case CorrectionSource::ROOT:
    return "ROOT";
  }

  return "Unknown";
}

CorrectionSource ParseSourceName(const std::string &sourceName)
{
  std::string normalized;
  normalized.reserve(sourceName.size());
  for (char ch : sourceName)
    normalized.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));

  if (normalized.empty() || normalized == "CSV")
    return CorrectionSource::CSV;
  if (normalized == "ROOT")
    return CorrectionSource::ROOT;

  throw std::runtime_error("TBwaveform - unknown correction source: " + sourceName + " (supported: CSV, ROOT)");
}

std::map<ADCorrectionMode, std::string> &PathMapForSource(CorrectionSource source)
{
  if (source == CorrectionSource::CSV)
    return gCorrectionCSVPathByMode;

  return gCorrectionROOTPathByMode;
}

bool EnsureCorrectionLoaded()
{
  if (gCorrectionMode == ADCorrectionMode::None)
    return false;

  if (gCorrectionLoaded)
    return true;

  const auto &pathMap = PathMapForSource(gCorrectionSource);
  const auto it = pathMap.find(gCorrectionMode);
  if (it == pathMap.end() || it->second.empty())
  {
    throw std::runtime_error(std::string("TBwaveform - correction path is not configured for mode ")
                             + ModeToName(gCorrectionMode) + " and source " + SourceToName(gCorrectionSource));
  }

  gCorrectionLoaded = TBcid::LoadCorrectionFactors(it->second, SourceToName(gCorrectionSource));
  if (!gCorrectionLoaded)
  {
    throw std::runtime_error(std::string("TBwaveform - failed to load correction factors for mode ")
                             + ModeToName(gCorrectionMode) + ", source " + SourceToName(gCorrectionSource)
                             + ", path " + it->second);
  }

  return gCorrectionLoaded;
}

const std::vector<double> *GetCorrectionFactorsForCurrentMode(const TString &name, int drsStop, int forcedPatchIndex)
{
  (void)drsStop;
  (void)forcedPatchIndex;

  switch (gCorrectionMode)
  {
  case ADCorrectionMode::None:
    return nullptr;
  case ADCorrectionMode::AvgRefLine:
    return TBcid::GetCachedCorrectionPtr(Form("%s-mean", name.Data()));
  case ADCorrectionMode::FixRefLine:
    return TBcid::GetCachedCorrectionPtr(name);
  }

  throw std::runtime_error("TBwaveform - unsupported ADC correction mode state");
}

template <typename ValueT>
std::vector<ValueT> BuildADCcorrectedWaveformImpl(const std::vector<short> &waveform, int drsStop, const TString &name, int forcedPatchIndex = -1)
{
  if (waveform.empty())
    return std::vector<ValueT>();

  std::vector<ValueT> result(waveform.begin(), waveform.end());

  if (!IsModuleTowerSCName(name))
    return result;

  if (gCorrectionMode == ADCorrectionMode::None)
    return result;

  EnsureCorrectionLoaded();

  const std::vector<double> *factors = GetCorrectionFactorsForCurrentMode(name, drsStop, forcedPatchIndex);
  if (factors == nullptr || factors->empty())
  {
    throw std::runtime_error(std::string("TBwaveform - missing correction factors for channel ") + name.Data() +
                             " in mode " + ModeToName(gCorrectionMode));
  }

  for (size_t j = 0; j < waveform.size(); ++j)
  {
    int bin = static_cast<int>(j) + drsStop + 1;
    if (bin >= 1024)
      bin -= 1024;

    if (bin >= 0 && static_cast<size_t>(bin) < factors->size())
      result[j] = static_cast<ValueT>(waveform[j] + (*factors)[static_cast<size_t>(bin)]);
  }

  return result;
}

std::vector<float> BuildADCcorrectedWaveformF(const std::vector<short> &waveform, int drsStop, const TString &name)
{
  return BuildADCcorrectedWaveformImpl<float>(waveform, drsStop, name);
}

std::vector<float> BuildADCcorrectedWaveformF(const std::vector<short> &waveform, int drsStop, const TString &name,const int index)
{
  return BuildADCcorrectedWaveformImpl<float>(waveform, drsStop, name, index);
}

std::vector<double> BuildADCcorrectedWaveform(const std::vector<short> &waveform, int drsStop, const TString &name)
{
  return BuildADCcorrectedWaveformImpl<double>(waveform, drsStop, name);
}

std::vector<double> BuildADCcorrectedWaveform(const std::vector<short> &waveform, int drsStop, const TString &name,const int index)
{
  return BuildADCcorrectedWaveformImpl<double>(waveform, drsStop, name, index);
}

}

void TBwaveform::SetCorrectionCSVPath(const std::string &csvPath)
{
  gCorrectionCSVPathByMode[ADCorrectionMode::AvgRefLine] = csvPath;
  gCorrectionCSVPathByMode[ADCorrectionMode::FixRefLine] = csvPath;
  gCorrectionLoaded = false;
}

void TBwaveform::SetCorrectionMode(const std::string &modeName)
{
  gCorrectionMode = ParseModeName(modeName);
  gCorrectionLoaded = false;
}

void TBwaveform::SetCorrectionSource(const std::string &sourceType)
{
  gCorrectionSource = ParseSourceName(sourceType);
  gCorrectionLoaded = false;
}

void TBwaveform::SetCorrectionPathForMode(const std::string &modeName, const std::string &path)
{
  const ADCorrectionMode mode = ParseModeName(modeName);
  PathMapForSource(gCorrectionSource)[mode] = path;
  gCorrectionLoaded = false;
}

void TBwaveform::SetCorrectionCSVPathForMode(const std::string &modeName, const std::string &csvPath)
{
  const ADCorrectionMode mode = ParseModeName(modeName);
  gCorrectionCSVPathByMode[mode] = csvPath;
  gCorrectionLoaded = false;
}

void TBwaveform::SetCorrectionROOTPathForMode(const std::string &modeName, const std::string &rootPath)
{
  const ADCorrectionMode mode = ParseModeName(modeName);
  gCorrectionROOTPathByMode[mode] = rootPath;
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
