CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

all: cities

cities: floyd-warshall-cities.cpp
	$(CXX) $(CXXFLAGS) -o cities floyd-warshall-cities.cpp

run_cities: cities czasy.txt
	./cities

clean:
	rm -f cities

.PHONY: all run_cities clean 