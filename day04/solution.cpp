#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int word_search_word_count(const vector<string>& word_search, const string word,
        map<pair<int, int>, int>& mid_char_pos_match_count);

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
    vector<string>  word_search;

    while (getline(fin, line)) {
        word_search.push_back(line);
    }

    map<pair<int, int>, int> mid_char_pos_match_count;
    int word_count = word_search_word_count(word_search, "XMAS", mid_char_pos_match_count);

    cout << "Part 1: Answer is " << word_count << "\n";

    mid_char_pos_match_count.clear();
    word_count = word_search_word_count(word_search, "MAS", mid_char_pos_match_count);

    word_count = 0;
    for (auto mit = mid_char_pos_match_count.begin(); mit != mid_char_pos_match_count.end(); mit++) {
        if (mit->second == 2) {
            word_count++;
        }
    }

    cout << "Part 2: Answer is " << word_count << "\n";

    return 0;
}

int word_search_word_count(const vector<string>& word_search, const string word,
        map<pair<int, int>, int>& mid_char_pos_match_count)
{
    int word_match_count = 0;
    const bool WORD_LEN_ODD = (word.size() % 2) == 1;
    vector<pair<int, int>> position_modifiers = {
        // Left      // Upper Left   // Up       // Upper Right
        { 0, -1 },   { -1, -1 },     { -1, 0 },  { -1, 1 },

        // Right   // Lower right  // Down    // Down left
        { 0, 1 },  { 1, 1 },       { 1, 0 },   { 1, -1 }
    };

    pair<int, int> UPPER_LEFT = position_modifiers[1];
    pair<int, int> UPPER_RIGHT = position_modifiers[3];
    pair<int, int> LOWER_RIGHT = position_modifiers[5];
    pair<int, int> LOWER_LEFT = position_modifiers[7];

    for (size_t i = 0; i < word_search.size();  i++) {
        for (size_t j = 0; j < word_search[i].size(); j++) {
            if (word_search[i][j] == word[0]) {
                for (auto& modifier : position_modifiers) {
                    int matched_chars = 0;
                    int word_i = 0;
                    int xmod = modifier.first;
                    int ymod = modifier.second;
                    int x = i;
                    int y = j;
                    int mid_char_x = 0;
                    int mid_char_y = 0;
                    while ((x >= 0) && (x < (int) word_search.size()) && (y >= 0)
                                    && (y < (int) word_search[i].size()) && (word_i < (int) word.size())) {
                        if (word_search[x][y] == word[word_i]) {
                            if (WORD_LEN_ODD && (word_i == (int) (word.size() / 2))) {
                                mid_char_x = x;
                                mid_char_y = y;
                            }
                            matched_chars++;
                            x += xmod;
                            y += ymod;
                            word_i++;
                        } else {
                            break;
                        }
                    }

                    if (matched_chars == (int) word.size()) {
                        word_match_count++;
                        if ((modifier == UPPER_LEFT) || (modifier == UPPER_RIGHT)
                            || (modifier == LOWER_RIGHT) || (modifier == LOWER_LEFT))
                        {
                            auto mit = mid_char_pos_match_count.find(make_pair(mid_char_x, mid_char_y));
                            if (mit == mid_char_pos_match_count.end()) {
                                mid_char_pos_match_count.insert(make_pair(make_pair(mid_char_x, mid_char_y), 1));
                            } else {
                                mit->second++;
                            }
                        }
                    }
                }
            }
        }
    }

    return word_match_count;
}
