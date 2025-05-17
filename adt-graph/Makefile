CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -I.
LDFLAGS =

BASE_DIR = .
BUILD_DIR = $(BASE_DIR)/build
BENCHMARK_SCRIPT_DIR = $(BASE_DIR)/benchmarks
DATA_DIR = $(BASE_DIR)/data
CHARTS_DIR = $(BASE_DIR)/charts

TRAFFIC_PHASES_SOURCE = $(BASE_DIR)/traffic_phases.cpp
TRAFFIC_PHASES_EXECUTABLE = $(BUILD_DIR)/traffic_phases

MATRIX_HEADER = $(BASE_DIR)/adt-graph-matrix.hpp
MATRIX_SOURCE = $(BASE_DIR)/adt-graph-matrix.cpp
MATRIX_TEST_EXECUTABLE = $(BUILD_DIR)/adt-graph-matrix-tests
MATRIX_TEST_SOURCE = $(BASE_DIR)/adt-graph-matrix-tests.cpp

LIST_HEADER = $(BASE_DIR)/adt-graph-list.hpp
LIST_SOURCE = $(BASE_DIR)/adt-graph-list.cpp
LIST_TEST_EXECUTABLE = $(BUILD_DIR)/adt-graph-list-tests
LIST_TEST_SOURCE = $(BASE_DIR)/adt-graph-list-tests.cpp

BENCHMARK_RUNNER_SRC = $(BASE_DIR)/benchmark_runner.cpp
BENCHMARK_RUNNER_EXE = $(BUILD_DIR)/benchmark_runner

OPERATIONS = \
    adjacent \
    neighbours \
    addVertex \
    removeVertex \
    addEdge \
    removeEdge \
    getVertexValue \
    setVertexValue \
    getEdgeValue \
    setEdgeValue

# Group operations by type for comparison charts
VERTEX_OPERATIONS = addVertex removeVertex getVertexValue setVertexValue
EDGE_OPERATIONS = addEdge removeEdge getEdgeValue setEdgeValue
QUERY_OPERATIONS = adjacent neighbours

EXPECTED_DATA_FILES = $(foreach op, $(OPERATIONS), $(DATA_DIR)/data_$(op)_matrix.txt $(DATA_DIR)/data_$(op)_list.txt)
EXPECTED_GNUPLOT_SCRIPTS = $(foreach op, $(OPERATIONS), $(BENCHMARK_SCRIPT_DIR)/plot_$(op)_matrix.gp $(BENCHMARK_SCRIPT_DIR)/plot_$(op)_list.gp $(BENCHMARK_SCRIPT_DIR)/plot_$(op)_combined.gp) \
                          $(BENCHMARK_SCRIPT_DIR)/plot_vertex_operations.gp \
                          $(BENCHMARK_SCRIPT_DIR)/plot_edge_operations.gp \
                          $(BENCHMARK_SCRIPT_DIR)/plot_query_operations.gp
PLOT_FILES = $(foreach op, $(OPERATIONS), $(CHARTS_DIR)/plot_$(op)_matrix.png $(CHARTS_DIR)/plot_$(op)_list.png $(CHARTS_DIR)/plot_$(op)_combined.png) \
             $(CHARTS_DIR)/plot_vertex_operations.png \
             $(CHARTS_DIR)/plot_edge_operations.png \
             $(CHARTS_DIR)/plot_query_operations.png

BENCHMARK_RUN_STAMP = $(BENCHMARK_SCRIPT_DIR)/.benchmark_run_completed

.PHONY: all clean directories run_tests run_charts compile_benchmark_runner execute_benchmark_runner generate_all_plots matrix list traffic_phases run_traffic_phases generate_traffic_graph

all: matrix list traffic_phases

matrix: run_matrix_tests run_matrix_charts

list: run_list_tests run_list_charts

directories:
	@mkdir -p $(BUILD_DIR) $(BENCHMARK_SCRIPT_DIR) $(DATA_DIR) $(CHARTS_DIR)

run_matrix_tests: $(MATRIX_TEST_EXECUTABLE)
	./$(MATRIX_TEST_EXECUTABLE)

$(MATRIX_TEST_EXECUTABLE): $(MATRIX_TEST_SOURCE) $(MATRIX_SOURCE) $(MATRIX_HEADER) | directories
	$(CXX) $(CXXFLAGS) $(MATRIX_TEST_SOURCE) $(MATRIX_SOURCE) -o $(MATRIX_TEST_EXECUTABLE) $(LDFLAGS)

run_matrix_charts: directories compile_benchmark_runner execute_benchmark_runner generate_all_plots

run_list_tests: $(LIST_TEST_EXECUTABLE)
	./$(LIST_TEST_EXECUTABLE)

$(LIST_TEST_EXECUTABLE): $(LIST_TEST_SOURCE) $(LIST_SOURCE) $(LIST_HEADER) | directories
	$(CXX) $(CXXFLAGS) $(LIST_TEST_SOURCE) $(LIST_SOURCE) -o $(LIST_TEST_EXECUTABLE) $(LDFLAGS)

run_list_charts: directories compile_benchmark_runner execute_benchmark_runner generate_all_plots

compile_benchmark_runner: $(BENCHMARK_RUNNER_EXE)

$(BENCHMARK_RUNNER_EXE): $(BENCHMARK_RUNNER_SRC) $(MATRIX_SOURCE) $(MATRIX_HEADER) $(LIST_SOURCE) $(LIST_HEADER) | directories
	$(CXX) $(CXXFLAGS) $(BENCHMARK_RUNNER_SRC) $(MATRIX_SOURCE) $(LIST_SOURCE) -o $(BENCHMARK_RUNNER_EXE) $(LDFLAGS)

execute_benchmark_runner: $(BENCHMARK_RUN_STAMP)

$(BENCHMARK_RUN_STAMP): $(BENCHMARK_RUNNER_EXE) | directories
	./$(BENCHMARK_RUNNER_EXE)
	@touch $(BENCHMARK_RUN_STAMP)

generate_all_plots: directories generate_matrix_plots generate_list_plots generate_combined_plots generate_category_plots

generate_matrix_plots: directories $(foreach op, $(OPERATIONS), $(CHARTS_DIR)/plot_$(op)_matrix.png)

generate_list_plots: directories $(foreach op, $(OPERATIONS), $(CHARTS_DIR)/plot_$(op)_list.png)

generate_combined_plots: directories $(foreach op, $(OPERATIONS), $(CHARTS_DIR)/plot_$(op)_combined.png)

generate_category_plots: directories $(CHARTS_DIR)/plot_vertex_operations.png $(CHARTS_DIR)/plot_edge_operations.png $(CHARTS_DIR)/plot_query_operations.png

run_charts: directories compile_benchmark_runner execute_benchmark_runner generate_all_plots

$(CHARTS_DIR)/plot_%_matrix.png: $(BENCHMARK_SCRIPT_DIR)/plot_%_matrix.gp $(DATA_DIR)/data_%_matrix.txt $(BENCHMARK_RUN_STAMP)
	@mkdir -p $(CHARTS_DIR)
	cd $(BENCHMARK_SCRIPT_DIR) && gnuplot plot_$*_matrix.gp

$(CHARTS_DIR)/plot_%_list.png: $(BENCHMARK_SCRIPT_DIR)/plot_%_list.gp $(DATA_DIR)/data_%_list.txt $(BENCHMARK_RUN_STAMP)
	@mkdir -p $(CHARTS_DIR)
	cd $(BENCHMARK_SCRIPT_DIR) && gnuplot plot_$*_list.gp

$(CHARTS_DIR)/plot_%_combined.png: $(BENCHMARK_SCRIPT_DIR)/plot_%_combined.gp $(DATA_DIR)/data_%_matrix.txt $(DATA_DIR)/data_%_list.txt $(BENCHMARK_RUN_STAMP)
	@mkdir -p $(CHARTS_DIR)
	cd $(BENCHMARK_SCRIPT_DIR) && gnuplot plot_$*_combined.gp

$(CHARTS_DIR)/plot_vertex_operations.png: $(BENCHMARK_SCRIPT_DIR)/plot_vertex_operations.gp $(BENCHMARK_RUN_STAMP)
	@mkdir -p $(CHARTS_DIR)
	cd $(BENCHMARK_SCRIPT_DIR) && gnuplot plot_vertex_operations.gp

$(CHARTS_DIR)/plot_edge_operations.png: $(BENCHMARK_SCRIPT_DIR)/plot_edge_operations.gp $(BENCHMARK_RUN_STAMP)
	@mkdir -p $(CHARTS_DIR)
	cd $(BENCHMARK_SCRIPT_DIR) && gnuplot plot_edge_operations.gp

$(CHARTS_DIR)/plot_query_operations.png: $(BENCHMARK_SCRIPT_DIR)/plot_query_operations.gp $(BENCHMARK_RUN_STAMP)
	@mkdir -p $(CHARTS_DIR)
	cd $(BENCHMARK_SCRIPT_DIR) && gnuplot plot_query_operations.gp

traffic_phases: $(TRAFFIC_PHASES_EXECUTABLE)

$(TRAFFIC_PHASES_EXECUTABLE): $(TRAFFIC_PHASES_SOURCE) $(LIST_SOURCE) $(LIST_HEADER) | directories
	$(CXX) $(CXXFLAGS) $(TRAFFIC_PHASES_SOURCE) $(LIST_SOURCE) -o $(TRAFFIC_PHASES_EXECUTABLE) $(LDFLAGS)

run_traffic_phases: $(TRAFFIC_PHASES_EXECUTABLE)
	./$(TRAFFIC_PHASES_EXECUTABLE)

generate_traffic_graph: run_traffic_phases
	dot -Tpng charts/traffic_phases.dot -o charts/traffic_phases.png

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(EXPECTED_DATA_FILES)
	rm -f $(EXPECTED_GNUPLOT_SCRIPTS)
	rm -f $(PLOT_FILES)
	rm -f $(BENCHMARK_RUN_STAMP)
	rm -f charts/traffic_phases.dot charts/traffic_phases.png
