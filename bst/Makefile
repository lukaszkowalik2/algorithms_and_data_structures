CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = bst
BUILD_DIR = build
SRC = BSTTree.cpp

OBJ = $(BUILD_DIR)/$(SRC:.cpp=.o)

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

zip:
	zip -r bst.zip $(SRC) Makefile

clean:
	rm -rf $(BUILD_DIR)
	rm -f bst.zip

.PHONY: all clean run zip