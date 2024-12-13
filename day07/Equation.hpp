#ifndef __EQUATION_H__
#define __EQUATION_H__

#include <ostream>
#include <vector>
#include <unordered_set>

using namespace std;

class Equation
{
public:
    Equation(
        unsigned long int result,
        const vector<unsigned long int>& operands,
        unordered_set<char>& operation_types
    );
    bool is_possible();
    void print(ostream& os);
    long unsigned int get_result();
    void add_operation_type(char operation_type);
    void set_operations_types(unordered_set<char>& new_operation_types);

private:
    long unsigned int result;
    vector<unsigned long int> operands;
    unordered_set<char> operation_types;
};

#endif