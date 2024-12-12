.PHONY: all

all: solution

GuardMap.o: GuardMap.cpp GuardMap.hpp GuardMapTypes.hpp
	g++ -Wall -std=c++11 -c GuardMap.cpp -o GuardMap.o

solution.o: solution.cpp GuardMap.hpp GuardMapTypes.hpp
	g++ -Wall -std=c++11 -c solution.cpp -o solution.o

solution: solution.o GuardMap.o
	g++ -o solution solution.o GuardMap.o