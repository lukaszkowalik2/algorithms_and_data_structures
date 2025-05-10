CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -I.
LDFLAGS =

BASE_DIR = .
BUILD_DIR = $(BASE_DIR)/build
BENCHMARK_SCRIPT_DIR = $(BASE_DIR)/benchmarks
DATA_DIR = $(BASE_DIR)/data
CHARTS_DIR = $(BASE_DIR)/charts

GRAPH_HEADER = $(BASE_DIR)/graph.hpp
GRAPH_SOURCE = $(BASE_DIR)/graph.cpp

TEST_EXECUTABLE = $(BUILD_DIR)/graph_test
TEST_SOURCE = $(BASE_DIR)/graph_test.cpp

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

EXPECTED_DATA_FILES = $(foreach op, $(OPERATIONS), $(DATA_DIR)/data_$(op).txt)
EXPECTED_GNUPLOT_SCRIPTS = $(foreach op, $(OPERATIONS), $(BENCHMARK_SCRIPT_DIR)/plot_$(op).gp)
PLOT_FILES = $(foreach op, $(OPERATIONS), $(CHARTS_DIR)/plot_$(op).png)

BENCHMARK_RUN_STAMP = $(BENCHMARK_SCRIPT_DIR)/.benchmark_run_completed

.PHONY: all clean directories run_tests run_charts compile_benchmark_runner execute_benchmark_runner generate_all_plots

all: run_tests run_charts

directories:
	@mkdir -p $(BUILD_DIR) $(BENCHMARK_SCRIPT_DIR) $(DATA_DIR) $(CHARTS_DIR)
	@echo "Directories ensured."

run_tests: $(TEST_EXECUTABLE)
	@echo "Running graph tests..."
	./$(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_SOURCE) $(GRAPH_SOURCE) $(GRAPH_HEADER) | directories
	@echo "Building tests: $(TEST_EXECUTABLE)..."
	$(CXX) $(CXXFLAGS) $(TEST_SOURCE) $(GRAPH_SOURCE) -o $(TEST_EXECUTABLE) $(LDFLAGS)

run_charts: directories compile_benchmark_runner execute_benchmark_runner generate_all_plots
	@echo "All charts processed. Check $(CHARTS_DIR)/"

compile_benchmark_runner: $(BENCHMARK_RUNNER_EXE)

$(BENCHMARK_RUNNER_EXE): $(BENCHMARK_RUNNER_SRC) $(GRAPH_SOURCE) $(GRAPH_HEADER) | directories
	@echo "Compiling benchmark runner: $(BENCHMARK_RUNNER_EXE)..."
	$(CXX) $(CXXFLAGS) $(BENCHMARK_RUNNER_SRC) $(GRAPH_SOURCE) -o $(BENCHMARK_RUNNER_EXE) $(LDFLAGS)

execute_benchmark_runner: $(BENCHMARK_RUN_STAMP)

$(BENCHMARK_RUN_STAMP): $(BENCHMARK_RUNNER_EXE) | directories
	@echo "Executing benchmark runner to generate data and gnuplot scripts..."
	./$(BENCHMARK_RUNNER_EXE)
	@touch $(BENCHMARK_RUN_STAMP)

generate_all_plots: $(PLOT_FILES)
	@echo "All PNG plots generated."

$(CHARTS_DIR)/plot_%.png: $(BENCHMARK_SCRIPT_DIR)/plot_%.gp $(DATA_DIR)/data_%.txt $(BENCHMARK_RUN_STAMP) | directories
	@echo "Generating plot: $@ from $(BENCHMARK_SCRIPT_DIR)/plot_$*.gp..."
	@mkdir -p $(CHARTS_DIR)
	cd $(BENCHMARK_SCRIPT_DIR) && gnuplot plot_$*.gp

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)
	rm -f $(EXPECTED_DATA_FILES)
	rm -f $(EXPECTED_GNUPLOT_SCRIPTS)
	rm -f $(PLOT_FILES)
	rm -f $(BENCHMARK_RUN_STAMP)
	@echo "Cleanup complete."
