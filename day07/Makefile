.PHONY: all

all: solution

solution: solution.o Equation.o
	g++ -o solution solution.o Equation.o

solution.o: solution.cpp Equation.hpp
	g++ -Wall --std=c++11 -c solution.cpp -o solution.o

Equation.o: Equation.cpp Equation.hpp
	g++ -Wall --std=c++11 -c Equation.cpp -o Equation.o
