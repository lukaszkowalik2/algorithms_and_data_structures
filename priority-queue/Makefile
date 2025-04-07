CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
LDFLAGS =

# Directories
BUILD_DIR = build
DATA_DIR = data
CHARTS_DIR = charts

# Executables (now defined with build path)
EXE_TESTS = $(BUILD_DIR)/program-tests
EXE_BENCHMARK = $(BUILD_DIR)/program-benchmark

# Common sources
COMMON_SOURCES = set-int-hashed.cpp utilities.cpp priority-queue.cpp priority-queue-binary.cpp
COMMON_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(COMMON_SOURCES))

# Test specific sources/objects
TESTS_SOURCES = tests.cpp
TESTS_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(TESTS_SOURCES))

# Benchmark specific sources/objects
BENCHMARK_SOURCES = benchmark.cpp
BENCHMARK_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(BENCHMARK_SOURCES))

# Gnuplot script
GNUPLOT_SCRIPT = plot_single.gp

# --- Configuration for Plotting (must match benchmark.cpp) ---
MAX_N = 1000
OPERATIONS = pop insert
PQ_TYPES = pq_set pq_binary
# Generate expected individual .dat and .png filenames
ALL_DATA_FILES = $(foreach type,$(PQ_TYPES),$(foreach op,$(OPERATIONS),$(DATA_DIR)/$(type)_$(op).dat))
CHART_FILES = $(patsubst $(DATA_DIR)/%.dat,$(CHARTS_DIR)/%.png,$(ALL_DATA_FILES))

# --- Targets ---

# Default: Build both executables
all: $(EXE_TESTS) $(EXE_BENCHMARK)

# Build test executable (output to build dir)
$(EXE_TESTS): $(COMMON_OBJECTS) $(TESTS_OBJECTS)
	@mkdir -p $(@D) # Ensure build dir exists
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Build benchmark executable (output to build dir)
$(EXE_BENCHMARK): $(COMMON_OBJECTS) $(BENCHMARK_OBJECTS)
	@mkdir -p $(@D) # Ensure build dir exists
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile object files into build directory
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Run Targets ---

# Tests
run: $(EXE_TESTS)
	./$(EXE_TESTS)

# Charts
run_charts: $(EXE_BENCHMARK)
	./$(EXE_BENCHMARK)
	$(MAKE) plots

# Target to generate all individual plots
plots: $(CHART_FILES)
	@echo "--- All charts generated in $(CHARTS_DIR) ---"

# --- Plotting Rule ---
$(CHARTS_DIR)/%.png: $(DATA_DIR)/%.dat $(GNUPLOT_SCRIPT)
	@mkdir -p $(CHARTS_DIR)
	@PQ_TYPE=$$(echo $(notdir $<) | sed -n 's/^\([a-z_]*\)_\([a-z]*\)\.dat/\1/p'); \
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
		$(GNUPLOT_SCRIPT) \
	 || echo "Warning: Gnuplot failed for '$$INPUT_FILE' -> '$$OUTPUT_FILE'."


# --- Clean Target ---
clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(DATA_DIR)
	@rm -rf $(CHARTS_DIR)

# --- Dependencies ---
$(BUILD_DIR)/tests.o: tests.cpp priority-queue.h priority-queue-binary.h utilities.h
$(BUILD_DIR)/benchmark.o: benchmark.cpp priority-queue.h priority-queue-binary.h utilities.h
$(BUILD_DIR)/priority-queue.o: priority-queue.cpp priority-queue.h set-int-hashed.h
$(BUILD_DIR)/priority-queue-binary.o: priority-queue-binary.cpp priority-queue-binary.h
$(BUILD_DIR)/set-int-hashed.o: set-int-hashed.cpp set-int-hashed.h
$(BUILD_DIR)/utilities.o: utilities.cpp utilities.h

# Declare phony targets
.PHONY: all run run_charts plots clean

