#include "adt-graph-matrix.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <filesystem>

const std::string DATA_DIR_PATH = "data";
const std::string BENCHMARKS_DIR_PATH = "benchmarks";
const std::string CHARTS_DIR_PATH = "charts";

void ensure_directory_exists(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        if (!std::filesystem::create_directories(path)) {
            std::cerr << "Error: Could not create directory " << path << std::endl;
        }
    }
}

void generate_gnuplot_script(
    const std::string& op_name,
    const std::string& plot_title,
    const std::string& xlabel,
    const std::string& ylabel_base,
    bool averaged) {

    std::string gp_script_full_path = BENCHMARKS_DIR_PATH + "/plot_" + op_name + ".gp";
    std::string data_file_for_gnuplot = "../data/data_" + op_name + ".txt";
    std::string chart_file_for_gnuplot = "../charts/plot_" + op_name + ".png";
    std::string ylabel = averaged ? ("Average " + ylabel_base) : ylabel_base;

    std::ofstream gpfile(gp_script_full_path);
    if (!gpfile.is_open()) {
        std::cerr << "Error: Could not open Gnuplot script file " << gp_script_full_path << std::endl;
        return;
    }
    gpfile << "set terminal png size 800,600 enhanced font 'Verdana,10'\n";
    gpfile << "set output '" << chart_file_for_gnuplot << "'\n";
    gpfile << "set title \"" << plot_title << "\"\n";
    gpfile << "set xlabel \"" << xlabel << "\"\n";
    gpfile << "set ylabel \"" << ylabel << "\"\n";
    gpfile << "set grid\n";
    gpfile << "set key top left\n";
    gpfile << "plot '" << data_file_for_gnuplot << "' using 1:2 with linespoints title '" << op_name << "'\n";
    gpfile.close();
    std::cout << "Generated Gnuplot script: " << gp_script_full_path << std::endl;
}

void run_benchmark(
    const std::string& op_name,
    int max_v,
    int repetitions,
    const std::string& plot_title_base,
    const std::string& xlabel,
    const std::string& ylabel,
    std::function<void(ADTGraphMatrix&, VertexId, VertexId, int)> core_operation_logic,
    std::function<void(ADTGraphMatrix&, int)> prepare_graph_extras = nullptr) {

    std::string data_filename_abs = DATA_DIR_PATH + "/data_" + op_name + ".txt";
    std::ofstream outfile(data_filename_abs);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open data file " << data_filename_abs << std::endl;
        return;
    }
    outfile << "N Time\n";
    std::cout << "Running benchmark for: " << op_name << " (Max V: " << max_v << ", Reps: " << repetitions << ")" << std::endl;

    for (int n_val = 0; n_val <= max_v; n_val += std::max(1, max_v / 100)) {
      int n = n_val == 0 ? 2 : n_val;

        ADTGraphMatrix g;
        for (int i = 0; i < n; ++i) {
            g.addVertex(i);
        }

        if (prepare_graph_extras) {
            prepare_graph_extras(g, n);
        }

        VertexId v1 = (n > 0) ? rand() % n : 0;
        VertexId v2 = (n > 0) ? rand() % n : 0;
        if (n > 1 && v1 == v2) v2 = (v1 + 1) % n;

        if (op_name == "removeEdge" || op_name == "getEdgeValue" || op_name == "setEdgeValue") {
            if (g.vertexExists(v1) && g.vertexExists(v2) && !g.adjacent(v1,v2)) g.addEdge(v1, v2);
        }
         if (op_name == "removeVertex") {
             if (n > 0) v1 = n-1;
         }

        std::chrono::duration<double, std::micro> total_duration(0);
        if (repetitions > 1) {
            auto start_time = std::chrono::high_resolution_clock::now();
            for (int k = 0; k < repetitions; ++k) {
                core_operation_logic(g, v1, v2, n);
            }
            auto end_time = std::chrono::high_resolution_clock::now();
            total_duration = (end_time - start_time) / repetitions;
        } else {
            auto start_time = std::chrono::high_resolution_clock::now();
            core_operation_logic(g, v1, v2, n);
            auto end_time = std::chrono::high_resolution_clock::now();
            total_duration = end_time - start_time;
        }
        outfile << n << " " << std::fixed << std::setprecision(4) << total_duration.count() << std::endl;
    }
    outfile.close();
    std::cout << "Data for " << op_name << " written to " << data_filename_abs << std::endl;
    generate_gnuplot_script(op_name, plot_title_base + " Performance", xlabel, ylabel, repetitions > 1);
}


int main() {
    srand(time(nullptr));

    ensure_directory_exists(DATA_DIR_PATH);
    ensure_directory_exists(BENCHMARKS_DIR_PATH);
    ensure_directory_exists(CHARTS_DIR_PATH);

    const int REP_AVG = 100000;
    const int REP_ONE = 1;

    run_benchmark("adjacent", 10000, REP_AVG, "adjacent(G, x, y)", "Number of Vertices (N)", "Time (microseconds)",
        [](ADTGraphMatrix& g, VertexId v1, VertexId v2, int n_val) {
            volatile bool res = g.adjacent(v1, v2); (void)res;
        },
        [](ADTGraphMatrix& g, int n_val){
            for(int i=0; i < n_val / 2; ++i) {
                if (n_val > 1) g.addEdge(rand()%n_val, rand()%n_val);
            }
        });

    run_benchmark("neighbours", 10000, REP_ONE, "neighbours(G, x)", "Number of Vertices (N)", "Time (microseconds)",
        [](ADTGraphMatrix& g, VertexId v1, VertexId v2, int n_val) {
            volatile std::vector<VertexId> res = g.neighbours(v1); (void)res;
        },
        [](ADTGraphMatrix& g, int n_val){ 
            for(int i=0; i < 2 * n_val; ++i) {
                 if (n_val > 1) g.addEdge(rand()%n_val, rand()%n_val);
            }
        });

    run_benchmark("addVertex", 10000, REP_ONE, "addVertex(G, v)", "Vertices before add (N)", "Time (microseconds)",
        [](ADTGraphMatrix& g, VertexId v1, VertexId v2, int n_val) {
            g.addVertex(n_val);
        });

    run_benchmark("removeVertex", 10000, REP_ONE, "removeVertex(G, v)", "Number of Vertices (N)", "Time (microseconds)",
        [](ADTGraphMatrix& g, VertexId v1, VertexId v2, int n_val) {
            if (g.vertexExists(v1)) g.removeVertex(v1);
        });

    run_benchmark("addEdge", 10000, REP_AVG, "addEdge(G, x, y, val)", "Number of Vertices (N)", "Time (microseconds)",
        [](ADTGraphMatrix& g, VertexId v1, VertexId v2, int n_val) {
            if (g.vertexExists(v1) && g.vertexExists(v2)) {
                // To correctly average addEdge, we should remove it afterwards if it was new
                // Or, assume addEdge updates if edge exists.
                // For simplicity, we time the effect of addEdge call.
                g.addEdge(v1, v2, rand() % 100);
            }
        });

    run_benchmark("removeEdge", 10000, REP_AVG, "removeEdge(G, x, y)", "Number of Vertices (N)", "Time (microseconds)",
        [](ADTGraphMatrix& g, VertexId v1, VertexId v2, int n_val) {
            if (g.vertexExists(v1) && g.vertexExists(v2)) {
                 bool edge_existed_before_remove = g.adjacent(v1,v2);
                 if (!edge_existed_before_remove) g.addEdge(v1,v2); // Ensure edge exists for removal

                 g.removeEdge(v1, v2);

                 if (edge_existed_before_remove) g.addEdge(v1,v2); // Restore if it was there
                 else if (!g.adjacent(v1,v2) && g.vertexExists(v1) && g.vertexExists(v2)) {
                    // If it was added just for this test, and then removed, add it back
                    // This logic is tricky; simpler is to ensure edge exists via prepare_graph_extras
                 }
            }
        });

    run_benchmark("getVertexValue", 10000, REP_AVG, "getVertexValue(G, x)", "Number of Vertices (N)", "Time (microseconds)",
        [](ADTGraphMatrix& g, VertexId v1, VertexId v2, int n_val) {
            if (g.vertexExists(v1)) {
                g.setVertexValue(v1, rand()%100);
                volatile std::optional<VertexValue> res = g.getVertexValue(v1); (void)res;
            }
        });

    run_benchmark("setVertexValue", 10000, REP_AVG, "setVertexValue(G, x, val)", "Number of Vertices (N)", "Time (microseconds)",
        [](ADTGraphMatrix& g, VertexId v1, VertexId v2, int n_val) {
            if (g.vertexExists(v1)) g.setVertexValue(v1, rand() % 100);
        });

    run_benchmark("getEdgeValue", 10000, REP_AVG, "getEdgeValue(G, x, y)", "Number of Vertices (N)", "Time (microseconds)",
        [](ADTGraphMatrix& g, VertexId v1, VertexId v2, int n_val) {
             if (g.vertexExists(v1) && g.vertexExists(v2)) {
                if(!g.adjacent(v1,v2)) g.addEdge(v1,v2, rand()%100);
                else g.setEdgeValue(v1,v2, rand()%100);
                volatile std::optional<EdgeValue> res = g.getEdgeValue(v1, v2); (void)res;
             }
        });

    run_benchmark("setEdgeValue", 10000, REP_AVG, "setEdgeValue(G, x, y, val)", "Number of Vertices (N)", "Time (microseconds)",
        [](ADTGraphMatrix& g, VertexId v1, VertexId v2, int n_val) {
            if (g.vertexExists(v1) && g.vertexExists(v2)) {
                if(!g.adjacent(v1,v2)) g.addEdge(v1,v2, rand()%100); // Ensure edge exists
                g.setEdgeValue(v1, v2, rand() % 100);
            }
        });

    std::cout << "All benchmarks finished. Run 'make generate_all_plots' or 'make run_charts'." << std::endl;
    return 0;
}

