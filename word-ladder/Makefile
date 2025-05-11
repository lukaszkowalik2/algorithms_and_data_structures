CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = word-ladder
SRC = word-ladder.cpp
BUILD_DIR = build

$(BUILD_DIR)/$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: all clean run
all: $(BUILD_DIR)/$(TARGET)

run: all
	./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
