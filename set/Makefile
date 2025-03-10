CXX = g++
CXXFLAGS = -O2 -std=c++11 -Wall

BUILD_DIR = build
SIMPLE_TARGET = $(BUILD_DIR)/set_simple
LINKED_TARGET = $(BUILD_DIR)/set_linked
DICTIONARY_TARGET = $(BUILD_DIR)/dictionary_simple
BENCHMARK_TARGET = benchmark

SIMPLE_SRC = set-simple.cpp
LINKED_SRC = set-linked.cpp
DICTIONARY_SRC = dictionary-simple.cpp
BENCHMARK_SRC = benchmark.cpp

all: $(BUILD_DIR) $(SIMPLE_TARGET) $(LINKED_TARGET) $(DICTIONARY_TARGET) $(BENCHMARK_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SIMPLE_TARGET): $(SIMPLE_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SIMPLE_SRC) -o $(SIMPLE_TARGET)

$(LINKED_TARGET): $(LINKED_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LINKED_SRC) -o $(LINKED_TARGET)

$(DICTIONARY_TARGET): $(DICTIONARY_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DICTIONARY_SRC) -o $(DICTIONARY_TARGET)

$(BENCHMARK_TARGET): $(BENCHMARK_SRC) headers/set-linked.hpp headers/set-simple.hpp headers/dictionary-simple.hpp
	$(CXX) $(CXXFLAGS) -o $(BENCHMARK_TARGET) $(BENCHMARK_SRC)

.PHONY: clean run_simple run_linked run_dictionary run_benchmark all charts clean_charts

run_simple: $(SIMPLE_TARGET)
	$(SIMPLE_TARGET)

run_linked: $(LINKED_TARGET)
	$(LINKED_TARGET)

run_dictionary: $(DICTIONARY_TARGET)
	$(DICTIONARY_TARGET)

run_benchmark: $(BENCHMARK_TARGET)
	./$(BENCHMARK_TARGET)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BENCHMARK_TARGET)
	rm -rf data/
	rm -rf charts/

charts: $(BENCHMARK_TARGET)
	@mkdir -p data
	@mkdir -p charts
	./$(BENCHMARK_TARGET)

clean_charts:
	rm -f $(BENCHMARK_TARGET)
	rm -rf data/
	rm -rf charts/