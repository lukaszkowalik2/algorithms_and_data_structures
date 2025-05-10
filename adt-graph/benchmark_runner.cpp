#include "adt-graph-matrix.hpp"
#include "adt-graph-list.hpp"

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
    bool averaged,
    const std::string& implementation) {

    std::string gp_script_full_path = BENCHMARKS_DIR_PATH + "/plot_" + op_name + "_" + implementation + ".gp";
    std::string data_file_for_gnuplot = "../data/data_" + op_name + "_" + implementation + ".txt";
    std::string chart_file_for_gnuplot = "../charts/plot_" + op_name + "_" + implementation + ".png";
    std::string ylabel = averaged ? ("Average " + ylabel_base) : ylabel_base;

    std::ofstream gpfile(gp_script_full_path);
    if (!gpfile.is_open()) {
        std::cerr << "Error: Could not open Gnuplot script file " << gp_script_full_path << std::endl;
        return;
    }
    gpfile << "set terminal png size 800,600 enhanced font 'Verdana,10'\n";
    gpfile << "set output '" << chart_file_for_gnuplot << "'\n";
    gpfile << "set title \"" << plot_title << " (" << implementation << ")\"\n";
    gpfile << "set xlabel \"" << xlabel << "\"\n";
    gpfile << "set ylabel \"" << ylabel << "\"\n";
    gpfile << "set grid\n";
    gpfile << "set key top left\n";
    gpfile << "plot '" << data_file_for_gnuplot << "' using 1:2 with linespoints title '" << op_name << "'\n";
    gpfile.close();
    std::cout << "Generated Gnuplot script: " << gp_script_full_path << std::endl;
}

void generate_combined_gnuplot_script(
    const std::string& op_name,
    const std::string& plot_title,
    const std::string& xlabel,
    const std::string& ylabel_base,
    bool averaged) {

    std::string gp_script_full_path = BENCHMARKS_DIR_PATH + "/plot_" + op_name + "_combined.gp";
    std::string matrix_data = "../data/data_" + op_name + "_matrix.txt";
    std::string list_data = "../data/data_" + op_name + "_list.txt";
    std::string chart_file_for_gnuplot = "../charts/plot_" + op_name + "_combined.png";
    std::string ylabel = averaged ? ("Average " + ylabel_base) : ylabel_base;

    std::ofstream gpfile(gp_script_full_path);
    if (!gpfile.is_open()) {
        std::cerr << "Error: Could not open Gnuplot script file " << gp_script_full_path << std::endl;
        return;
    }
    gpfile << "set terminal png size 800,600 enhanced font 'Verdana,10'\n";
    gpfile << "set output '" << chart_file_for_gnuplot << "'\n";
    gpfile << "set title \"" << plot_title << " Performance Comparison\"\n";
    gpfile << "set xlabel \"" << xlabel << "\"\n";
    gpfile << "set ylabel \"" << ylabel << "\"\n";
    gpfile << "set grid\n";
    gpfile << "set key top left\n";
    gpfile << "plot '" << matrix_data << "' using 1:2 with linespoints title 'Matrix Implementation', \\\n";
    gpfile << "     '" << list_data << "' using 1:2 with linespoints title 'List Implementation'\n";
    gpfile.close();
    std::cout << "Generated combined Gnuplot script: " << gp_script_full_path << std::endl;
}

void generate_category_gnuplot_script(
    const std::string& category_name,
    const std::vector<std::string>& operations,
    const std::string& plot_title,
    const std::string& xlabel,
    const std::string& ylabel) {

    std::string gp_script_full_path = BENCHMARKS_DIR_PATH + "/plot_" + category_name + ".gp";
    std::string chart_file_for_gnuplot = "../charts/plot_" + category_name + ".png";

    std::ofstream gpfile(gp_script_full_path);
    if (!gpfile.is_open()) {
        std::cerr << "Error: Could not open Gnuplot script file " << gp_script_full_path << std::endl;
        return;
    }

    gpfile << "set terminal png size 1200,800 enhanced font 'Verdana,10'\n";
    gpfile << "set output '" << chart_file_for_gnuplot << "'\n";
    gpfile << "set title \"" << plot_title << "\"\n";
    gpfile << "set xlabel \"" << xlabel << "\"\n";
    gpfile << "set ylabel \"" << ylabel << "\"\n";
    gpfile << "set grid\n";
    gpfile << "set key top left\n";
    gpfile << "set multiplot layout 2,1\n";

    // Matrix implementation plot
    gpfile << "set title \"Matrix Implementation\"\n";
    gpfile << "plot ";
    for (size_t i = 0; i < operations.size(); ++i) {
        const auto& op = operations[i];
        gpfile << "'../data/data_" << op << "_matrix.txt' using 1:2 with linespoints title '" << op << "'";
        if (i < operations.size() - 1) gpfile << ", \\\n";
    }
    gpfile << "\n";

    // List implementation plot
    gpfile << "set title \"List Implementation\"\n";
    gpfile << "plot ";
    for (size_t i = 0; i < operations.size(); ++i) {
        const auto& op = operations[i];
        gpfile << "'../data/data_" << op << "_list.txt' using 1:2 with linespoints title '" << op << "'";
        if (i < operations.size() - 1) gpfile << ", \\\n";
    }
    gpfile << "\n";

    gpfile << "unset multiplot\n";
    gpfile.close();
    std::cout << "Generated category Gnuplot script: " << gp_script_full_path << std::endl;
}

template<typename GraphType>
void run_benchmark(
    const std::string& op_name,
    int max_v,
    int repetitions,
    const std::string& plot_title_base,
    const std::string& xlabel,
    const std::string& ylabel,
    const std::string& implementation,
    std::function<void(GraphType&, VertexId, VertexId, int)> core_operation_logic,
    std::function<void(GraphType&, int)> prepare_graph_extras = nullptr) {

    std::string data_filename_abs = DATA_DIR_PATH + "/data_" + op_name + "_" + implementation + ".txt";
    std::ofstream outfile(data_filename_abs);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open data file " << data_filename_abs << std::endl;
        return;
    }
    outfile << "N Time\n";
    std::cout << "Running benchmark for: " << op_name << " (" << implementation << ") (Max V: " << max_v << ", Reps: " << repetitions << ")" << std::endl;

    for (int n_val = 0; n_val <= max_v; n_val += std::max(1, max_v / 100)) {
        int n = n_val == 0 ? 2 : n_val;

        GraphType g;
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
    std::cout << "Data for " << op_name << " (" << implementation << ") written to " << data_filename_abs << std::endl;
    generate_gnuplot_script(op_name, plot_title_base + " Performance", xlabel, ylabel, repetitions > 1, implementation);
}

template<typename GraphType>
void run_all_benchmarks(const std::string& implementation) {
    const int REP_AVG = 100000;
    const int REP_ONE = 1;

    run_benchmark<GraphType>("adjacent", 10000, REP_AVG, "adjacent(G, x, y)", "Number of Vertices (N)", "Time (microseconds)", implementation,
        [](GraphType& g, VertexId v1, VertexId v2, [[maybe_unused]] int n_val) {
            volatile bool res = g.adjacent(v1, v2); (void)res;
        },
        [](GraphType& g, int n_val){
            for(int i=0; i < n_val / 2; ++i) {
                if (n_val > 1) g.addEdge(rand()%n_val, rand()%n_val);
            }
        });

    run_benchmark<GraphType>("neighbours", 10000, REP_ONE, "neighbours(G, x)", "Number of Vertices (N)", "Time (microseconds)", implementation,
        [](GraphType& g, VertexId v1, [[maybe_unused]] VertexId v2, [[maybe_unused]] int n_val) {
            volatile std::vector<VertexId> res = g.neighbours(v1); (void)res;
        },
        [](GraphType& g, int n_val){ 
            for(int i=0; i < 2 * n_val; ++i) {
                 if (n_val > 1) g.addEdge(rand()%n_val, rand()%n_val);
            }
        });

    run_benchmark<GraphType>("addVertex", 10000, REP_ONE, "addVertex(G, v)", "Vertices before add (N)", "Time (microseconds)", implementation,
        [](GraphType& g, [[maybe_unused]] VertexId v1, [[maybe_unused]] VertexId v2, int n_val) {
            g.addVertex(n_val);
        });

    run_benchmark<GraphType>("removeVertex", 10000, REP_ONE, "removeVertex(G, v)", "Number of Vertices (N)", "Time (microseconds)", implementation,
        [](GraphType& g, VertexId v1, [[maybe_unused]] VertexId v2, [[maybe_unused]] int n_val) {
            if (g.vertexExists(v1)) g.removeVertex(v1);
        });

    run_benchmark<GraphType>("addEdge", 10000, REP_AVG, "addEdge(G, x, y, val)", "Number of Vertices (N)", "Time (microseconds)", implementation,
        [](GraphType& g, VertexId v1, VertexId v2, [[maybe_unused]] int n_val) {
            if (g.vertexExists(v1) && g.vertexExists(v2)) {
                g.addEdge(v1, v2, rand() % 100);
            }
        });

    run_benchmark<GraphType>("removeEdge", 10000, REP_AVG, "removeEdge(G, x, y)", "Number of Vertices (N)", "Time (microseconds)", implementation,
        [](GraphType& g, VertexId v1, VertexId v2, [[maybe_unused]] int n_val) {
            if (g.vertexExists(v1) && g.vertexExists(v2)) {
                 bool edge_existed_before_remove = g.adjacent(v1,v2);
                 if (!edge_existed_before_remove) g.addEdge(v1,v2);

                 g.removeEdge(v1, v2);

                 if (edge_existed_before_remove) g.addEdge(v1,v2);
            }
        });

    run_benchmark<GraphType>("getVertexValue", 10000, REP_AVG, "getVertexValue(G, x)", "Number of Vertices (N)", "Time (microseconds)", implementation,
        [](GraphType& g, VertexId v1, [[maybe_unused]] VertexId v2, [[maybe_unused]] int n_val) {
            if (g.vertexExists(v1)) {
                g.setVertexValue(v1, rand()%100);
                volatile std::optional<VertexValue> res = g.getVertexValue(v1); (void)res;
            }
        });

    run_benchmark<GraphType>("setVertexValue", 10000, REP_AVG, "setVertexValue(G, x, val)", "Number of Vertices (N)", "Time (microseconds)", implementation,
        [](GraphType& g, VertexId v1, [[maybe_unused]] VertexId v2, [[maybe_unused]] int n_val) {
            if (g.vertexExists(v1)) g.setVertexValue(v1, rand() % 100);
        });

    run_benchmark<GraphType>("getEdgeValue", 10000, REP_AVG, "getEdgeValue(G, x, y)", "Number of Vertices (N)", "Time (microseconds)", implementation,
        [](GraphType& g, VertexId v1, VertexId v2, [[maybe_unused]] int n_val) {
             if (g.vertexExists(v1) && g.vertexExists(v2)) {
                if(!g.adjacent(v1,v2)) g.addEdge(v1,v2, rand()%100);
                else g.setEdgeValue(v1,v2, rand()%100);
                volatile std::optional<EdgeValue> res = g.getEdgeValue(v1, v2); (void)res;
             }
        });

    run_benchmark<GraphType>("setEdgeValue", 10000, REP_AVG, "setEdgeValue(G, x, y, val)", "Number of Vertices (N)", "Time (microseconds)", implementation,
        [](GraphType& g, VertexId v1, VertexId v2, [[maybe_unused]] int n_val) {
            if (g.vertexExists(v1) && g.vertexExists(v2)) {
                if(!g.adjacent(v1,v2)) g.addEdge(v1,v2, rand()%100);
                g.setEdgeValue(v1, v2, rand() % 100);
            }
        });

    // After all benchmarks are done, generate combined plots
    if (implementation == "matrix") {
        // Generate individual operation comparison plots
        for (const auto& op : {"adjacent", "neighbours", "addVertex", "removeVertex", 
                              "addEdge", "removeEdge", "getVertexValue", "setVertexValue", 
                              "getEdgeValue", "setEdgeValue"}) {
            generate_combined_gnuplot_script(
                op,
                std::string(op) + "(G, ...)",
                "Number of Vertices (N)",
                "Time (microseconds)",
                op != "neighbours" && op != "addVertex" && op != "removeVertex"
            );
        }

        // Generate category comparison plots
        generate_category_gnuplot_script(
            "vertex_operations",
            {"addVertex", "removeVertex", "getVertexValue", "setVertexValue"},
            "Vertex Operations Performance Comparison",
            "Number of Vertices (N)",
            "Time (microseconds)"
        );

        generate_category_gnuplot_script(
            "edge_operations",
            {"addEdge", "removeEdge", "getEdgeValue", "setEdgeValue"},
            "Edge Operations Performance Comparison",
            "Number of Vertices (N)",
            "Time (microseconds)"
        );

        generate_category_gnuplot_script(
            "query_operations",
            {"adjacent", "neighbours"},
            "Query Operations Performance Comparison",
            "Number of Vertices (N)",
            "Time (microseconds)"
        );
    }
}

int main() {
    srand(time(nullptr));
    ensure_directory_exists(DATA_DIR_PATH);
    ensure_directory_exists(BENCHMARKS_DIR_PATH);
    ensure_directory_exists(CHARTS_DIR_PATH);

    // Run benchmarks for both implementations
    std::cout << "Running benchmarks for Matrix implementation..." << std::endl;
    run_all_benchmarks<ADTGraphMatrix>("matrix");

    std::cout << "Running benchmarks for List implementation..." << std::endl;
    run_all_benchmarks<ADTGraphList>("list");

    // Generate individual plots for each operation
    std::vector<std::string> operations = {
        "adjacent", "neighbours", "addVertex", "removeVertex",
        "addEdge", "removeEdge", "getVertexValue", "setVertexValue",
        "getEdgeValue", "setEdgeValue"
    };

    for (const auto& op : operations) {
        generate_gnuplot_script(op, op + " Performance", "Number of Vertices (N)", "Time (microseconds)", true, "matrix");
        generate_gnuplot_script(op, op + " Performance", "Number of Vertices (N)", "Time (microseconds)", true, "list");
        generate_combined_gnuplot_script(op, op + " Performance", "Number of Vertices (N)", "Time (microseconds)", true);
    }

    // Generate category plots
    std::vector<std::string> vertex_ops = {"addVertex", "removeVertex", "getVertexValue", "setVertexValue"};
    std::vector<std::string> edge_ops = {"addEdge", "removeEdge", "getEdgeValue", "setEdgeValue"};
    std::vector<std::string> query_ops = {"adjacent", "neighbours"};

    generate_category_gnuplot_script("vertex_operations", vertex_ops, "Vertex Operations Performance", "Number of Vertices (N)", "Time (microseconds)");
    generate_category_gnuplot_script("edge_operations", edge_ops, "Edge Operations Performance", "Number of Vertices (N)", "Time (microseconds)");
    generate_category_gnuplot_script("query_operations", query_ops, "Query Operations Performance", "Number of Vertices (N)", "Time (microseconds)");

    return 0;
}

