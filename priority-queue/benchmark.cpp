#include <algorithm>
#include <cmath>
#include <filesystem>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "priority-queue-binary.h"
#include "priority-queue.h"
#include "utilities.h"

const size_t maxNAnalysis = 10000;
const size_t numAnalysisPoints = 50;
const std::string dataDirectory = "data";

template <typename PQType>
void benchmarkPop(const std::string &pqName, size_t maxN, size_t numPoints, const std::string &dataDir) {
  if (numPoints == 0)
    return;
  size_t step = maxN / numPoints;
  if (step == 0)
    step = 1;

  std::cout << "Benchmarking Pop: " << pqName
            << ", Max N: " << maxN << ", Points: " << numPoints << std::endl;

  std::vector<std::pair<size_t, double>> dataPoints;
  Timer timer;
  std::mt19937 generator(12345);
  std::uniform_int_distribution<> distribution(0, maxN * 10);

  for (size_t n = step; n <= maxN; n += step) {
    if (n == 0)
      continue;
    PQType pq;
    for (size_t i = 0; i < n; ++i) {
      pq.insert(distribution(generator));
    }
    if (pq.isEmpty())
      continue;

    timer.start();
    try {
      pq.pop();
    } catch (const std::exception &e) {
      std::cerr << "Error during pop benchmark at N=" << n << ": " << e.what() << std::endl;
      continue;
    }
    double elapsedTime = timer.stop();
    dataPoints.push_back({n, elapsedTime});
  }

  std::filesystem::path dataPath = dataDir;
  std::filesystem::path filename = pqName + "_pop.dat";
  std::filesystem::path fullPath = dataPath / filename;

  try {
    saveDataToFile(fullPath.string(), dataPoints);
  } catch (const std::runtime_error &e) {
    std::cerr << "Error saving file: " << e.what() << std::endl;
  }
  std::cout << "Saved pop data to: " << fullPath.string() << std::endl;
}

template <typename PQType>
void benchmarkInsert(const std::string &pqName, size_t maxN, size_t numPoints, const std::string &dataDir) {
  if (numPoints == 0)
    return;
  size_t step = maxN / numPoints;
  if (step == 0)
    step = 1;

  std::cout << "Benchmarking Insert: " << pqName
            << ", Max N: " << maxN << ", Points: " << numPoints << std::endl;

  std::vector<std::pair<size_t, double>> dataPoints;
  Timer timer;
  std::mt19937 generator(54321);
  std::uniform_int_distribution<> distribution(0, maxN * 10);

  for (size_t n = step; n <= maxN; n += step) {
    if (n == 0)
      continue;
    PQType pq;
    for (size_t i = 0; i < n - 1; ++i) {
      pq.insert(distribution(generator));
    }
    int elementToInsert = distribution(generator);

    timer.start();
    try {
      pq.insert(elementToInsert);
    } catch (const std::exception &e) {
      std::cerr << "Error during insert benchmark at N=" << n << ": " << e.what() << std::endl;
      continue;
    }
    double elapsedTime = timer.stop();
    dataPoints.push_back({n, elapsedTime});
  }

  std::filesystem::path dataPath = dataDir;
  std::filesystem::path filename = pqName + "_insert.dat";
  std::filesystem::path fullPath = dataPath / filename;

  try {
    saveDataToFile(fullPath.string(), dataPoints);
  } catch (const std::runtime_error &e) {
    std::cerr << "Error saving file: " << e.what() << std::endl;
  }
  std::cout << "Saved insert data to: " << fullPath.string() << std::endl;
}

int main() {
  std::cout << "--- Running Priority Queue Benchmarks ---" << std::endl;

  ensureDirectoryExists(dataDirectory);

  benchmarkPop<PriorityQueue>("pq_set", maxNAnalysis, numAnalysisPoints, dataDirectory);
  benchmarkPop<PriorityQueueBinary>("pq_binary", maxNAnalysis, numAnalysisPoints, dataDirectory);

  benchmarkInsert<PriorityQueue>("pq_set", maxNAnalysis, numAnalysisPoints, dataDirectory);
  benchmarkInsert<PriorityQueueBinary>("pq_binary", maxNAnalysis, numAnalysisPoints, dataDirectory);

  std::cout << "--- Benchmarks Finished ---\n";
  std::cout << "Generated .dat files in '" << dataDirectory << "' directory.\n\n";
  return 0;
}
