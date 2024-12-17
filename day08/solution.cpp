#include "AntennaMap.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char**argv)
{
    if (argc != 2) {
        cout << "Usage: ./solution input.txt\n";
        return 0;
    }

    string infile = argv[1];
    ifstream fin;

    fin.open(infile, fstream::in);

    if (!fin.is_open()) {
        cout << "Failed to open " << infile << " for reading\n";
        return 0;
    }

    string line;
    vector<string> antenna_map_input;

    while (getline(fin, line)) {
        antenna_map_input.push_back(line);
    }

    AntennaMap antenna_map(antenna_map_input);
    antenna_map.find_antenna_locations();
    antenna_map.find_antinode_locations();

    cout << "Part 1 Answer: " << antenna_map.get_num_unique_antinode_locations() << "\n";
    antenna_map.p2_find_antinode_locations();
    cout << "Part 2 Answer: " << antenna_map.get_num_unique_antinode_locations() << "\n";
}