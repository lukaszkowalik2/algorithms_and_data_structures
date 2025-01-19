CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = hash-table
GENERATOR = generate-numbers
BUILD_DIR = build
SRC = hash-table.cpp
GEN_SRC = generate-numbers.cpp

OBJ = $(BUILD_DIR)/$(SRC:.cpp=.o)
GEN_OBJ = $(BUILD_DIR)/$(GEN_SRC:.cpp=.o)

all: $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/$(GENERATOR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@

$(BUILD_DIR)/$(GENERATOR): $(GEN_OBJ)
	$(CXX) $(GEN_OBJ) -o $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET) numbers.txt

generate: $(BUILD_DIR)/$(GENERATOR)
	./$(BUILD_DIR)/$(GENERATOR) 100

zip:
	zip -r hash-table.zip $(SRC) $(GEN_SRC) Makefile

clean:
	rm -rf $(BUILD_DIR)
	rm -f hash-table.zip
	rm -f numbers.txt

.PHONY: all clean run generate zip 