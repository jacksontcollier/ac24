#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> diskmap2fileblocks(const string& diskmap);

void print_fileblocks(const vector<int>& fileblocks);

void compact(vector<int>& fileblocks);

unsigned long long calc_checksum(const vector<int>& fileblocks);

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

    vector<int> fileblocks = diskmap2fileblocks(diskmap);

    compact(fileblocks);
    unsigned long long checksum = calc_checksum(fileblocks);
    cout << "Part 1 Answer: Checksum is " << checksum << "\n";

    return 0;
}

vector<int> diskmap2fileblocks(const string& diskmap)
{
    bool is_reading_file_block = true;
    size_t disk_id = 0;
    vector<int> fileblocks;

    for (size_t i = 0; i < diskmap.size(); i++) {
        int digit = diskmap[i] - '0';
        if (is_reading_file_block) {
            for (int j = 0; j < digit; j++) {
                fileblocks.push_back(disk_id);
            }
            is_reading_file_block = false;
            disk_id++;
        } else {
            for (int j = 0; j < digit; j++) {
                fileblocks.push_back(-1);
            }
            is_reading_file_block = true;
        }
    }

    return fileblocks;
}

void print_fileblocks(const vector<int>& fileblocks)
{
    for (size_t i = 0; i < fileblocks.size(); i++) {
        const int& fb = fileblocks[i];

        if (fb == -1) {
            cout << ".";
        } else {
            cout << fb;
        }

        if (i + 1 < fileblocks.size()) {
            cout << " ";
        }
    }

    cout << "\n";
}

void compact(vector<int>& fileblocks)
{
    while (true) {
        size_t first_free_mem_block_index;
        size_t last_used_mem_block_index;

        // Find first free memory block
        for (size_t i = 0; i < fileblocks.size(); i++) {
            if (fileblocks[i] == -1) {
                first_free_mem_block_index = i;
                break;
            }
        }

        // Find last used memory block
        for (signed long i = fileblocks.size() - 1; i >= 0; i--) {
            if (fileblocks[i] != -1) {
                last_used_mem_block_index = i;
                break;
            }
        }

        // Are we done?
        if (last_used_mem_block_index < first_free_mem_block_index) {
            break;
        }

        // Not done, swap
        fileblocks[first_free_mem_block_index] = fileblocks[last_used_mem_block_index];
        fileblocks[last_used_mem_block_index] = -1;
    }
}

unsigned long long calc_checksum(const vector<int>& fileblocks)
{
    unsigned long long checksum = 0;

    for (size_t i = 0; i < fileblocks.size(); i++) {
        if (fileblocks[i] != -1) {
            checksum += (fileblocks[i] * i);
        }
    }

    return checksum;
}
