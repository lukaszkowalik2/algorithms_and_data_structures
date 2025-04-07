CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
LDFLAGS =

EXE_TESTS = program-tests
EXE_BENCHMARK = program-benchmark

COMMON_SOURCES = set-hashed.cpp utilities.cpp
COMMON_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(COMMON_SOURCES))

TESTS_SOURCES = tests.cpp
TESTS_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(TESTS_SOURCES))

BENCHMARK_SOURCES = benchmark.cpp
BENCHMARK_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(BENCHMARK_SOURCES))

BUILD_DIR = build
DATA_DIR = data
CHARTS_DIR = charts

GNUPLOT_SCRIPT = plot_script.gp

MAX_N = 1000
OPERATIONS = insert contains remove union intersection difference equals
TABLE_SIZES_STATIC = $(shell seq 1 100)
TABLE_SIZES_PROPORTIONAL = $(shell echo $$(( $(MAX_N) / 10 ))) $(shell echo $$(( $(MAX_N) / 5 ))) $(shell echo $$(( $(MAX_N) / 2 ))) $(shell echo $(MAX_N))
ALL_TABLE_SIZES = $(sort $(TABLE_SIZES_STATIC) $(TABLE_SIZES_PROPORTIONAL))
DATA_FILES = $(foreach op,$(OPERATIONS),$(foreach ts,$(ALL_TABLE_SIZES),$(DATA_DIR)/$(op)_ts$(ts).dat))
CHART_FILES = $(patsubst $(DATA_DIR)/%.dat,$(CHARTS_DIR)/%.png,$(DATA_FILES))

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

plots: $(CHART_FILES)
	@echo "--- All charts generated in $(CHARTS_DIR) ---"

$(CHARTS_DIR)/%.png: $(DATA_DIR)/%.dat $(GNUPLOT_SCRIPT)
	@mkdir -p $(CHARTS_DIR)
	@OPERATION=$$(echo $(notdir $<) | sed -n 's/^\([a-z]*\)_ts.*/\1/p'); \
	 TS=$$(echo $(notdir $<) | sed -n 's/.*_ts\([0-9]*\)\.dat/\1/p'); \
	 INPUT_FILE="$<"; \
	 OUTPUT_FILE="$@"; \
	 PLOT_TITLE="setHashed: $$OPERATION Performance (TS=$$TS, Max N=$(MAX_N))"; \
	 gnuplot \
		-e "input_file='$$INPUT_FILE'" \
		-e "output_file='$$OUTPUT_FILE'" \
		-e "operation='$$OPERATION'" \
		-e "table_size='$$TS'" \
		-e "plot_title='$$PLOT_TITLE'" \
		$(GNUPLOT_SCRIPT) \
	 || echo "Warning: Gnuplot failed for '$$INPUT_FILE' -> '$$OUTPUT_FILE'."

clean:
	@rm -f $(EXE_TESTS) $(EXE_BENCHMARK)
	@rm -rf $(BUILD_DIR)
	@rm -rf $(DATA_DIR)
	@rm -rf $(CHARTS_DIR)

$(BUILD_DIR)/tests.o: tests.cpp set-hashed.h utilities.h
$(BUILD_DIR)/benchmark.o: benchmark.cpp set-hashed.h utilities.h
$(BUILD_DIR)/set-hashed.o: set-hashed.cpp set-hashed.h
$(BUILD_DIR)/utilities.o: utilities.cpp utilities.h

.PHONY: all run run_charts plots clean
