CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
LDFLAGS =

# Executables
EXE_TESTS = $(BUILD_DIR)/program-tests
EXE_SIMPLE_TESTS = $(BUILD_DIR)/program-simple-tests
EXE_BENCHMARK = $(BUILD_DIR)/program-benchmark
EXE_SIMPLE_BENCHMARK = $(BUILD_DIR)/program-simple-benchmark

# Common sources/objects
COMMON_SOURCES = utilities.cpp
COMMON_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(COMMON_SOURCES))

# setHashed specific sources/objects
SETHASHED_SOURCES = set-hashed.cpp
SETHASHED_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SETHASHED_SOURCES))

# simple-mapping specific sources/objects
SIMPLEMAP_SOURCES = simple-mapping.cpp
SIMPLEMAP_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SIMPLEMAP_SOURCES))

# Test specific sources/objects
TESTS_SOURCES = tests.cpp
TESTS_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(TESTS_SOURCES))
SIMPLE_TESTS_SOURCES = simple-tests.cpp
SIMPLE_TESTS_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SIMPLE_TESTS_SOURCES))

# Benchmark specific sources/objects
BENCHMARK_SOURCES = benchmark.cpp
BENCHMARK_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(BENCHMARK_SOURCES))
SIMPLE_BENCHMARK_SOURCES = simple-benchmark.cpp
SIMPLE_BENCHMARK_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SIMPLE_BENCHMARK_SOURCES))


# Directories
BUILD_DIR = build
DATA_DIR = data
CHARTS_DIR = charts

# Gnuplot script
GNUPLOT_SCRIPT = plot_script.gp

# --- Configuration for Plotting (must match benchmark.cpp) ---
MAX_N_HASHED = 1000
OPERATIONS_HASHED = insert contains remove union intersection difference equals
TABLE_SIZES_STATIC = $(shell seq 1 100)
TABLE_SIZES_PROPORTIONAL = $(shell echo $$(( $(MAX_N_HASHED) / 10 ))) $(shell echo $$(( $(MAX_N_HASHED) / 5 ))) $(shell echo $$(( $(MAX_N_HASHED) / 2 ))) $(shell echo $(MAX_N_HASHED))
ALL_TABLE_SIZES = $(sort $(TABLE_SIZES_STATIC) $(TABLE_SIZES_PROPORTIONAL))
DATA_FILES_HASHED = $(foreach op,$(OPERATIONS_HASHED),$(foreach ts,$(ALL_TABLE_SIZES),$(DATA_DIR)/$(op)_ts$(ts).dat))
CHART_FILES_HASHED = $(patsubst $(DATA_DIR)/%.dat,$(CHARTS_DIR)/%.png,$(DATA_FILES_HASHED))

OPERATIONS_SIMPLE = map_int_range_seq map_int_range_step2 map_char_az map_string_az2
DATA_FILES_SIMPLE = $(foreach op,$(OPERATIONS_SIMPLE),$(DATA_DIR)/$(op).dat)
CHART_FILES_SIMPLE = $(patsubst $(DATA_DIR)/%.dat,$(CHARTS_DIR)/%.png,$(DATA_FILES_SIMPLE))


# --- Targets ---

all: $(EXE_TESTS) $(EXE_SIMPLE_TESTS) $(EXE_BENCHMARK) $(EXE_SIMPLE_BENCHMARK)

$(EXE_TESTS): $(COMMON_OBJECTS) $(SETHASHED_OBJECTS) $(TESTS_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(EXE_SIMPLE_TESTS): $(COMMON_OBJECTS) $(SIMPLEMAP_OBJECTS) $(SIMPLE_TESTS_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(EXE_BENCHMARK): $(COMMON_OBJECTS) $(SETHASHED_OBJECTS) $(BENCHMARK_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(EXE_SIMPLE_BENCHMARK): $(COMMON_OBJECTS) $(SIMPLEMAP_OBJECTS) $(SIMPLE_BENCHMARK_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)


$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Run Targets ---

run: $(EXE_TESTS) $(EXE_SIMPLE_TESTS)
	./$(EXE_TESTS)
	./$(EXE_SIMPLE_TESTS)

run_charts: $(EXE_BENCHMARK)
	./$(EXE_BENCHMARK)
	$(MAKE) plots

run_simple_charts: $(EXE_SIMPLE_BENCHMARK)
	./$(EXE_SIMPLE_BENCHMARK)
	$(MAKE) plots_simple

plots: $(CHART_FILES_HASHED)
	@echo "--- setHashed charts generated in $(CHARTS_DIR) ---"

plots_simple: $(CHART_FILES_SIMPLE)
	@echo "--- Simple Mapping charts generated in $(CHARTS_DIR) ---"

# --- Plotting Rule (Simplified Pattern) ---
$(CHARTS_DIR)/%.png: $(DATA_DIR)/%.dat $(GNUPLOT_SCRIPT)
	@mkdir -p $(CHARTS_DIR)
	@if echo "$<" | grep -q '_ts[0-9]'; then \
	    OPERATION=$$(echo $(notdir $<) | sed -n 's/^\([a-z]*\)_ts.*/\1/p'); \
	    TS=$$(echo $(notdir $<) | sed -n 's/.*_ts\([0-9]*\)\.dat/\1/p'); \
	    XLABEL='Number of Elements (N)'; \
	    PLOT_TITLE="setHashed: $$OPERATION Performance (TS=$$TS, Max N=$(MAX_N_HASHED))"; \
	else \
	    OPERATION=$$(echo $(notdir $<) | sed -n 's/^\(.*\)\.dat/\1/p'); \
	    TS=-1; \
	    XLABEL='Number of Calls'; \
	    PLOT_TITLE="Mapping Function: $$OPERATION Performance"; \
	fi; \
	gnuplot \
		-e "input_file='$<'" \
		-e "output_file='$@'" \
		-e "operation='$$OPERATION'" \
		-e "table_size='$$TS'" \
		-e "plot_title='$$PLOT_TITLE'" \
		-e "xlabel_text='$$XLABEL'" \
		$(GNUPLOT_SCRIPT) \
	 || echo "Warning: Gnuplot failed for '$<' -> '$@'."


clean:
	@rm -f $(EXE_TESTS) $(EXE_SIMPLE_TESTS) $(EXE_BENCHMARK) $(EXE_SIMPLE_BENCHMARK)
	@rm -rf $(BUILD_DIR)
	@rm -rf $(DATA_DIR)
	@rm -rf $(CHARTS_DIR)

$(BUILD_DIR)/tests.o: tests.cpp set-hashed.h utilities.h
$(BUILD_DIR)/simple-tests.o: simple-tests.cpp simple-mapping.h
$(BUILD_DIR)/benchmark.o: benchmark.cpp set-hashed.h utilities.h
$(BUILD_DIR)/simple-benchmark.o: simple-benchmark.cpp simple-mapping.h utilities.h
$(BUILD_DIR)/set-hashed.o: set-hashed.cpp set-hashed.h
$(BUILD_DIR)/simple-mapping.o: simple-mapping.cpp simple-mapping.h
$(BUILD_DIR)/utilities.o: utilities.cpp utilities.h

.PHONY: all run run_tests run_simple_tests run_analysis run_simple_analysis plots plots_simple clean run_charts run_simple_charts

