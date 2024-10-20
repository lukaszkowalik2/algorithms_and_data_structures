CXX = g++

CXXFLAGS = -std=c++11 -O2

EXEC = sorting_algorithms

SRC = sorting_algorithms.cpp

all: $(EXEC)

$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRC)

clean:
	rm -f $(EXEC)

run: all
	./$(EXEC)
