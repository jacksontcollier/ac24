#include "GuardMap.hpp"
#include "GuardMapTypes.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "Usage: ./solution input.txt\n";
        return 0;
    }

    string infile = argv[1];
    ifstream fin;

    fin.open(infile, fstream::in);

    if (!fin.is_open()) {
        cout << "Failed to open " << infile << "for reading\n";
        return 0;
    }

    GuardMap guard_map;
    string line;

    // Read input
    while (getline(fin, line)) {
        guard_map.append_map_row(line);
    }

    // Find guard starting position
    GuardPos guard_pos = guard_map.find_guard_pos();

    pair<long unsigned int, bool> answer = guard_map.count_distinct_guard_pos(guard_pos);
    cout << "Part 1: Number of Distinct Locations: " << answer.first << "\n";

    vector<string>& gmap = guard_map.map();
    int num_obstacle_placement = 0;
    for (size_t i = 0; i < gmap.size(); i++) {
        for (size_t j = 0; j < gmap.size(); j++) {
            if (gmap[i][j] == '.' && (((signed long long) i != guard_pos.pos.first) || ((signed long long) j != guard_pos.pos.second))) {
                gmap[i][j] = '#';
                answer = guard_map.count_distinct_guard_pos(guard_pos);
                if (answer.second == true) {
                    num_obstacle_placement++;
                }
                gmap[i][j] = '.';
            }
        }
    }

    cout << "Part 2: Number of Obstacle Placements: " << num_obstacle_placement << "\n";

    return 0;
}

