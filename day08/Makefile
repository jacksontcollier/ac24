.PHONY: all

all: solution

solution: solution.o AntennaMap.o
	g++ -o solution solution.o AntennaMap.o

solution.o: solution.cpp AntennaMap.hpp
	g++ -Wall --std=c++11 -c solution.cpp -o solution.o

AntennaMap.o: AntennaMap.cpp AntennaMap.hpp
	g++ -Wall --std=c++11 -c AntennaMap.cpp -o AntennaMap.o