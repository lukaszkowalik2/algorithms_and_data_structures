CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
LDFLAGS =

EXE_TESTS = program-tests
EXE_BENCHMARK = program-benchmark

COMMON_SOURCES = set-int-hashed.cpp utilities.cpp priority-queue.cpp priority-queue-binary.cpp
COMMON_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(COMMON_SOURCES))

TESTS_SOURCES = tests.cpp
TESTS_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(TESTS_SOURCES))

BENCHMARK_SOURCES = benchmark.cpp
BENCHMARK_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(BENCHMARK_SOURCES))

BUILD_DIR = build
DATA_DIR = data
CHARTS_DIR = charts

GNUPLOT_POP_COMPARE_SCRIPT = plot_pop_compare.gp
GNUPLOT_INSERT_COMPARE_SCRIPT = plot_insert_compare.gp
GNUPLOT_SINGLE_SCRIPT = plot_single.gp

MAX_N = 1000
OPERATIONS = pop insert
PQ_TYPES = pq_set pq_binary

DATA_POP_SET = $(DATA_DIR)/pq_set_pop.dat
DATA_POP_BINARY = $(DATA_DIR)/pq_binary_pop.dat
DATA_INSERT_SET = $(DATA_DIR)/pq_set_insert.dat
DATA_INSERT_BINARY = $(DATA_DIR)/pq_binary_insert.dat
ALL_DATA_FILES = $(DATA_POP_SET) $(DATA_POP_BINARY) $(DATA_INSERT_SET) $(DATA_INSERT_BINARY)

CHART_POP_COMPARE = $(CHARTS_DIR)/pop_comparison.png
CHART_INSERT_COMPARE = $(CHARTS_DIR)/insert_comparison.png
COMPARISON_CHART_FILES = $(CHART_POP_COMPARE) $(CHART_INSERT_COMPARE)

INDIVIDUAL_CHART_FILES = $(patsubst $(DATA_DIR)/%.dat,$(CHARTS_DIR)/%.png,$(ALL_DATA_FILES))

all: $(EXE_TESTS) $(EXE_BENCHMARK)

$(EXE_TESTS): $(COMMON_OBJECTS) $(TESTS_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(EXE_BENCHMARK): $(COMMON_OBJECTS) $(BENCHMARK_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(EXE_TESTS)
	./$(EXE_TESTS)

run_charts: $(EXE_BENCHMARK)
	./$(EXE_BENCHMARK)
	$(MAKE) plots
	$(MAKE) plots_individual

plots: $(COMPARISON_CHART_FILES)

plots_individual: $(INDIVIDUAL_CHART_FILES)

$(CHART_POP_COMPARE): $(DATA_POP_SET) $(DATA_POP_BINARY) $(GNUPLOT_POP_COMPARE_SCRIPT)
	@mkdir -p $(CHARTS_DIR)
	@gnuplot \
		-e "input_file_set='$(DATA_POP_SET)'" \
		-e "input_file_binary='$(DATA_POP_BINARY)'" \
		-e "output_file='$@'" \
		-e "plot_title='Priority Queue Pop Performance Comparison'" \
		$(GNUPLOT_POP_COMPARE_SCRIPT)

$(CHART_INSERT_COMPARE): $(DATA_INSERT_SET) $(DATA_INSERT_BINARY) $(GNUPLOT_INSERT_COMPARE_SCRIPT)
	@mkdir -p $(CHARTS_DIR)
	@gnuplot \
		-e "input_file_set='$(DATA_INSERT_SET)'" \
		-e "input_file_binary='$(DATA_INSERT_BINARY)'" \
		-e "output_file='$@'" \
		-e "plot_title='Priority Queue Insert Performance Comparison'" \
		$(GNUPLOT_INSERT_COMPARE_SCRIPT)

$(CHARTS_DIR)/%.png: $(DATA_DIR)/%.dat $(GNUPLOT_SINGLE_SCRIPT)
	@mkdir -p $(CHARTS_DIR)
	@if [ "$@" != "$(CHART_POP_COMPARE)" ] && [ "$@" != "$(CHART_INSERT_COMPARE)" ]; then \
		PQ_TYPE=$$(echo $(notdir $<) | sed -n 's/^\([a-z_]*\)_\([a-z]*\)\.dat/\1/p'); \
		OPERATION=$$(echo $(notdir $<) | sed -n 's/^\([a-z_]*\)_\([a-z]*\)\.dat/\2/p'); \
		INPUT_FILE="$<"; \
		OUTPUT_FILE="$@"; \
		PLOT_TITLE="Priority Queue Performance: $$PQ_TYPE / $$OPERATION"; \
		gnuplot \
			-e "input_file='$$INPUT_FILE'" \
			-e "output_file='$$OUTPUT_FILE'" \
			-e "pq_type='$$PQ_TYPE'" \
			-e "operation='$$OPERATION'" \
			-e "plot_title='$$PLOT_TITLE'" \
			$(GNUPLOT_SINGLE_SCRIPT); \
	fi

clean:
	@rm -f $(EXE_TESTS) $(EXE_BENCHMARK)
	@rm -rf $(BUILD_DIR)
	@rm -rf $(DATA_DIR)
	@rm -rf $(CHARTS_DIR)

$(BUILD_DIR)/tests.o: tests.cpp priority-queue.h priority-queue-binary.h utilities.h
$(BUILD_DIR)/benchmark.o: benchmark.cpp priority-queue.h priority-queue-binary.h utilities.h
$(BUILD_DIR)/priority-queue.o: priority-queue.cpp priority-queue.h set-int-hashed.h
$(BUILD_DIR)/priority-queue-binary.o: priority-queue-binary.cpp priority-queue-binary.h
$(BUILD_DIR)/set-int-hashed.o: set-int-hashed.cpp set-int-hashed.h
$(BUILD_DIR)/utilities.o: utilities.cpp utilities.h

.PHONY: all run run_charts plots plots_individual clean
