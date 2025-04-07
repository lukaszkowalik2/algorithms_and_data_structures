#include "utilities.h"
#include <random>
#include <algorithm>
#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <iomanip>

bool ensure_directory_exists(const std::string& path) {
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


std::string generate_random_string(size_t length) {
    const std::string characters =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, characters.length() - 1);

    std::string random_string;
    random_string.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        random_string += characters[distribution(generator)];
    }
    return random_string;
}

void save_data_to_file(
    const std::string& filename,
    const std::vector<std::pair<size_t, double>>& data) {

    std::filesystem::path filepath(filename);
    std::filesystem::path dir = filepath.parent_path();

    if (!dir.empty() && !ensure_directory_exists(dir.string())) {
         std::cerr << "Warning: Could not ensure directory exists for " << filename << std::endl;
    }

    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    outfile << "# N Time(s)\n";
    for (const auto& point : data) {
        outfile << point.first << " " << std::scientific << point.second << "\n";
    }
    outfile.close();
}

void Timer::start() {
    start_time = std::chrono::high_resolution_clock::now();
}

double Timer::stop() {
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    return elapsed.count();
}
