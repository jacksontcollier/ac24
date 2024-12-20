#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

string diskmap2fileblocks(const string& diskmap);

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

    string diskmap;
    getline(fin, diskmap);

    string fileblocks = diskmap2fileblocks(diskmap);

    cout << fileblocks << "\n";

    return 0;
}

string diskmap2fileblocks(const string& diskmap)
{
    bool is_reading_file_block = true;
    size_t disk_id = 0;
    string fileblocks;
    
    for (size_t i = 0; i < diskmap.size(); i++) {
        int digit = diskmap[i] - '0';
        if (is_reading_file_block) {
            for (int j = 0; j < digit; j++) {
                fileblocks.push_back('0' + disk_id);
            }
            is_reading_file_block = false;
            disk_id++;
        } else {
            for (int j = 0; j < digit; j++) {
                fileblocks.push_back('.');
            }
            is_reading_file_block = true;
        }
    }

    return fileblocks;
}
