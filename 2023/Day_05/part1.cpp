// https://adventofcode.com/2023/day/5

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <chrono>

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

    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<seedsize> seeds;

    std::string line;
    std::getline(inputfile, line);

    for (size_t i = 0; i < line.size(); i++) { // read in the seeds
        if (!std::isdigit(line[i])) { continue; }

        int numDigitsRead;
        seedsize seed;

        sscanf(&line[i], "%u%n", &seed, &numDigitsRead);

        seeds.emplace_back(seedsize{seed});

        i += numDigitsRead - 1;
    }

    std::array<std::vector<map_entry>, MAP_COUNT> mapLevels;
    size_t mapIndex = -1;

    while (std::getline(inputfile, line)) {
        if (!std::isdigit(line[0])) { // consume 2 lines if the input doesn't start with a number
            std::getline(inputfile, line);
            mapIndex++;
            continue;
        }

        seedsize dest, src, range;
        sscanf(line.c_str(), "%u %u %u", &dest, &src, &range);

        mapLevels[mapIndex].emplace_back(map_entry{dest, src, range});
    }

    inputfile.close();

    seedsize nearestLocation = 0xFFFFFFFF;

    for (const auto& s: seeds) {
        seedsize dest = s;

        for (const auto& mapLevel : mapLevels) {
            for (const auto& map : mapLevel) {
                if (map.src <= dest && dest < map.src+map.range) {
                    seedsize offset = map.dest - map.src;
                    dest += offset;
                    break;
                }
            }
        }

        nearestLocation = std::min(nearestLocation, dest);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Nearest location for part 1:\n" << nearestLocation << std::endl;
    std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() << " nanoseconds" << std::endl;
}

