#include <limits.h>

#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <memory>
#include <stdexcept>

#include "TFile.h"
#include "TKey.h"
#include "TH1.h"

#include "TBdetector.h"


std::map<std::string, std::vector<double>> TBcid::correctionFactorsCache_;
bool TBcid::correctionFactorsLoaded_ = false;

namespace {
std::string Trim(const std::string &v)
{
  size_t start = 0;
  while (start < v.size() && std::isspace(static_cast<unsigned char>(v[start])))
    ++start;

  size_t end = v.size();
  while (end > start && std::isspace(static_cast<unsigned char>(v[end - 1])))
    --end;

  return v.substr(start, end - start);
}

bool EndsWith(const std::string &value, const std::string &suffix)
{
  return value.size() >= suffix.size() &&
         value.compare(value.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool IsCorrectionChannelRow(const std::string &rowName)
{
  TString trimmed(rowName.c_str());
  if (trimmed.EndsWith("_mean"))
    trimmed.Resize(trimmed.Length() - 5);
  else if (trimmed.EndsWith("-mean"))
    trimmed.Resize(trimmed.Length() - 5);

  if (TBdetector::IsCorrectionChannelName(trimmed))
    return true;

  if (rowName.find("_S_") != std::string::npos || rowName.find("_C_") != std::string::npos)
    return true;

  return EndsWith(rowName, "_S") || EndsWith(rowName, "_C");
}

std::string NormalizeSourceType(const std::string &sourceType)
{
  std::string normalized;
  normalized.reserve(sourceType.size());

  for (char ch : sourceType)
    normalized.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));

  return normalized;
}
}

std::string TBcid::BuildCorrectionKey(const TString &name, int patch)
{
  TString key = name;
  key.ReplaceAll("-", "_");

  return Form("%s_%02d", key.Data(), patch);
}

std::string TBcid::BuildCorrectionKey(const TString &name)
{
  TString key = name;
  key.ReplaceAll("-", "_");

  return std::string(key.Data());
}

bool TBcid::LoadCorrectionFactorsFromCSV(const std::string &csvPath)
{
  correctionFactorsCache_.clear();

  std::ifstream fin(csvPath.c_str());
  if (!fin.is_open())
    return false;

  std::string line;
  bool firstLine = true;

  while (std::getline(fin, line))
  {
    if (line.empty())
      continue;

    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ','))
      tokens.push_back(Trim(token));

    if (tokens.empty())
      continue;

    if (firstLine && tokens.at(0) == "name")
    {
      firstLine = false;
      continue;
    }
    firstLine = false;

    const std::string &rowName = tokens.at(0);
    if (!IsCorrectionChannelRow(rowName))
      continue;

    std::vector<double> factors;
    factors.reserve(tokens.size() > 1 ? tokens.size() - 1 : 0);

    for (size_t i = 1; i < tokens.size(); ++i)
    {
      if (tokens.at(i).empty())
        continue;
      factors.push_back(std::atof(tokens.at(i).c_str()));
    }

    if (!factors.empty())
      correctionFactorsCache_[rowName] = factors;
  }

  correctionFactorsLoaded_ = true;
  return !correctionFactorsCache_.empty();
}

bool TBcid::LoadCorrectionFactorsFromROOT(const std::string &rootPath)
{
  correctionFactorsCache_.clear();

  std::unique_ptr<TFile> fin(TFile::Open(rootPath.c_str(), "READ"));
  if (!fin || fin->IsZombie())
    return false;

  TIter next(fin->GetListOfKeys());
  TKey *key = nullptr;

  // ROOT format assumption:
  // - one TH1 per correction key (same naming convention as CSV "name" column)
  // - per-bin correction value stored in bin content
  while ((key = dynamic_cast<TKey *>(next())) != nullptr)
  {
    TObject *obj = key->ReadObj();
    TH1 *hist = dynamic_cast<TH1 *>(obj);
    if (hist == nullptr)
    {
      delete obj;
      continue;
    }

    const std::string histName = hist->GetName();
    if (!IsCorrectionChannelRow(histName))
    {
      delete obj;
      continue;
    }

    std::vector<double> factors;
    factors.reserve(static_cast<size_t>(hist->GetNbinsX()));
    for (int bin = 1; bin <= hist->GetNbinsX(); ++bin)
      factors.push_back(hist->GetBinContent(bin));

    if (!factors.empty())
      correctionFactorsCache_[histName] = factors;

    delete obj;
  }

  correctionFactorsLoaded_ = true;
  return !correctionFactorsCache_.empty();
}

bool TBcid::LoadCorrectionFactors(const std::string &path, const std::string &sourceType)
{
  const std::string source = NormalizeSourceType(sourceType);

  if (source.empty() || source == "CSV")
    return LoadCorrectionFactorsFromCSV(path);
  if (source == "ROOT")
    return LoadCorrectionFactorsFromROOT(path);

  throw std::runtime_error("TBcid - unsupported correction source type: " + sourceType);
}

bool TBcid::HasCachedCorrection(const TString &name, int patch)
{
  if (!correctionFactorsLoaded_)
    return false;

  return correctionFactorsCache_.find(BuildCorrectionKey(name, patch)) != correctionFactorsCache_.end();
}

bool TBcid::GetCachedCorrection(const TString &name, int patch, std::vector<double> &factors)
{
  factors.clear();

  const std::vector<double> *cached = GetCachedCorrectionPtr(name, patch);
  if (cached == nullptr)
    return false;

  factors = *cached;
  return true;
}


const std::vector<double> *TBcid::GetCachedCorrectionPtr(const TString &name, int patch)
{
  if (!correctionFactorsLoaded_)
    return nullptr;

  const auto it = correctionFactorsCache_.find(BuildCorrectionKey(name, patch));
  if (it == correctionFactorsCache_.end())
    return nullptr;

  return &(it->second);
}

const std::vector<double> *TBcid::GetCachedCorrectionPtr(const TString &name)
{
  if (!correctionFactorsLoaded_)
    return nullptr;

  const auto it = correctionFactorsCache_.find(BuildCorrectionKey(name));
  if (it == correctionFactorsCache_.end())
    return nullptr;

  return &(it->second);
}

TBcid::TBcid(int midin, int channelin)
    : mid_(midin), channel_(channelin), name_("") {}

TBcid::TBcid()
    : mid_(0), channel_(0), name_("") {}

bool TBcid::operator<(const TBcid &rh) const
{
  if (mid_ != rh.mid())
    return mid_ < rh.mid();

  return channel_ < rh.channel();
}

bool TBcid::operator==(const TBcid &rh) const
{
  return (channel_ == rh.channel()) && (mid_ == rh.mid());
}

void TBcid::print() const
{
  std::cout << "TBcid::mid()=" << mid_ << " TBcid::channel()=" << channel_ << "TBcid::name()=" <<name().Data() << std::endl;
}

TBdetector::TBdetector()
    : det_(TBdetector::detid::nulldet), id_(0) {}

TBdetector::TBdetector(TBdetector::detid in)
    : det_(in), id_(0) {}

void TBdetector::encodeModule(int mod, int tow, bool isc)
{
  uint32_t mod32 = static_cast<uint32_t>(mod) << 2 * sizeof(uint8_t) * CHAR_BIT;
  uint32_t tow32 = static_cast<uint32_t>(tow) << sizeof(uint8_t) * CHAR_BIT;
  uint32_t isc32 = static_cast<uint32_t>(isc);

  uint32_t val = mod32 | tow32 | isc32;
  uint64_t val64 = static_cast<uint64_t>(val) << sizeof(uint32_t) * CHAR_BIT;

  id_ = id_ & 0x00000000FFFFFFFF;
  id_ = val64 | id_;
}

void TBdetector::encodeMultiCh(int row, int column)
{
  uint32_t plate32 = static_cast<uint32_t>(row) << sizeof(uint16_t) * CHAR_BIT;
  uint32_t column32 = static_cast<uint32_t>(column);

  uint32_t val = plate32 | column32;
  uint64_t val64 = static_cast<uint64_t>(val);

  id_ = id_ & 0xFFFFFFFF00000000;
  id_ = val64 | id_;
}

void TBdetector::encodeColumn(int column)
{
  uint64_t val64 = static_cast<uint64_t>(static_cast<uint16_t>(column));

  id_ = id_ & 0xFFFFFFFFFFFF0000;
  id_ = val64 | id_;
}

// hard-coded!!!
int TBdetector::module() const
{
  uint64_t val64 = id_ & 0x00FF000000000000;
  return static_cast<int>(val64 >> 6 * sizeof(uint8_t) * CHAR_BIT);
}

int TBdetector::tower() const
{
  uint64_t val64 = id_ & 0x0000FF0000000000;
  return static_cast<int>(val64 >> 5 * sizeof(uint8_t) * CHAR_BIT);
}

bool TBdetector::isCeren() const
{
  uint64_t val64 = id_ & 0x000000FF00000000;
  return static_cast<bool>(val64 >> 4 * sizeof(uint8_t) * CHAR_BIT);
}

int TBdetector::row() const
{
  uint64_t val64 = id_ & 0x00000000FFFF0000;
  return static_cast<int>(val64 >> sizeof(uint16_t) * CHAR_BIT);
}

int TBdetector::column() const
{
  uint64_t val64 = id_ & 0x000000000000FFFF;
  return static_cast<int>(val64);
}

TBdetector::correction_channel_family TBdetector::ParseCorrectionChannelFamily(const TString &name)
{
  auto parseRange = [](const TString &input, const char *prefix, int min, int max) {
    if (!input.BeginsWith(prefix))
      return false;

    TString number = input;
    number.Remove(0, TString(prefix).Length());

    if (number.IsNull() || !number.IsDigit())
      return false;

    const int value = number.Atoi();
    return value >= min && value <= max;
  };

  if ((name.BeginsWith("M") && name.Contains("-T") && (name.EndsWith("-S") || name.EndsWith("-C"))) ||
      (name.BeginsWith("T") && (name.EndsWith("-S") || name.EndsWith("-C"))))
    return correction_channel_family::ModuleTower;

  if (parseRange(name, "LC", 1, 20))
    return correction_channel_family::LC;

  if (parseRange(name, "S", 1, 64))
    return correction_channel_family::S;

  if (parseRange(name, "C", 1, 64))
    return correction_channel_family::C;

  return correction_channel_family::Invalid;
}

bool TBdetector::IsCorrectionChannelName(const TString &name)
{
  return ParseCorrectionChannelFamily(name) != correction_channel_family::Invalid;
}
