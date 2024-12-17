#include "AntennaMap.hpp"

#include <map>
#include <set>
#include <vector>
#include <utility>

using namespace std;

AntennaMap::AntennaMap(vector<string>& antenna_map)
{
    this->antenna_map = antenna_map;
}

void AntennaMap::find_antenna_locations()
{
    for (size_t i = 0; i < antenna_map.size(); i++) {
        for (size_t j = 0; j < antenna_map[i].size(); j++) {
            char map_character = antenna_map[i][j];

            if ((map_character >= '0' && map_character <= '9')   ||
                (map_character >= 'a' && map_character <= 'z')  ||
                (map_character >= 'A' && map_character <= 'Z')) {
                // Found new antenna
                auto mit = antenna_frequencies.find(map_character);

                if (mit == antenna_frequencies.end()) {
                    // First occurence of this antenna frequency discovered
                    vector<pair<int, int>> locations;
                    locations.push_back(make_pair((int) i, (int) j));
                    antenna_frequencies.insert(make_pair(map_character, locations));
                } else {
                    mit->second.push_back(make_pair((int) i, (int) j));
                }
            }
        }
    }
}

void AntennaMap::find_antinode_locations()
{
    antinode_locations.clear();

    for (auto& freq : antenna_frequencies) {
        auto& ant_locs = freq.second;
        for (size_t i = 0; i < ant_locs.size(); i++) {
            auto& first_loc = ant_locs[i];
            for (size_t j = i + 1; j < ant_locs.size(); j++) {
                auto& sec_loc = ant_locs[j];
                pair<int, int> first_loc_antinode_offset = {
                    first_loc.first - sec_loc.first, first_loc.second - sec_loc.second
                };
                pair<int, int> sec_loc_antinode_offset = {
                    sec_loc.first - first_loc.first, sec_loc.second - first_loc.second
                };
                pair<int, int> first_antinode_loc = {
                    first_loc.first + first_loc_antinode_offset.first,
                    first_loc.second + first_loc_antinode_offset.second
                };
                pair<int, int> sec_antinode_loc = {
                    sec_loc.first + sec_loc_antinode_offset.first,
                    sec_loc.second + sec_loc_antinode_offset.second
                };
                vector<pair<int, int>> antinode_locs = {
                    first_antinode_loc, sec_antinode_loc
                };
                for (auto& antinode_loc : antinode_locs) {
                    if ((antinode_loc.first >= 0)  && (antinode_loc.first < (int) antenna_map.size()) &&
                        (antinode_loc.second >= 0) && (antinode_loc.second < (int) antenna_map[0].size()))
                    {
                        antinode_locations.insert(antinode_loc);
                    }
                }
            }
        }
    }
}

void AntennaMap::p2_find_antinode_locations()
{
    antinode_locations.clear();

    for (auto& freq : antenna_frequencies) {
        auto& ant_locs = freq.second;
        for (size_t i = 0; i < ant_locs.size(); i++) {
            auto& first_loc = ant_locs[i];
            for (size_t j = i+1; j < ant_locs.size(); j++) {
                auto& sec_loc = ant_locs[j];
                pair<int, int> first_loc_antinode_offset = {
                    first_loc.first - sec_loc.first, first_loc.second - sec_loc.second
                };
                pair<int, int> sec_loc_antinode_offset = {
                    sec_loc.first - first_loc.first, sec_loc.second - first_loc.second
                };
                pair<int, int> first_antinode_loc = first_loc;
                while (pos_in_bounds(first_antinode_loc)) {
                    antinode_locations.insert(first_antinode_loc);
                    first_antinode_loc.first += first_loc_antinode_offset.first;
                    first_antinode_loc.second += first_loc_antinode_offset.second;
                }
                pair<int, int> sec_antinode_loc = sec_loc;
                while (pos_in_bounds(sec_antinode_loc)) {
                    antinode_locations.insert(sec_antinode_loc);
                    sec_antinode_loc.first += sec_loc_antinode_offset.first;
                    sec_antinode_loc.second += sec_loc_antinode_offset.second;
                }
            }
        }
    }
}

size_t AntennaMap::get_num_unique_antinode_locations()
{
    return antinode_locations.size();
}

bool AntennaMap::pos_in_bounds(pair<int, int>& pos)
{
    return ((pos.first >= 0)  && (pos.first < (int) antenna_map.size()) &&
            (pos.second >= 0) && (pos.second < (int) antenna_map[0].size()));
}
