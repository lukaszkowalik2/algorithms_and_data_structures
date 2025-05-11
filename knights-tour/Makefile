CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

BASE_DIR = .
BUILD_DIR = $(BASE_DIR)/build

SOURCES = kingths-tour.cpp
EXECUTABLE = $(BUILD_DIR)/kingths-tour

all: directories $(EXECUTABLE)

directories:
	@mkdir -p $(BUILD_DIR)

$(EXECUTABLE): $(SOURCES) | directories
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean run directories
