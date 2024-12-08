#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
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
        cout << "Failed to open " << infile << " for reading\n";
        return 0;
    }

    string line;
    bool is_reading_rule = true;

    vector<string> rules;
    vector<vector<int>> updates;
    while (getline(fin, line)) {
        if (is_reading_rule && (line.size() == 0)) {
            is_reading_rule = false;
            continue;
        }

        if (is_reading_rule) {
            rules.push_back(line);
        } else {
            vector<int> update;
            size_t start_pos = 0;
            size_t substr_len = 0;
            line.push_back(',');
            for (size_t i = 0; i < line.size(); i++) {
                if (line[i] == ',') {
                    substr_len = i - start_pos;
                    string token = line.substr(start_pos, substr_len);
                    start_pos = i + 1;
                    update.push_back(atoi(token.c_str()));
                }
            }
            updates.push_back(update);
        }
    }

    unordered_map<int, unordered_set<int>> rule_map;

    for (auto& rule : rules) {
        size_t pipe_char_pos = rule.find('|');
        string first_num = rule.substr(0, pipe_char_pos);
        string second_num = rule.substr(pipe_char_pos + 1);
        int first, second;
        stringstream ss;
        ss.clear();
        ss.str(first_num);
        ss >> first;
        ss.clear();
        ss.str(second_num);
        ss >> second;

        auto mit = rule_map.find(first);
        if (mit == rule_map.end()) {
            unordered_set<int> before_list = { second };
            rule_map.insert(make_pair(first, before_list));
        } else {
            mit->second.insert(second);
        }
    }

    size_t valid_update_sum = 0;
    vector<vector<int>> invalid_updates;
    for (auto& update : updates) {
        bool is_update_valid = true;
        for (size_t i = 1; i < update.size(); i++) {
            if (!is_update_valid) {
                break;
            }
            for (size_t j = 0; j < i; j++) {
                int update_num_after = update[i];
                int update_num_before = update[j];
                auto mit = rule_map.find(update_num_after);
                if (mit != rule_map.end()) {
                    if (mit->second.find(update_num_before) != mit->second.end()) {
                        is_update_valid = false;
                        break;
                    }
                }
            }
        }

        if (is_update_valid == true) {
            valid_update_sum += update[update.size() / 2];
        } else {
            invalid_updates.push_back(update);
        }
    }

    size_t fixed_invalid_update_sum = 0;
    for (auto& update : invalid_updates) {
        for (size_t i = 1; i < update.size(); i++) {
            for (size_t j = 0; j < i; j++) {
                int update_num_after = update[i];
                int update_num_before = update[j];
                auto mit = rule_map.find(update_num_after);
                if (mit != rule_map.end()) {
                    if (mit->second.find(update_num_before) != mit->second.end()) {
                        // swap update_num_after and update_num_before
                        update[i] = update_num_before;
                        update[j] = update_num_after;
                    }
                }
            }
        }
        fixed_invalid_update_sum += update[update.size() / 2];
    }

    cout << "Part 1: Valid update sum is " << valid_update_sum << "\n";
    cout << "Part 2: Invalid update sum is " << fixed_invalid_update_sum << "\n";

    return 0;
}