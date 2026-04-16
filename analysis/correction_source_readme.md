# Correction Factor Source 확장 안내 (CSV + ROOT)

## 개요
기존 CSV 기반 correction factor 로딩 경로를 유지하면서 ROOT 파일 로딩을 추가했습니다.
기본 source는 `CSV`이며, 설정 API로 `ROOT`로 전환할 수 있습니다.

## 추가된 설정 API (`TBwaveform`)
- `SetCorrectionSource("CSV" | "ROOT")`
- `SetCorrectionPathForMode(modeName, path)`
- `SetCorrectionROOTPathForMode(modeName, rootPath)`

기존 API는 그대로 동작합니다.
- `SetCorrectionCSVPath(...)`
- `SetCorrectionCSVPathForMode(...)`
- `SetCorrectionMode(...)`

## ROOT 포맷 명세
현재 구현은 ROOT 파일 내부에 **채널 key 이름과 동일한 `TH1` 객체**가 있다고 가정합니다.

- 히스토그램 이름: CSV의 `name` 컬럼과 동일한 key (`M1_T1_S_00`, `M1_T1_S`, ...)
- bin content: 각 bin의 correction factor
- 사용 bin 범위: `1..GetNbinsX()`

즉, `bin#_mean`에 해당하는 값은 해당 히스토그램의 bin content로 읽어 cache에 적재됩니다.

## 동작 방식
1. mode/source/path를 설정
2. waveform 보정 첫 호출에서 lazy-load (`gCorrectionLoaded`) 수행
3. 선택된 source(CSV/ROOT)로 factor를 읽어 공통 cache (`map<string, vector<double>>`)에 적재
4. mode별 key 규칙(PatchBased / AvgRefLine / FixRefLine)으로 조회

## 사용 예시
```cpp
TBwaveform::SetCorrectionMode("PatchBased");
TBwaveform::SetCorrectionSource("ROOT");
TBwaveform::SetCorrectionROOTPathForMode("PatchBased", "../th2d_means.root");

// 또는 source 기준 공통 API
TBwaveform::SetCorrectionPathForMode("PatchBased", "../th2d_means.root");
```

CSV로 되돌리기:
```cpp
TBwaveform::SetCorrectionSource("CSV");
TBwaveform::SetCorrectionCSVPathForMode("PatchBased", "../th2d_means.csv");
```
