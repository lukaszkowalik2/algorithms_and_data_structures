#ifndef UTILITIES_H
#define UTILITIES_H

#include <chrono>
#include <cstddef>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

bool ensureDirectoryExists(const std::string &path);

int generateRandomInt(int minVal = 0, int maxVal = 1000000);

void saveDataToFile(
    const std::string &filename,
    const std::vector<std::pair<size_t, double>> &data);

class Timer {
  private:
  std::chrono::high_resolution_clock::time_point startTime;

  public:
  void start();
  double stop();
};

#endif // UTILITIES_H
