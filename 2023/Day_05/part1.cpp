// https://adventofcode.com/2023/day/5

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

const size_t MAP_COUNT = 7;
struct map_entry {
    uint dest;
    uint src;
    uint range;
};

const char* INPUT_FILE = "input.txt";

int main() {
    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }

    std::vector<uint> seeds;

    std::string line;
    std::getline(inputfile, line);
    bool incrementedType = false;

    { // read in the seeds
        size_t i;
        for (i = 0; i < line.size(); i++) {
            if (line[i] == ':') { i += 2; break; }
        }

        for (; i < line.size(); i++) {
            int num;
            int len;

            sscanf(&line[i], "%d%n", &num, &len);

            seeds.emplace_back(num);

            i += len - 1;
        }

        incrementedType = true;
    }

    size_t mapCategoryIndex = 0;

    std::vector<map_entry> mapCategories[MAP_COUNT];

    while (std::getline(inputfile, line)) {
        if (!std::isdigit(line[0])) {
            if (!incrementedType) {
                mapCategoryIndex++;
                incrementedType = true;
            }

            continue;
        }

        incrementedType = false;
        mapCategories[mapCategoryIndex].emplace_back();
        map_entry& map = mapCategories[mapCategoryIndex].back();

        sscanf(line.c_str(), "%u %u %u", &map.dest, &map.src, &map.range);
    }

    inputfile.close();

    uint nearestLocation = 0xFFFFFFFF;

    for (size_t si = 0; si < seeds.size(); si++) {
        uint dest = seeds[si];

        for (size_t ci = 0; ci < MAP_COUNT; ci++) {
            for (size_t i = 0; i < mapCategories[ci].size(); i++) {
                uint currentSource = mapCategories[ci][i].src;
                uint currentRange = mapCategories[ci][i].range;
                if (currentSource <= dest && dest <= currentSource+currentRange) {
                    uint distanceFromSource = dest - currentSource;
                    dest = mapCategories[ci][i].dest + distanceFromSource;
                    break;
                }
            }
        }

        if (dest < nearestLocation) {
            nearestLocation = dest;
        }
    }

    std::cout << "Nearest location for part 1:\n" << nearestLocation << std::endl;
}

