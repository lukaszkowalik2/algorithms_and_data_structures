CXX = g++
CXXFLAGS = -Wall -std=c++11

BUILD_DIR = build
TARGET = $(BUILD_DIR)/dijkstra-algorithm

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): dijkstra-algorithm.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean 