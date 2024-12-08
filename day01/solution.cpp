#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "Usage: ./solution input.txt";
        return 0;
    }

    ifstream fin;
    string infile = argv[1];
    fin.open(infile, fstream::in);

    if (!fin.is_open()) {
        cout << "Failed to open " << infile << " for reading\n";
        return 0;
    }

    vector<int> first_list;
    vector<int> second_list;

    string line;
    stringstream token_parser;
    int n;

    while (getline(fin, line)) {
        token_parser.clear();
        token_parser.str(line);
        token_parser >> n;
        first_list.push_back(n);
        token_parser >> n;
        second_list.push_back(n);
    }

    sort(first_list.begin(), first_list.end());
    sort(second_list.begin(), second_list.end());
    int sum_distance = 0;

    for (size_t i = 0; (i < first_list.size()) && (i < second_list.size()); i++) {
        sum_distance += abs(first_list[i] - second_list[i]);
    }

    cout << "Part 1: Distance is " << sum_distance << "\n";

    unordered_map<int, int> num2count;

    for (auto& num: second_list) {
        auto mit = num2count.find(num);
        if (mit != num2count.end()) {
            mit->second++;
        } else {
            num2count.insert(make_pair(num, 1));
        }
    }

    int similarity_score = 0;
    for (auto& num: first_list) {
        auto mit = num2count.find(num);
        if (mit != num2count.end()) {
            similarity_score += (num * mit->second);
        }
    }

    cout << "Part 2: Similarity score is " << similarity_score << "\n";

    return 0;
}
