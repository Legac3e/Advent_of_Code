// https://adventofcode.com/2023/day/5

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>

const size_t MAP_COUNT = 7;

#define seedsize uint

struct map_entry {
    seedsize dest;
    seedsize src;
    seedsize range;
};

const char* INPUT_FILE = "input.txt";

int main() {
    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }

    std::vector<seedsize> seeds;

    std::string line;
    std::getline(inputfile, line);

    { // read in the seeds
        size_t i;
        for (i = 0; i < line.size(); i++) {
            if (line[i] == ':') { i += 2; break; }
        }

        for (; i < line.size(); i++) {
            seedsize seed;
            int seedlen;

            sscanf(&line[i], "%u%n", &seed, &seedlen);

            seeds.emplace_back(seed);

            i += seedlen - 1;
        }
    }

    std::array<std::vector<map_entry>, MAP_COUNT> mapEntries;
    size_t mapIndex = -1;

    while (std::getline(inputfile, line)) {
        if (!std::isdigit(line[0])) { // consume 2 lines if the input doesn't start with a number
            std::getline(inputfile, line);
            mapIndex++;
            continue;
        }

        seedsize dest, src, range;
        sscanf(line.c_str(), "%u %u %u", &dest, &src, &range);

        mapEntries[mapIndex].emplace_back(dest, src, range);
    }

    /*
    // prints out all of the map entries for all of the mapLevels
    for (const auto& maplevel : mapEntries) {
        std::cout << "[ ";
        for (const auto& map : maplevel) {
            std::cout << "[" << map.src << ", " << map.dest << ", " << map.src << "] ";
        }
        std::cout << "]" << std::endl;
    }
    */

    inputfile.close();

    seedsize nearestLocation = 0xFFFFFFFF;

    for (const auto& s: seeds) {
        seedsize dest = s;

        for (const auto& maplevel : mapEntries) {
            for (const auto& map : maplevel) {
                if (map.src <= dest && dest < map.src+map.range) {
                    seedsize offset = map.dest - map.src;
                    dest += offset;
                    break;
                }
            }
        }

        nearestLocation = std::min(nearestLocation, dest);
    }

    std::cout << "Nearest location for part 1:\n" << nearestLocation << std::endl;
}

