#ifndef __GUARD_MAP_H__
#define __GUARD_MAP_H__

#include "GuardMapTypes.hpp"

#include <string>
#include <vector>

using namespace std;

bool compareGuardPos(const GuardPos& p1, const GuardPos& p2);

class GuardMap
{
public:
    void append_map_row(const string& row);
    GuardPos find_guard_pos();
    GuardPos get_next_pos(const GuardPos& guard_pos);
    bool guard_pos_in_bounds(const GuardPos& guard_pos);
    bool is_pos_obstructed(const GuardPos& guard_pos);
    GuardOrientation adjust_orientation(const GuardOrientation& guard_orientation);
    pair<long unsigned int, bool> count_distinct_guard_pos(const GuardPos& starting_guard_pos);
    vector<string>& map();

private:
    vector<string> guard_map;
};

#endif