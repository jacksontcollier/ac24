#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct FreeBlock
{
    size_t location;
    size_t size;
};

struct FileBlock
{
    size_t id;
    size_t location;
    size_t size;
};

vector<int> diskmap2fileblocks(const string& diskmap);

void print_fileblocks(const vector<int>& fileblocks);

void compact(vector<int>& fileblocks);

vector<int> compact2(const string& diskmap);

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

    vector<int> compact2_fileblocks = compact2(diskmap);
    cout << "\n";
    cout << "Part 2 FileBlocks\n";
    cout << "=================\n";
    for (size_t i = 0; i < compact2_fileblocks.size(); i++) {
        cout << compact2_fileblocks[i] << " ";
    }
    cout << "\n";

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

vector<int> compact2(const string& diskmap)
{
    vector<FileBlock> file_blocks;
    map<size_t, FreeBlock> free_blocks;
    vector<int> compact_file_blocks;
    bool is_reading_file_block = true;
    size_t file_block_id = 0;
    size_t location = 0;

    for (size_t i = 0; i < diskmap.size(); i++) {
        if (is_reading_file_block) {
            FileBlock fb;
            fb.id = file_block_id;
            fb.location = location;
            fb.size = diskmap[i] - '0';
            file_block_id++;
            location += fb.size;
            file_blocks.push_back(fb);
            for (size_t i = 0; i < fb.size; i++) {
                compact_file_blocks.push_back(0);
            }
        } else {
            FreeBlock free_block;
            free_block.location = location;
            free_block.size = diskmap[i] - '0';
            free_blocks.insert(make_pair(free_block.location, free_block));
            location += free_block.size;
            for (size_t i = 0; i < free_block.size; i++) {
                compact_file_blocks.push_back(0);
            }
        }

        is_reading_file_block = !is_reading_file_block;
    }

    /*
    cout << "Free Blocks\n";
    cout << "===========\n";
    for (auto& mit : free_blocks) {
        size_t location = mit.first;
        cout << "Location: " << location << ", Size: " << mit.second->size << "\n";
    }

    cout << "\n";
    cout << "File Blocks\n";
    cout << "===========\n";
    for (size_t i = 0; i < file_blocks.size(); i++) {
        cout << "Location: " << file_blocks[i]->location << ", ID: "
             << file_blocks[i]->id << ", Size: " << file_blocks[i]->size << "\n";
    }
    */

    map<size_t, FileBlock> relocated_file_blocks;
    for (signed long i = file_blocks.size() - 1; i >= 0; i--) {
        FileBlock& file_block = file_blocks[i];
        bool found_file_block = false;
        for (auto mit = free_blocks.begin(); mit != free_blocks.end(); mit++) {
            FreeBlock& free_block = mit->second;
            if (free_block.size >= file_block.size) {
                // Found new location for file block
                FileBlock new_file_block;
                new_file_block.id = file_block.id;
                new_file_block.location = free_block.location;
                new_file_block.size = file_block.size;
                relocated_file_blocks.insert(make_pair(new_file_block.location, new_file_block));
                free_blocks.erase(mit);
                if (free_block.size > file_block.size) {
                    FreeBlock new_free_block;
                    new_free_block.size = free_block.size - file_block.size;
                    new_free_block.location = free_block.location + file_block.size;
                    free_blocks.insert(make_pair(new_free_block.location, new_free_block));
                }
                FreeBlock new_free_block(FreeBlock());
                new_free_block->size = file_block.size;
                new_free_block->location = file_block.location;
                free_blocks.insert(make_pair(new_free_block.location, new_free_block));
                found_file_block = true;
                break;
            }
        }
        if (!found_file_block) {
            relocated_file_blocks.insert(make_pair(file_block->location, file_block));
        }
    }

    for (auto relocated_file_blocks_iter : relocated_file_blocks) {
        shared_ptr<FileBlock> file_block = relocated_file_blocks_iter.second;
        for (size_t i = file_block->location; i < (file_block->location + file_block->size); i++) {
            compact_file_blocks[i] = file_block->id;
        }
    }

    for (auto free_block_iter : free_blocks) {
        shared_ptr<FreeBlock> free_block = free_block_iter.second;
        for (size_t i = free_block->location; i < (free_block->location + free_block->size); i++) {
            compact_file_blocks[i] = -1;
        }
    }

    return compact_file_blocks;
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
