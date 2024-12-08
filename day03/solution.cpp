#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int execute_instruction(const string instruction);

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "Usage: ./solution input.txt\n";
        return 0;
    }

    ifstream fin;

    fin.open(argv[1], ifstream::in);

    if (!fin.is_open()) {
        cout << "Failed to open " << argv[1] << " for reading\n";
        return 0;
    }

    string line;
    string input;

    while (getline(fin, line)) {
        input += line;
    }

    smatch instruction_match;
    regex multiply_instruction_regex("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
    vector<string> instructions;
    string search_substr = input;

    while (regex_search(search_substr, instruction_match, multiply_instruction_regex)) {
        instructions.push_back(instruction_match[0]);
        search_substr = search_substr.substr(instruction_match.position() + instruction_match[0].length());
    }

    int sum = 0;

    for (auto& instruction : instructions) {
        sum += execute_instruction(instruction);
    }

    cout << "Part 1: Answer is " << sum << "\n";

    search_substr = input;
    regex do_instruction_regex("do\\(\\)");
    regex dont_instruction_regex("don't\\(\\)");
    enum InstructionType {
        MUL,
        DO,
        DONT
    };

    vector<pair<InstructionType, string>> types_and_instructions;

    while (true) {
        vector<pair<InstructionType, smatch>> instruction_matches;
        if (regex_search(search_substr, instruction_match, multiply_instruction_regex)) {
            instruction_matches.push_back(make_pair(MUL, instruction_match));
        }

        if (regex_search(search_substr, instruction_match, do_instruction_regex)) {
            instruction_matches.push_back(make_pair(DO, instruction_match));
        }

        if (regex_search(search_substr, instruction_match, dont_instruction_regex)) {
            instruction_matches.push_back(make_pair(DONT, instruction_match));
        }

        if (!instruction_matches.size()) {
            break;
        }

        size_t next_instruction_i = 0;
        long int instruction_substr_start_pos = instruction_matches[0].second.position();
        for (size_t i = 1; i < instruction_matches.size(); i++) {
            if (instruction_matches[i].second.position() < instruction_substr_start_pos) {
                instruction_substr_start_pos = instruction_matches[i].second.position();
                next_instruction_i = i;
            }
        }

        types_and_instructions.push_back(make_pair(instruction_matches[next_instruction_i].first,
                instruction_matches[next_instruction_i].second[0]));
        search_substr = search_substr.substr(instruction_matches[next_instruction_i].second.position() +
                instruction_matches[next_instruction_i].second[0].length());

    }

    sum = 0;
    bool multiply_enabled = true;
    for (auto& type_and_instruction : types_and_instructions) {
        if (type_and_instruction.first == DO) {
            multiply_enabled = true;
        } else if (type_and_instruction.first == DONT) {
            multiply_enabled = false;
        } else if (type_and_instruction.first == MUL && multiply_enabled) {
            sum += execute_instruction(type_and_instruction.second);
        }
    }

    cout << "Part 2: Answer is " << sum << "\n";

    return 0;
}

int execute_instruction(const string instruction)
{
    regex operand_regex("([0-9]{1,3})");
    smatch operand_smatch;
    int result = 0;
    stringstream ss;
    int a = 0;
    int b = 0;

    string search_str;
    search_str = instruction;
    if (regex_search(search_str, operand_smatch, operand_regex)) {
        ss.str(operand_smatch[0]);
        ss >> a;
        search_str = search_str.substr(operand_smatch.position() + operand_smatch[0].length());
        if (regex_search(search_str, operand_smatch, operand_regex)) {
            ss.str("");
            ss.clear();
            ss.str(operand_smatch[0]);
            ss >> b;
            result = a * b;
        }
    }

    return result;
}
