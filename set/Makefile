CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

BUILD_DIR = build
SIMPLE_TARGET = $(BUILD_DIR)/set_simple
LINKED_TARGET = $(BUILD_DIR)/set_linked
DICTIONARY_TARGET = $(BUILD_DIR)/dictionary_simple

SIMPLE_SRC = set-simple.cpp
LINKED_SRC = set-linked.cpp
DICTIONARY_SRC = dictionary-simple.cpp

all: $(BUILD_DIR) $(SIMPLE_TARGET) $(LINKED_TARGET) $(DICTIONARY_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SIMPLE_TARGET): $(SIMPLE_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SIMPLE_SRC) -o $(SIMPLE_TARGET)

$(LINKED_TARGET): $(LINKED_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LINKED_SRC) -o $(LINKED_TARGET)

$(DICTIONARY_TARGET): $(DICTIONARY_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DICTIONARY_SRC) -o $(DICTIONARY_TARGET)

.PHONY: clean run_simple run_linked run_dictionary all

run_simple: $(SIMPLE_TARGET)
	$(SIMPLE_TARGET)

run_linked: $(LINKED_TARGET)
	$(LINKED_TARGET)

run_dictionary: $(DICTIONARY_TARGET)
	$(DICTIONARY_TARGET)	

clean:
	rm -rf $(BUILD_DIR)