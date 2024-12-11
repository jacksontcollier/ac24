#include "GuardMap.hpp"
#include "GuardMapTypes.hpp"

#include <set>
#include <utility>
#include <vector>

using namespace std;

void GuardMap::append_map_row(const string& row)
{
    guard_map.push_back(row);
}

GuardPos GuardMap::find_guard_pos()
{
    GuardPos guard_pos;

    for (size_t i = 0; i < guard_map.size(), i++) {
        for (size_t j = 0; j < guard_map[0].size(); j++) {
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
    return (guard_pos.pos.first >= 0)  && (guard_pos.pos.first < guard_map.size()) &&
           (guard_pos.pos.second >= 0) && (guard_pos.pos.second < guard_map[0].size());
}

bool GuardMap::is_pos_obstructed(const GuardPos& guard_pos)
{
    return guard_map[guard_pos.pos.first][guard_pos.pos.second] == '#';
}

GuardOrientation adjust_orientation(const GuardOrientation& guard_orientation)
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
    long unsigned int num_distinct_pos = 0;
    bool is_guard_pos_in_bounds = true;
    set<GuardPos> visited_locations;
    GuardPos guard_pos = starting_guard_pos;

    while (is_guard_pos_in_bounds && !loop_detected) {
        visited_locations.insert(guard_pos);
        GuardPos next_guard_pos = get_next_pos(guard_pos);

        if (!guard_pos_in_bounds(next_guard_pos)) {
            is_guard_pos_in_bounds = false;
            continue;
        }

        if (is_pos_obstructed(next_guard_pos)) {
            next_guard_pos.orientation = adjust_orientation(next_guard_pos.orientation);
        } else {
            if (visited_locations.find(next_guard_pos) != visited_locations.end()) {
                loop_detected = true;
                continue;
            }
        }
    }

    return make_pair(visited_locations.size(), loop_detected);
}
