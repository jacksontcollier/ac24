#include "Equation.hpp"

#include <cmath>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

Equation::Equation(
    unsigned long int result,
    const vector<unsigned long int>& operands,
    unordered_set<char>& operation_types)
{
    this->result = result;
    this->operands = operands;
    this->operation_types = operation_types;
}

bool Equation::is_possible()
{
    if (operands.size() == 0) {
        return false;
    }

    if (operands.size() == 1) {
        return operands[0] == result;
    }

    vector<unsigned long int> previous_operation_results;
    vector<unsigned long int> current_operation_results;

    current_operation_results.push_back(operands[0]);

    for (size_t i = 1; i < operands.size(); i++) {
        previous_operation_results = current_operation_results;
        current_operation_results.clear();
        for (size_t j = 0; j < previous_operation_results.size(); j++) {
            unsigned long int previous_result = previous_operation_results[j];
            unsigned long int new_operand = operands[i];
            for (const auto& op: operation_types) {
                if (op == '+') {
                    current_operation_results.push_back(previous_result + new_operand);
                } else if (op == '*') {
                    current_operation_results.push_back(previous_result * new_operand);
                } else if (op == '|') {
                    current_operation_results.push_back(
                        stoul(to_string(previous_result) + to_string(new_operand))
                    );
                }
            }
        }
    }

    for (auto& possible_result : current_operation_results) {
        if (possible_result == result) {
            return true;
        }
    }

    return false;
}

void Equation::print(ostream& os)
{
    os << result << ":";
    for (auto& operand : operands) {
        os << " " << operand;
    }
}

long unsigned int Equation::get_result()
{
    return result;
}

void Equation::add_operation_type(char operation_type)
{
    operation_types.insert(operation_type);
}

void Equation::set_operations_types(unordered_set<char>& new_operation_types)
{
    operation_types = new_operation_types;
}
