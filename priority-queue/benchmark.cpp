#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <numeric>
#include <random>

#include "priority-queue.h"
#include "priority-queue-binary.h"
#include "utilities.h"

const size_t MAX_N_ANALYSIS = 10000;
const size_t NUM_ANALYSIS_POINTS = 50;
const std::string DATA_DIRECTORY = "data";

template<typename PQ_Type>
void benchmark_pop(const std::string& pq_name, size_t max_n, size_t num_points, const std::string& data_dir) {
    if (num_points == 0) return;
    size_t step = max_n / num_points;
    if (step == 0) step = 1;

    std::cout << "Benchmarking Pop: " << pq_name
              << ", Max N: " << max_n << ", Points: " << num_points << std::endl;

    std::vector<std::pair<size_t, double>> data_points;
    Timer timer;
    std::mt19937 generator(12345);
    std::uniform_int_distribution<> distribution(0, max_n * 10);

    for (size_t n = step; n <= max_n; n += step) {
        if (n == 0) continue;
        PQ_Type pq;
        for(size_t i = 0; i < n; ++i) {
            pq.insert(distribution(generator));
        }
        if (pq.empty()) continue;

        timer.start();
        try {
            pq.pop();
        } catch (const std::exception& e) {
             std::cerr << "Error during pop benchmark at N=" << n << ": " << e.what() << std::endl;
             continue;
        }
        double elapsed_time = timer.stop();
        data_points.push_back({n, elapsed_time});
    }

    std::filesystem::path data_path = data_dir;
    std::filesystem::path filename = pq_name + "_pop.dat";
    std::filesystem::path full_path = data_path / filename;

    try {
        save_data_to_file(full_path.string(), data_points);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error saving file: " << e.what() << std::endl;
    }
     std::cout << "Saved pop data to: " << full_path.string() << std::endl;
}

template<typename PQ_Type>
void benchmark_insert(const std::string& pq_name, size_t max_n, size_t num_points, const std::string& data_dir) {
    if (num_points == 0) return;
    size_t step = max_n / num_points;
    if (step == 0) step = 1;

     std::cout << "Benchmarking Insert: " << pq_name
              << ", Max N: " << max_n << ", Points: " << num_points << std::endl;

    std::vector<std::pair<size_t, double>> data_points;
    Timer timer;
    std::mt19937 generator(54321);
    std::uniform_int_distribution<> distribution(0, max_n * 10);

    for (size_t n = step; n <= max_n; n += step) {
         if (n == 0) continue;
         PQ_Type pq;
         for(size_t i = 0; i < n - 1; ++i) {
             pq.insert(distribution(generator));
         }
         int element_to_insert = distribution(generator);

         timer.start();
         try {
            pq.insert(element_to_insert);
         } catch (const std::exception& e) {
             std::cerr << "Error during insert benchmark at N=" << n << ": " << e.what() << std::endl;
             continue;
         }
         double elapsed_time = timer.stop();
         data_points.push_back({n, elapsed_time});
    }

    std::filesystem::path data_path = data_dir;
    std::filesystem::path filename = pq_name + "_insert.dat";
    std::filesystem::path full_path = data_path / filename;

    try {
        save_data_to_file(full_path.string(), data_points);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error saving file: " << e.what() << std::endl;
    }
    std::cout << "Saved insert data to: " << full_path.string() << std::endl;
}


int main() {
    std::cout << "--- Running Priority Queue Benchmarks ---" << std::endl;

    ensure_directory_exists(DATA_DIRECTORY);

    benchmark_pop<priorityQueue>("pq_set", MAX_N_ANALYSIS, NUM_ANALYSIS_POINTS, DATA_DIRECTORY);
    benchmark_pop<priorityQueueBinary>("pq_binary", MAX_N_ANALYSIS, NUM_ANALYSIS_POINTS, DATA_DIRECTORY);

    benchmark_insert<priorityQueue>("pq_set", MAX_N_ANALYSIS, NUM_ANALYSIS_POINTS, DATA_DIRECTORY);
    benchmark_insert<priorityQueueBinary>("pq_binary", MAX_N_ANALYSIS, NUM_ANALYSIS_POINTS, DATA_DIRECTORY);

    std::cout << "--- Benchmarks Finished ---\n";
    std::cout << "Generated .dat files in '" << DATA_DIRECTORY << "' directory.\n\n";
    return 0;
}
