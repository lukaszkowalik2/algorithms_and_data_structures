#include "utilities.h"
#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>

bool ensureDirectoryExists(const std::string &path) {
  std::error_code ec;
  if (std::filesystem::exists(path, ec) && std::filesystem::is_directory(path, ec)) {
    return true;
  }
  if (!std::filesystem::create_directories(path, ec)) {
    if (ec && !(std::filesystem::exists(path, ec) && std::filesystem::is_directory(path, ec))) {
      std::cerr << "Error creating directory " << path << ": " << ec.message() << std::endl;
      return false;
    }
  }
  return std::filesystem::exists(path, ec) && std::filesystem::is_directory(path, ec);
}

std::string generateRandomString(size_t length) {
  const std::string characters =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<> distribution(0, characters.length() - 1);

  std::string randomString;
  randomString.reserve(length);
  for (size_t i = 0; i < length; ++i) {
    randomString += characters[distribution(generator)];
  }
  return randomString;
}

int generateRandomInt(int minVal, int maxVal) {
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<> distribution(minVal, maxVal);
  return distribution(generator);
}

void saveDataToFile(
    const std::string &filename,
    const std::vector<std::pair<size_t, double>> &data) {

  std::filesystem::path filepath(filename);
  std::filesystem::path dir = filepath.parent_path();

  if (!dir.empty() && !ensureDirectoryExists(dir.string())) {
    std::cerr << "Warning: Could not ensure directory exists for " << filename << std::endl;
  }

  std::ofstream outfile(filename);
  if (!outfile.is_open()) {
    throw std::runtime_error("Cannot open file for writing: " + filename);
  }
  outfile << "# N Time(s)\n";
  for (const auto &point : data) {
    outfile << point.first << " " << std::scientific << point.second << "\n";
  }
  outfile.close();
}

void Timer::start() {
  startTime = std::chrono::high_resolution_clock::now();
}

double Timer::stop() {
  auto endTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = endTime - startTime;
  return elapsed.count();
}
