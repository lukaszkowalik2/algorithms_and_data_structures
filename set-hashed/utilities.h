#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <utility>
#include <cstddef>

bool ensure_directory_exists(const std::string& path);

std::string generate_random_string(size_t length = 4);

void save_data_to_file(
    const std::string& filename,
    const std::vector<std::pair<size_t, double>>& data);

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;

public:
    void start();
    double stop();
};

#endif // UTILITIES_H
