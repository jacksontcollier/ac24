all: solution

.PHONY: clean

clean:
	rm -f *.o solution 

Stone.o: Stone.cpp Stone.hpp
	g++ -Wall -std=c++11 -c Stone.cpp -o Stone.o

solution.o: solution.cpp Stone.hpp
	g++ -Wall -std=c++11 -c solution.cpp -o solution.o

solution: solution.o Stone.o
	g++ -Wall -std=c++11 -o solution solution.o Stone.o