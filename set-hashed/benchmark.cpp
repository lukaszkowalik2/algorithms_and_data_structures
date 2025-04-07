#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <numeric>

#include "set-hashed.h"
#include "utilities.h"

const size_t MAX_N_ANALYSIS = 1000;
const size_t NUM_ANALYSIS_POINTS = 50;
const std::vector<std::string> OPERATIONS = {
    "insert", "contains", "remove", "union", "intersection", "difference", "equals"
};
const std::vector<size_t> TABLE_SIZES_TO_TEST = []{
    std::vector<size_t> sizes(100);
    std::iota(sizes.begin(), sizes.end(), 1);

    sizes.push_back(MAX_N_ANALYSIS / 10);
    sizes.push_back(MAX_N_ANALYSIS / 5);
    sizes.push_back(MAX_N_ANALYSIS / 2);
    sizes.push_back(MAX_N_ANALYSIS);

    std::sort(sizes.begin(), sizes.end());
    sizes.erase(std::unique(sizes.begin(), sizes.end()), sizes.end());

    if (!sizes.empty() && sizes[0] == 0) {
         sizes[0] = 1;
    } else if (sizes.empty()) {
        sizes.push_back(1);
    }
    return sizes;
}();
const std::string DATA_DIRECTORY = "data";


void analyze_operation(const std::string& op_name, size_t table_size,
                       size_t max_n, size_t num_points,
                       const std::string& data_dir,
                       std::function<void(setHashed&, const std::string&)> op,
                       std::function<void(setHashed&, setHashed&)> set_op = nullptr) {

    if (num_points == 0) return;
    size_t step = max_n / num_points;
    if (step == 0) step = 1;

    std::cout << "Analyzing: " << op_name << ", Table Size: " << table_size
              << ", Max N: " << max_n << ", Points: " << num_points << std::endl;
    std::vector<std::pair<size_t, double>> data_points;
    Timer timer;
    std::vector<std::string> random_elements;
    setHashed unique_checker(max_n * 2);
    while(random_elements.size() < max_n + step + 10) {
        std::string s = generate_random_string();
        if (!unique_checker.contains(s)) {
            unique_checker.insert(s);
            random_elements.push_back(s);
        }
    }

    for (size_t n = step; n <= max_n; n += step) {
        if (n == 0) continue;
        setHashed test_set(table_size);
        setHashed set2(table_size);
        size_t current_elem_idx = 0;

        if (set_op) {
             size_t half_n = n / 2;
             for (size_t i = 0; i < half_n && current_elem_idx < random_elements.size(); ++i) {
                 test_set.insert(random_elements[current_elem_idx++]);
             }
             for (size_t i = 0; i < half_n && current_elem_idx < random_elements.size(); ++i) {
                 set2.insert(random_elements[current_elem_idx++]);
             }
             if (test_set.size() + set2.size() < n && n > 1) continue;
        } else {
            for (size_t i = 0; i < n - 1 && current_elem_idx < random_elements.size(); ++i) {
                 test_set.insert(random_elements[current_elem_idx++]);
            }
            if (current_elem_idx >= random_elements.size()) continue;
        }

        std::string element_for_op = (current_elem_idx < random_elements.size()) ? random_elements[current_elem_idx] : generate_random_string();

        timer.start();
        if (set_op) {
            set_op(test_set, set2);
        } else {
            op(test_set, element_for_op);
        }
        double elapsed_time = timer.stop();
        data_points.push_back({n, elapsed_time});
    }

    std::filesystem::path data_path = data_dir;
    std::filesystem::path filename = op_name + "_ts" + std::to_string(table_size) + ".dat";
    std::filesystem::path full_path = data_path / filename;

    try {
        save_data_to_file(full_path.string(), data_points);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error saving file: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "--- Running setHashed Complexity Analysis (Max N = " << MAX_N_ANALYSIS << ") ---\n";

    ensure_directory_exists(DATA_DIRECTORY);

    for (size_t ts : TABLE_SIZES_TO_TEST) {
        for (const std::string& op_name : OPERATIONS) {
            if (op_name == "insert") {
                analyze_operation(op_name, ts, MAX_N_ANALYSIS, NUM_ANALYSIS_POINTS, DATA_DIRECTORY,
                    [](setHashed& s, const std::string& elem){ s.insert(elem); });
            } else if (op_name == "contains") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, NUM_ANALYSIS_POINTS, DATA_DIRECTORY,
                    [](setHashed& s, const std::string& elem){ s.contains(elem); });
            } else if (op_name == "remove") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, NUM_ANALYSIS_POINTS, DATA_DIRECTORY,
                    [](setHashed& s, const std::string& elem){ s.remove(elem); });
            } else if (op_name == "union") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, NUM_ANALYSIS_POINTS, DATA_DIRECTORY, nullptr,
                    [](setHashed& s1, setHashed& s2){ setHashed::setUnion(s1, s2); });
            } else if (op_name == "intersection") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, NUM_ANALYSIS_POINTS, DATA_DIRECTORY, nullptr,
                    [](setHashed& s1, setHashed& s2){ setHashed::intersection(s1, s2); });
            } else if (op_name == "difference") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, NUM_ANALYSIS_POINTS, DATA_DIRECTORY, nullptr,
                    [](setHashed& s1, setHashed& s2){ setHashed::difference(s1, s2); });
            } else if (op_name == "equals") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, NUM_ANALYSIS_POINTS, DATA_DIRECTORY, nullptr,
                    [](setHashed& s1, setHashed& s2){ (void)s2; setHashed s1_copy = s1; setHashed::equals(s1, s1_copy); });
            }
        }
    }
    std::cout << "--- Complexity Analysis Finished ---\n";
    std::cout << "Generated .dat files in '" << DATA_DIRECTORY << "' directory.\n\n";
    return 0;
}
