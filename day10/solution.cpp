#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

void recursive_path_find(
    vector<vector<int>>& topographic_map,
    pair<size_t, size_t> pos,
    set<pair<size_t, size_t>>& visited_pos,
    int& nine_count
);

void compute_trailhead_rating(
    vector<vector<int>>& topographic_map,
    pair<size_t, size_t> pos,
    vector<pair<size_t, size_t>>& path,
    unordered_set<string>& distinct_paths,
    int& trailhead_rating
);

string path2str(vector<pair<size_t, size_t>>& path);

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "Usage: ./solution input.txt";
        return 0;
    }

    string infile = argv[1];
    ifstream fin;

    fin.open(infile, fstream::in);

    if (!fin.is_open()) {
        cout << "Failed to open " << infile << " for reading\n";
        return 0;
    }

    string linebuf;
    vector<vector<int>> topographic_map;
    vector<pair<size_t, size_t>> trailhead_pos;
    size_t row_pos = 0;

    while (getline(fin, linebuf)) {
        vector<int> map_row;
        for (size_t column_pos = 0; column_pos < linebuf.size(); column_pos++) {
            int height = linebuf[column_pos] - '0';
            map_row.push_back(height);
            if (height == 0) {
                trailhead_pos.push_back(make_pair(row_pos, column_pos));
            }
        }
        topographic_map.push_back(map_row);
        row_pos++;
    }

    int nine_sum = 0;

    for (auto& tpos : trailhead_pos) {
        int nine_count = 0;
        set<pair<size_t, size_t>> visited_pos;

        visited_pos.clear();

        recursive_path_find(
            topographic_map,
            tpos,
            visited_pos,
            nine_count
        );

        nine_sum += nine_count;
    }
    
    cout << "Part 1 Answer: " << nine_sum << "\n";
   
    unsigned long long trailhead_rating_sum = 0;
    for (auto& tpos : trailhead_pos) {
        int trailhead_rating = 0;
        set<pair<size_t, size_t>> visited_pos;
        vector<pair<size_t, size_t>> path;
        unordered_set<string> distinct_paths;

        visited_pos.clear();

        compute_trailhead_rating(
            topographic_map,
            tpos,
            path,
            distinct_paths,
            trailhead_rating
        );

        trailhead_rating_sum += trailhead_rating;
    }

    cout << "Part 2 Answer: " << trailhead_rating_sum << "\n";

    return 0;
}

void recursive_path_find(
    vector<vector<int>>& topographic_map,
    pair<size_t, size_t> pos,
    set<pair<size_t, size_t>>& visited_pos,
    int& nine_count
)
{
    // We have now visited this location
    visited_pos.insert(pos);

    // Grab position height
    int pos_height = topographic_map[pos.first][pos.second];

    // If we are on a 9, add it to count
    if (pos_height == 9) {
        nine_count++;
        return;
    }

    // Determine viable adjacent positions
    vector<pair<size_t, size_t>> viable_adjacent_positions;

    // Up
    if (pos.first > 0) {
        pair<size_t, size_t> up_pos = { pos.first - 1, pos.second };
        int up_pos_height = topographic_map[up_pos.first][up_pos.second];
        if ((up_pos_height == pos_height + 1) &&
            (visited_pos.find(up_pos) == visited_pos.end())) {
            viable_adjacent_positions.push_back(up_pos);
        }
    }

    // Right
    if (pos.second + 1 < topographic_map[0].size()) {
        pair<size_t, size_t> right_pos = { pos.first, pos.second + 1 };
        int right_pos_height = topographic_map[right_pos.first][right_pos.second];
        if ((right_pos_height == pos_height + 1) &&
            (visited_pos.find(right_pos) == visited_pos.end())) {
            viable_adjacent_positions.push_back(right_pos);
        }
    }

    // Down
    if (pos.first + 1 < topographic_map.size()) {
        pair<size_t, size_t> down_pos = { pos.first + 1, pos.second };
        int down_pos_height = topographic_map[down_pos.first][down_pos.second];
        if ((down_pos_height == pos_height + 1) &&
            (visited_pos.find(down_pos) == visited_pos.end())) {
            viable_adjacent_positions.push_back(down_pos);
        }

    }

    // Left
    if (pos.second > 0) {
        pair<size_t, size_t> left_pos = { pos.first, pos.second - 1 };
        int left_pos_height = topographic_map[left_pos.first][left_pos.second];
        if ((left_pos_height == pos_height + 1) &&
            (visited_pos.find(left_pos) == visited_pos.end())) {
            viable_adjacent_positions.push_back(left_pos);
        }
    }

    for (auto& pos : viable_adjacent_positions) {
        recursive_path_find(
            topographic_map,
            pos,
            visited_pos,
            nine_count
        );
    }

    return;
}

void compute_trailhead_rating(
    vector<vector<int>>& topographic_map,
    pair<size_t, size_t> pos,
    vector<pair<size_t, size_t>>& path,
    unordered_set<string>& distinct_paths,
    int& trailhead_rating
)
{
    // We have now visited this location
    path.push_back(pos);

    // Grab position height
    int pos_height = topographic_map[pos.first][pos.second];

    // If we are on a 9, add it to count
    if (pos_height == 9) {
        string path_str = path2str(path);

        if (distinct_paths.find(path_str) == distinct_paths.end()) {
            distinct_paths.insert(path_str);
            trailhead_rating++;
        }

        path.pop_back();

        return;
    }

    // Determine viable adjacent positions
    vector<pair<size_t, size_t>> viable_adjacent_positions;

    // Up
    if (pos.first > 0) {
        pair<size_t, size_t> up_pos = { pos.first - 1, pos.second };
        int up_pos_height = topographic_map[up_pos.first][up_pos.second];
        if (up_pos_height == pos_height + 1) {
            viable_adjacent_positions.push_back(up_pos);
        }
    }

    // Right
    if (pos.second + 1 < topographic_map[0].size()) {
        pair<size_t, size_t> right_pos = { pos.first, pos.second + 1 };
        int right_pos_height = topographic_map[right_pos.first][right_pos.second];
        if (right_pos_height == pos_height + 1) {
            viable_adjacent_positions.push_back(right_pos);
        }
    }

    // Down
    if (pos.first + 1 < topographic_map.size()) {
        pair<size_t, size_t> down_pos = { pos.first + 1, pos.second };
        int down_pos_height = topographic_map[down_pos.first][down_pos.second];
        if (down_pos_height == pos_height + 1) {
            viable_adjacent_positions.push_back(down_pos);
        }

    }

    // Left
    if (pos.second > 0) {
        pair<size_t, size_t> left_pos = { pos.first, pos.second - 1 };
        int left_pos_height = topographic_map[left_pos.first][left_pos.second];
        if (left_pos_height == pos_height + 1) {
            viable_adjacent_positions.push_back(left_pos);
        }
    }

    for (auto& pos : viable_adjacent_positions) {
        compute_trailhead_rating(
            topographic_map,
            pos,
            path,
            distinct_paths,
            trailhead_rating
        );
    }

    path.pop_back();

    return;

}

string path2str(vector<pair<size_t, size_t>>& path)
{
    stringstream ss;

    ss << "[";

    for (size_t i = 0; i < path.size(); i++) {
        pair<size_t, size_t>& pos = path[i];
        ss << "(" << pos.first << "," << pos.second << ")";
        if (i + 1 < path.size()) {
            ss << ",";
        }
    }

    ss << "]";

    return ss.str();
}
