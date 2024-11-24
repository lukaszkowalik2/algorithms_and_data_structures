CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

BUILD_DIR = build

TARGET = $(BUILD_DIR)/dictionary

SRC = singly_linked_list.cpp doubly_linked_list.cpp cursor_singly_linked_list_vector.cpp cursor_doubly_linked_list_vector.cpp main.cpp cursor_singly_linked_list.cpp cursor_doubly_linked_list.cpp

OBJ = $(SRC:%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

zip:
	zip -r dictionary.zip * -x "build/*"
