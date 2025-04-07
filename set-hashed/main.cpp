#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <cstdlib>
#include <sstream>
#include <numeric>

#include "set-hashed.h"
#include "utilities.h"

const size_t MAX_N_ANALYSIS = 1000;
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
const std::string GNUPLOT_SCRIPT_NAME = "plot_script.gp";


bool compare_element_vectors(std::vector<std::string> v1, std::vector<std::string> v2) {
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    return v1 == v2;
}

void run_correctness_tests() {
    std::cout << "--- Running setHashed Correctness Tests ---\n";
    setHashed s1(5);
    s1.insert("Test"); s1.insert("Data"); s1.insert("AAAA"); s1.insert("BBBB");
    assert(s1.size() == 4); assert(s1.contains("Test")); assert(!s1.contains("ZZZZ"));

    s1.insert("Test"); assert(s1.size() == 4);
    s1.remove("Data"); assert(s1.size() == 3); assert(!s1.contains("Data"));
    s1.remove("XXXX"); assert(s1.size() == 3);

    setHashed s2(5);
    s2.insert("AAAA"); s2.insert("CCCC"); s2.insert("DDDD");

    setHashed s_union = setHashed::setUnion(s1, s2);
    std::vector<std::string> expected_union = {"AAAA", "BBBB", "CCCC", "DDDD", "Test"};
    assert(compare_element_vectors(s_union.get_elements(), expected_union)); assert(s_union.size() == 5);

    setHashed s_intersection = setHashed::intersection(s1, s2);
    std::vector<std::string> expected_intersection = {"AAAA"};
    assert(compare_element_vectors(s_intersection.get_elements(), expected_intersection)); assert(s_intersection.size() == 1);

    setHashed s_diff1 = setHashed::difference(s1, s2);
    std::vector<std::string> expected_diff1 = {"BBBB", "Test"};
    assert(compare_element_vectors(s_diff1.get_elements(), expected_diff1)); assert(s_diff1.size() == 2);

    setHashed s3(5); s3.insert("Test"); s3.insert("AAAA"); s3.insert("BBBB");
    assert(setHashed::equals(s1, s3)); assert(!setHashed::equals(s1, s2));
    setHashed s4(10); s4.insert("Test"); s4.insert("AAAA"); s4.insert("BBBB");
    assert(setHashed::equals(s1, s4));
    std::cout << "Basic and Set Operation tests passed.\n";

    std::cout << "--- Correctness Tests Passed Successfully ---\n\n";
}

void analyze_operation(const std::string& op_name, size_t table_size,
                       size_t max_n, size_t step,
                       const std::string& data_dir,
                       std::function<void(setHashed&, const std::string&)> op,
                       std::function<void(setHashed&, setHashed&)> set_op = nullptr) {

    std::cout << "Analyzing: " << op_name << ", Table Size: " << table_size
              << ", Max N: " << max_n << std::endl;
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

void run_complexity_analysis(const std::string& data_dir) {
    std::cout << "--- Running setHashed Complexity Analysis (Max N = " << MAX_N_ANALYSIS << ") ---\n";
    size_t step = 50;

    ensure_directory_exists(data_dir);

    for (size_t ts : TABLE_SIZES_TO_TEST) {
        for (const std::string& op_name : OPERATIONS) {
            if (op_name == "insert") {
                analyze_operation(op_name, ts, MAX_N_ANALYSIS, step, data_dir,
                    [](setHashed& s, const std::string& elem){ s.insert(elem); });
            } else if (op_name == "contains") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, step, data_dir,
                    [](setHashed& s, const std::string& elem){ s.contains(elem); });
            } else if (op_name == "remove") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, step, data_dir,
                    [](setHashed& s, const std::string& elem){ s.remove(elem); });
            } else if (op_name == "union") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, step, data_dir, nullptr,
                    [](setHashed& s1, setHashed& s2){ setHashed::setUnion(s1, s2); });
            } else if (op_name == "intersection") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, step, data_dir, nullptr,
                    [](setHashed& s1, setHashed& s2){ setHashed::intersection(s1, s2); });
            } else if (op_name == "difference") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, step, data_dir, nullptr,
                    [](setHashed& s1, setHashed& s2){ setHashed::difference(s1, s2); });
            } else if (op_name == "equals") {
                 analyze_operation(op_name, ts, MAX_N_ANALYSIS, step, data_dir, nullptr,
                    [](setHashed& s1, setHashed& s2){ (void)s2; setHashed s1_copy = s1; setHashed::equals(s1, s1_copy); });
            }
        }
    }
    std::cout << "--- Complexity Analysis Finished ---\n";
    std::cout << "Generated .dat files in '" << data_dir << "' directory.\n\n";
}

void generate_plots(const std::string& data_dir, const std::string& charts_dir) {
    std::cout << "--- Generating Plots ---" << std::endl;
    ensure_directory_exists(charts_dir);

    if (!std::filesystem::exists(GNUPLOT_SCRIPT_NAME)) {
        std::cerr << "Error: Gnuplot script '" << GNUPLOT_SCRIPT_NAME << "' not found." << std::endl;
        return;
    }

    for (const std::string& op_name : OPERATIONS) {
        for (size_t ts : TABLE_SIZES_TO_TEST) {
            std::filesystem::path data_path = data_dir;
            std::filesystem::path data_filename = op_name + "_ts" + std::to_string(ts) + ".dat";
            std::filesystem::path input_filepath = data_path / data_filename;

            std::filesystem::path charts_path = charts_dir;
            std::filesystem::path chart_filename = op_name + "_ts" + std::to_string(ts) + ".png";
            std::filesystem::path output_filepath = charts_path / chart_filename;

            if (!std::filesystem::exists(input_filepath)) {
                std::cerr << "Warning: Data file not found, skipping plot: " << input_filepath.string() << std::endl;
                continue;
            }

            std::string plot_title = "setHashed: " + op_name + " Performance (TS=" + std::to_string(ts) + ", Max N=" + std::to_string(MAX_N_ANALYSIS) + ")";

            std::stringstream cmd_stream;
            cmd_stream << "gnuplot";
            cmd_stream << " -e \"input_file='" << input_filepath.string() << "'\"";
            cmd_stream << " -e \"output_file='" << output_filepath.string() << "'\"";
            cmd_stream << " -e \"operation='" << op_name << "'\"";
            cmd_stream << " -e \"table_size='" << ts << "'\"";
            cmd_stream << " -e \"plot_title='" << plot_title << "'\"";
            cmd_stream << " " << GNUPLOT_SCRIPT_NAME;

            std::string command = cmd_stream.str();
            std::cout << "Executing: " << command << std::endl;
            int ret = system(command.c_str());
            if (ret != 0) {
                std::cerr << "Warning: gnuplot command failed for " << input_filepath.string() << " (return code: " << ret << ")" << std::endl;
            }
        }
    }
    std::cout << "--- Plot Generation Finished ---" << std::endl;
    std::cout << "Generated .png files in '" << charts_dir << "' directory.\n\n";
}


int main(int argc, char* argv[]) {
    std::string mode = "all";
    std::string data_directory = "data";
    std::string charts_directory = "charts";

    if (argc > 1) {
        mode = argv[1];
    }

    bool run_tests_flag = (mode == "tests" || mode == "all");
    bool run_analysis_flag = (mode == "analysis" || mode == "all" || mode == "plots");
    bool run_plots_flag = (mode == "plots" || mode == "all");

    if (run_tests_flag) {
        run_correctness_tests();
    }

    if (run_analysis_flag) {
        run_complexity_analysis(data_directory);
    }

    if (run_plots_flag) {
        generate_plots(data_directory, charts_directory);
    }


    return 0;
}
