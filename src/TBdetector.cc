#include <limits.h>

#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdlib>

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
}

std::string TBcid::BuildCorrectionKey(const TString &name, int patch)
{
  TString key = name;
  key.ReplaceAll("-", "_");

  return Form("%s_%02d", key.Data(), patch);
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
    if (rowName.find("_S_") == std::string::npos && rowName.find("_C_") == std::string::npos)
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
