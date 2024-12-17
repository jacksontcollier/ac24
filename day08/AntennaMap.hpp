#ifndef __ANTENNA_MAP_H__
#define __ANTENNA_MAP_H__

#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class AntennaMap
{
public:
    AntennaMap(vector<string>& antenna_map);
    void find_antenna_locations();
    void find_antinode_locations();
    void p2_find_antinode_locations();
    size_t get_num_unique_antinode_locations();
    bool pos_in_bounds(pair<int, int>& pos);


private:
    vector<string> antenna_map;
    map<char, vector<pair<int, int>>> antenna_frequencies;
    set<pair<int, int>> antinode_locations;
};

#endif