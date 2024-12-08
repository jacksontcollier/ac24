#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Report
{
public:
    Report(const string report_input);
    Report(const vector<int> data);
    bool is_safe(bool is_dampener_used);

private:
    vector<int> data;
};

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "Usage: ./solution input.txt\n";
        return 0;
    }

    ifstream fin;
    fin.open(argv[1]);

    if (!fin.is_open()) {
        cout << "Failed to open " << argv[1] << " for reading\n";
        return 0;
    }

    string line;
    vector<Report> reports;
    while (getline(fin, line)) {
        Report report(line);
        reports.push_back(report);
    }

    int num_safe_reports = 0;
    int num_safe_reports_with_dampener = 0;

    for (auto& report: reports) {
        if (report.is_safe(false)) {
            num_safe_reports++;
        }
        if (report.is_safe(true)) {
            num_safe_reports_with_dampener++;
        }
    }

    cout << "Part 1: Number of safe reports " << num_safe_reports << "\n";
    cout << "Part 2: Number of safe reports with dampener "
        << num_safe_reports_with_dampener << "\n";

    return 0;
}

Report::Report(const string report_input)
{
    stringstream ss;

    ss.str(report_input);

    int num;

    while (ss >> num) {
        data.push_back(num);
    }
}

Report::Report(const vector<int> data)
{
    this->data = data;
}

bool Report::is_safe(const bool is_dampener_used)
{
    bool is_safe = true;
    bool is_increasing = true;

    if (data[0] >= data[1]) {
        is_increasing = false;
    }

    for (size_t i = 0; i + 1 < data.size(); i++) {
        if (is_increasing && (data[i] >= data[i+1])) {
            is_safe = false;
            break;
        } else if (!is_increasing && (data[i+1] >= data[i])) {
            is_safe = false;
            break;
        }

        int difference = abs(data[i] - data[i+1]);

        if ((difference < 1) || (difference > 3)) {
            is_safe = false;
            break;
        }
    }

    if (!is_safe && is_dampener_used) {
        for (size_t i = 0; i < data.size(); i++) {
            vector<int> new_data;
            for (size_t j = 0; j < data.size(); j++) {
                if (j != i) {
                    new_data.push_back(data[j]);
                }
            }

            Report sub_report(new_data);

            if (sub_report.is_safe(false)) {
                is_safe = true;
                break;
            }
        }
    }

    return is_safe;
}
