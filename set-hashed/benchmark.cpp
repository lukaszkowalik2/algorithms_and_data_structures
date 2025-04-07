#include <algorithm>
#include <cmath>
#include <filesystem>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "set-hashed.h"
#include "utilities.h"

const size_t maxNAnalysis = 1000;
const size_t numAnalysisPoints = 50;
const std::vector<std::string> operations = {
    "insert", "contains", "remove", "union", "intersection", "difference", "equals"};
const std::vector<size_t> tableSizesToTest = [] {
  std::vector<size_t> sizes(100);
  std::iota(sizes.begin(), sizes.end(), 1);

  sizes.push_back(maxNAnalysis / 10);
  sizes.push_back(maxNAnalysis / 5);
  sizes.push_back(maxNAnalysis / 2);
  sizes.push_back(maxNAnalysis);

  std::sort(sizes.begin(), sizes.end());
  sizes.erase(std::unique(sizes.begin(), sizes.end()), sizes.end());

  if (!sizes.empty() && sizes[0] == 0) {
    sizes[0] = 1;
  } else if (sizes.empty()) {
    sizes.push_back(1);
  }
  return sizes;
}();
const std::string dataDirectory = "data";

void analyzeOperation(const std::string &opName, size_t tableSize,
                      size_t maxN, size_t numPoints,
                      const std::string &dataDir,
                      std::function<void(setHashed &, const std::string &)> op,
                      std::function<void(setHashed &, setHashed &)> setOp = nullptr) {

  if (numPoints == 0)
    return;
  size_t step = maxN / numPoints;
  if (step == 0)
    step = 1;

  std::cout << "Analyzing: " << opName << ", Table Size: " << tableSize
            << ", Max N: " << maxN << ", Points: " << numPoints << std::endl;
  std::vector<std::pair<size_t, double>> dataPoints;
  Timer timer;
  std::vector<std::string> randomElements;
  setHashed uniqueChecker(maxN * 2);
  while (randomElements.size() < maxN + step + 10) {
    std::string s = generateRandomString();
    if (!uniqueChecker.contains(s)) {
      uniqueChecker.insert(s);
      randomElements.push_back(s);
    }
  }

  for (size_t n = step; n <= maxN; n += step) {
    if (n == 0)
      continue;
    setHashed testSet(tableSize);
    setHashed set2(tableSize);
    size_t currentElemIdx = 0;

    if (setOp) {
      size_t halfN = n / 2;
      for (size_t i = 0; i < halfN && currentElemIdx < randomElements.size(); ++i) {
        testSet.insert(randomElements[currentElemIdx++]);
      }
      for (size_t i = 0; i < halfN && currentElemIdx < randomElements.size(); ++i) {
        set2.insert(randomElements[currentElemIdx++]);
      }
      if (testSet.size() + set2.size() < n && n > 1)
        continue;
    } else {
      for (size_t i = 0; i < n - 1 && currentElemIdx < randomElements.size(); ++i) {
        testSet.insert(randomElements[currentElemIdx++]);
      }
      if (currentElemIdx >= randomElements.size())
        continue;
    }

    std::string elementForOp = (currentElemIdx < randomElements.size()) ? randomElements[currentElemIdx] : generateRandomString();

    timer.start();
    if (setOp) {
      setOp(testSet, set2);
    } else {
      op(testSet, elementForOp);
    }
    double elapsedTime = timer.stop();
    dataPoints.push_back({n, elapsedTime});
  }

  std::filesystem::path dataPath = dataDir;
  std::filesystem::path filename = opName + "_ts" + std::to_string(tableSize) + ".dat";
  std::filesystem::path fullPath = dataPath / filename;

  try {
    saveDataToFile(fullPath.string(), dataPoints);
  } catch (const std::runtime_error &e) {
    std::cerr << "Error saving file: " << e.what() << std::endl;
  }
}

int main() {
  std::cout << "--- Running setHashed Complexity Analysis (Max N = " << maxNAnalysis << ") ---\n";

  ensureDirectoryExists(dataDirectory);

  for (size_t ts : tableSizesToTest) {
    for (const std::string &opName : operations) {
      if (opName == "insert") {
        analyzeOperation(opName, ts, maxNAnalysis, numAnalysisPoints, dataDirectory,
                         [](setHashed &s, const std::string &elem) { s.insert(elem); });
      } else if (opName == "contains") {
        analyzeOperation(opName, ts, maxNAnalysis, numAnalysisPoints, dataDirectory,
                         [](setHashed &s, const std::string &elem) { s.contains(elem); });
      } else if (opName == "remove") {
        analyzeOperation(opName, ts, maxNAnalysis, numAnalysisPoints, dataDirectory,
                         [](setHashed &s, const std::string &elem) { s.remove(elem); });
      } else if (opName == "union") {
        analyzeOperation(opName, ts, maxNAnalysis, numAnalysisPoints, dataDirectory, nullptr,
                         [](setHashed &s1, setHashed &s2) { setHashed::setUnion(s1, s2); });
      } else if (opName == "intersection") {
        analyzeOperation(opName, ts, maxNAnalysis, numAnalysisPoints, dataDirectory, nullptr,
                         [](setHashed &s1, setHashed &s2) { setHashed::intersection(s1, s2); });
      } else if (opName == "difference") {
        analyzeOperation(opName, ts, maxNAnalysis, numAnalysisPoints, dataDirectory, nullptr,
                         [](setHashed &s1, setHashed &s2) { setHashed::difference(s1, s2); });
      } else if (opName == "equals") {
        analyzeOperation(opName, ts, maxNAnalysis, numAnalysisPoints, dataDirectory, nullptr,
                         [](setHashed &s1, setHashed &s2) { (void)s2; setHashed s1Copy = s1; setHashed::equals(s1, s1Copy); });
      }
    }
  }
  std::cout << "--- Complexity Analysis Finished ---\n";
  std::cout << "Generated .dat files in '" << dataDirectory << "' directory.\n\n";
  return 0;
}
