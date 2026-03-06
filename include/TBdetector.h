#ifndef TBdetector_h
#define TBdetector_h 1

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include "TString.h"

class TBcid
{
public:
  TBcid(int midin, int channelin);
  TBcid();
  ~TBcid() {}

  bool operator<(const TBcid &rh) const;
  bool operator==(const TBcid &rh) const;

  int mid() const { return mid_; }
  int channel() const { return channel_; }
  void setName(const TString &name) {name_ = name; }
  const TString &name() const {return name_;}

  void print() const;

  static bool LoadCorrectionFactorsFromCSV(const std::string &csvPath);
  static bool HasCachedCorrection(const TString &name, int patch);
  static bool GetCachedCorrection(const TString &name, int patch, std::vector<double> &factors);
  static const std::vector<double> *GetCachedCorrectionPtr(const TString &name, int patch);

private:
  static std::string BuildCorrectionKey(const TString &name, int patch);

  int mid_;
  int channel_;
  TString name_;

  static std::map<std::string, std::vector<double>> correctionFactorsCache_;
  static bool correctionFactorsLoaded_;
};

class TBdetector
{
public:
  enum detid
  {
    nulldet = -1,
    aux = 0,
    ext = 4,
    ceren = 6,
    SFHS = 10,
    LEGO,
    MCPPMT_gen,
    MCPPMT,
    SiPM
  };

public:
  TBdetector();
  TBdetector(detid in);
  ~TBdetector() {}

  detid det() const { return det_; }
  int detType() const { return static_cast<int>(det_); }
  uint64_t id() const { return id_; }

  bool isSiPM() const { return det_ == detid::SiPM; }
  bool isMCPPMT() const { return det_ == detid::MCPPMT; }

  bool isSFHS() const { return det_ == detid::SFHS; }
  bool isLEGO() const { return det_ == detid::LEGO; }
  bool isMCPPMT_gen() const { return det_ == detid::MCPPMT_gen; }

  bool isGeneric() const { return isSFHS() || isLEGO() || isMCPPMT_gen(); }

  bool isNull() const { return det_ == detid::nulldet; }

  void encodeModule(int mod, int tow, bool isc);
  void encodeMultiCh(int row, int column);
  void encodeColumn(int column);

  int module() const;
  int tower() const;
  bool isCeren() const;
  int row() const;
  int column() const;

private:
  detid det_;
  uint64_t id_;
};

#endif
