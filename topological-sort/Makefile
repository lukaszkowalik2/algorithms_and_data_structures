CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lgvc -lcgraph -lcdt

BUILD_DIR = build
SRC_DIR = .

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = $(BUILD_DIR)/topological-sort

$(shell mkdir -p $(BUILD_DIR))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

visualize: $(TARGET)
	./$(TARGET)
	dot -Tpng $(BUILD_DIR)/pancake_tasks.dot -o $(BUILD_DIR)/pancake_tasks.png
	dot -Tpng $(BUILD_DIR)/other_tasks.dot -o $(BUILD_DIR)/other_tasks.png

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean run visualize 