#include "Equation.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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
    vector<Equation> equations;
    unordered_set<char> operation_types = { '+', '*' };

    while (getline(fin, line)) {
        size_t col_pos = line.find(':');
        stringstream ss;
        ss.clear();
        ss.str(line.substr(0, col_pos));
        unsigned long int result;
        ss >> result;
        vector<unsigned long int> operands;
        ss.clear();
        ss.str(line.substr(col_pos + 1));
        unsigned long int operand;
        while (ss >> operand) {
            operands.push_back(operand);
        }
        equations.push_back(Equation(result, operands, operation_types));
    }


    long long unsigned int possible_equation_sum = 0;
    for (auto& equation : equations) {
        if (equation.is_possible()) {
            possible_equation_sum += equation.get_result();
        }
    }

    cout << "Part 1: Good Equations Sum: " << possible_equation_sum << "\n";

    possible_equation_sum = 0;
    for (auto& equation : equations) {
        equation.add_operation_type('|');
        if (equation.is_possible()) {
            possible_equation_sum += equation.get_result();
        }
    }

    cout << "Part 2: Good Equations Sum: " << possible_equation_sum << "\n";

    return 0;
}