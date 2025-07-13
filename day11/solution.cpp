#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

#define P1_BLINK_LIMIT 25
#define P2_BLINK_LIMIT 75

typedef unordered_map<unsigned long long, unsigned long long> Stone2CountMap;

unsigned long long get_stone_count(const Stone2CountMap& input, const size_t BLINK_LIMIT);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cout << "Usage: ./solution input.txt\n";
        return 0;
    }

    string infile = argv[1];
    ifstream fin;

    fin.open(infile, fstream::in);

    if (!fin.is_open()) {
        cout << "Failed to open " << infile << "\n";
        return 0;
    }

    string linebuf;

    Stone2CountMap stone2count;

    while (getline(fin, linebuf)) {
        stringstream ss;
        ss.str(linebuf);
        unsigned long long stone_number;
        while (ss >> stone_number) {
            auto mit = stone2count.find(stone_number);
            if (mit == stone2count.end()) {
                stone2count.insert(make_pair(stone_number, 1));
            } else {
                mit->second++;
            }
        }
    }

    unsigned long long p1_answer = get_stone_count(stone2count, P1_BLINK_LIMIT);
    unsigned long long p2_answer = get_stone_count(stone2count, P2_BLINK_LIMIT);

    cout << "Part 1 Answer: " << p1_answer << "\n";
    cout << "Part 2 Answer: " << p2_answer << "\n";

    return 0;
}

unsigned long long get_stone_count(const Stone2CountMap& input, const size_t BLINK_LIMIT)
{
    Stone2CountMap stone2count = input;

    for (size_t i = 0; i < BLINK_LIMIT; i++) {
        Stone2CountMap new_stone2count;
        for (auto mit = stone2count.begin(); mit != stone2count.end(); mit++) {
            if (mit->first == 0) {
                auto mit2 = new_stone2count.find(1);
                if (mit2 == new_stone2count.end()) {
                    new_stone2count.insert(make_pair(1, mit->second));
                } else {
                    mit2->second += mit->second;
                }
            } else {
                stringstream ss;
                ss.clear();
                ss << mit->first;
                if (ss.str().size() % 2 == 0) {
                    int first_half = stoull(ss.str().substr(0, ss.str().size() / 2));
                    int second_half = stoull(ss.str().substr(ss.str().size()/2, ss.str().size()/2));
                    auto mit2 = new_stone2count.find(first_half);
                    if (mit2 == new_stone2count.end()) {
                        new_stone2count.insert(make_pair(first_half, mit->second));
                    } else {
                        mit2->second += mit->second;
                    }
                    mit2 = new_stone2count.find(second_half);
                    if (mit2 == new_stone2count.end()) {
                        new_stone2count.insert(make_pair(second_half, mit->second));
                    } else {
                        mit2->second += mit->second;
                    }
                } else {
                    auto mit2 = new_stone2count.find(mit->first * 2024);
                    if (mit2 == new_stone2count.end()) {
                        new_stone2count.insert(make_pair(mit->first * 2024, mit->second));
                    } else {
                        mit2->second += mit->second;
                    }
                }
            } 
        }
        stone2count = new_stone2count;
    }

    unsigned long long sum = 0;

    for (auto mit = stone2count.begin(); mit != stone2count.end(); mit++) {
        sum += mit->second;
    }

    return sum;
}