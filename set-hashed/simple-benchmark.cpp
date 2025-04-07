#include <algorithm>
#include <cmath>
#include <filesystem>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "simple-mapping.h"
#include "utilities.h"

const size_t MAX_CALLS = 1000000;
const size_t NUM_PLOT_POINTS = 50;
const std::string DATA_DIRECTORY = "data";

void benchmarkMappingFunction(
    const std::string &funcName,
    size_t maxCalls,
    size_t numPoints,
    const std::string &dataDir,
    std::function<size_t(void)> mappingCall) {
  if (numPoints == 0)
    return;
  size_t step = maxCalls / numPoints;
  if (step == 0)
    step = 1;

  std::cout << "Benchmarking Mapping Function: " << funcName
            << ", Max Calls: " << maxCalls << ", Points: " << numPoints << std::endl;

  std::vector<std::pair<size_t, double>> dataPoints;
  Timer timer;
  volatile size_t resultSink = 0;

  for (size_t nCalls = step; nCalls <= maxCalls; nCalls += step) {
    if (nCalls == 0)
      continue;

    timer.start();
    for (size_t i = 0; i < nCalls; ++i) {
      resultSink += mappingCall();
    }
    double elapsedTime = timer.stop();
    dataPoints.push_back({nCalls, elapsedTime});
  }

  std::filesystem::path dataPath = dataDir;
  std::filesystem::path filename = funcName + ".dat";
  std::filesystem::path fullPath = dataPath / filename;

  try {
    saveDataToFile(fullPath.string(), dataPoints);
  } catch (const std::runtime_error &e) {
    std::cerr << "Error saving file: " << e.what() << std::endl;
  }
}

int main() {
  std::cout << "--- Running Simple Mapping Function Benchmarks (Max Calls = " << MAX_CALLS << ") ---\n";
  ensureDirectoryExists(DATA_DIRECTORY);

  std::mt19937 rng(std::random_device{}());

  int nSeq = 100, mSeq = 1000;
  std::uniform_int_distribution<> distSeq(nSeq, mSeq);
  benchmarkMappingFunction("mapIntRangeSeq", MAX_CALLS, NUM_PLOT_POINTS, DATA_DIRECTORY,
                           [&]() { return mapIntRangeSeq(distSeq(rng), nSeq, mSeq); });

  int nStep = 50, mStep = 500;
  std::uniform_int_distribution<> distStepIdx(0, (mStep - nStep) / 2);
  benchmarkMappingFunction("mapIntRangeStep2", MAX_CALLS, NUM_PLOT_POINTS, DATA_DIRECTORY,
                           [&]() { int val = nStep + distStepIdx(rng) * 2; return mapIntRangeStep2(val, nStep, mStep); });

  std::uniform_int_distribution<> distCharIdx(0, 25);
  benchmarkMappingFunction("mapCharAz", MAX_CALLS, NUM_PLOT_POINTS, DATA_DIRECTORY,
                           [&]() { char c = 'a' + distCharIdx(rng); return mapCharAz(c); });

  std::uniform_int_distribution<> distCharIdx2(0, 25);
  benchmarkMappingFunction("mapStringAz2", MAX_CALLS, NUM_PLOT_POINTS, DATA_DIRECTORY,
                           [&]() {
                             std::string s = "aa";
                             s[0] = 'a' + distCharIdx2(rng);
                             s[1] = 'a' + distCharIdx2(rng);
                             return mapStringAz2(s);
                           });

  std::cout << "--- Simple Mapping Function Benchmarks Finished ---\n";
  std::cout << "Generated .dat files in '" << DATA_DIRECTORY << "' directory.\n\n";
  return 0;
}
