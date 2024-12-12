#include "GuardMap.hpp"
#include "GuardMapTypes.hpp"

#include <iostream>
#include <functional>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

bool operator==(const GuardPos& p1, const GuardPos& p2)
{
    return (p1.pos.first == p2.pos.first) && (p1.pos.second == p2.pos.second) &&
           (p1.orientation == p2.orientation);
}

struct GuardPosComparator{
    bool operator() (const GuardPos& p1, const GuardPos& p2) const
    {
        if (p1.pos.first < p2.pos.first) {
            return true;
        } else if (p2.pos.first < p1.pos.first) {
            return false;
        }

        if (p1.pos.second < p2.pos.second) {
            return true;
        } else if (p2.pos.second < p1.pos.second) {
            return false;
        }

        if (p1.orientation < p2.orientation) {
            return true;
        } else if (p2.orientation < p1.orientation) {
            return false;
        }

        return false;
    }
};

struct GuardPosHashFunction
{
    size_t operator()(const GuardPos& guard_pos) const
    {
        string s = "(" + to_string(guard_pos.pos.first) + ", " + to_string(guard_pos.pos.second) + ") ";
        if (guard_pos.orientation == UP_GUARD_FACE) {
            s += "UP";
        } else if (guard_pos.orientation == RIGHT_GUARD_FACE) {
            s += "RIGHT";
        } else if (guard_pos.orientation == DOWN_GUARD_FACE) {
            s += "DOWN";
        } else {
            s += "LEFT";
        }

        return hash<string>()(s);
    }
};

void GuardMap::append_map_row(const string& row)
{
    guard_map.push_back(row);
}

GuardPos GuardMap::find_guard_pos()
{
    GuardPos guard_pos;

    for (size_t i = 0; i < guard_map.size(); i++) {
        for (size_t j = 0; j < guard_map[i].size(); j++) {
            if (guard_map[i][j] == '^') {
                guard_pos.pos.first = i;
                guard_pos.pos.second = j;
                guard_pos.orientation = UP_GUARD_FACE;
            }
        }
    }

    return guard_pos;
}

GuardPos GuardMap::get_next_pos(const GuardPos& guard_pos)
{
    GuardPos next_guard_pos = guard_pos;

    if (guard_pos.orientation == UP_GUARD_FACE) {
        next_guard_pos.pos.first--;
    } else if (guard_pos.orientation == RIGHT_GUARD_FACE) {
        next_guard_pos.pos.second++;
    } else if (guard_pos.orientation == DOWN_GUARD_FACE) {
        next_guard_pos.pos.first++;
    } else if (guard_pos.orientation == LEFT_GUARD_FACE) {
        next_guard_pos.pos.second--;
    }

    return next_guard_pos;
}

bool GuardMap::guard_pos_in_bounds(const GuardPos& guard_pos)
{
    return (guard_pos.pos.first >= 0)  && (guard_pos.pos.first < (signed long long) guard_map.size()) &&
           (guard_pos.pos.second >= 0) && (guard_pos.pos.second < (signed long long) guard_map[0].size());
}

bool GuardMap::is_pos_obstructed(const GuardPos& guard_pos)
{
    return guard_map[guard_pos.pos.first][guard_pos.pos.second] == '#';
}

GuardOrientation GuardMap::adjust_orientation(const GuardOrientation& guard_orientation)
{
    switch (guard_orientation) {
        case UP_GUARD_FACE:
            return RIGHT_GUARD_FACE;
        case RIGHT_GUARD_FACE:
            return DOWN_GUARD_FACE;
        case DOWN_GUARD_FACE:
            return LEFT_GUARD_FACE;
        case LEFT_GUARD_FACE:
            return UP_GUARD_FACE;
        default:
            return UP_GUARD_FACE;
    }
}

pair<long unsigned int, bool> GuardMap::count_distinct_guard_pos(const GuardPos& starting_guard_pos)
{
    bool loop_detected = false;
    bool is_guard_pos_in_bounds = true;
    set<GuardPos, GuardPosComparator> visited_locations_orientations;
    set<BoardPos> visited_locations;
    GuardPos guard_pos = starting_guard_pos;

    while (is_guard_pos_in_bounds && !loop_detected) {
        visited_locations_orientations.insert(guard_pos);
        visited_locations.insert(guard_pos.pos);
        GuardPos next_guard_pos = get_next_pos(guard_pos);

        if (!guard_pos_in_bounds(next_guard_pos)) {
            is_guard_pos_in_bounds = false;
            continue;
        }

        if (is_pos_obstructed(next_guard_pos)) {
            guard_pos.orientation = adjust_orientation(guard_pos.orientation);
        } else {
            auto sit = visited_locations_orientations.find(next_guard_pos);

            if (sit != visited_locations_orientations.end()) {
                loop_detected = true;
                continue;
            }

            guard_pos = next_guard_pos;
        }
    }

    return make_pair(visited_locations.size(), loop_detected);
}

vector<string>& GuardMap::map()
{
    return guard_map;
}
