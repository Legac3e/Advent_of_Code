// https://adventofcode.com/2023/day/5

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <chrono>

const size_t MAP_COUNT = 7;

#define seedsize unsigned int

struct seed_range {
    seedsize start;
    seedsize end;
};

struct map_entry {
    seedsize dest;
    seedsize src;
    seedsize range;
};

const char* INPUT_FILE = "input.txt";

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }

    std::vector<seed_range> seedRanges;

    std::string line;
    std::getline(inputfile, line);

    for (size_t i = 0; i < line.size(); i++) { // read in the seeds
        if (!std::isdigit(line[i])) { continue; }

        int numDigitsRead;
        seedsize start;
        seedsize range;

        sscanf(&line[i], "%u %u%n", &start, &range, &numDigitsRead);

        seedsize end = start+range;
        seedRanges.emplace_back(start, end);

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

        mapLevels[mapIndex].emplace_back(dest, src, range);
    }

    inputfile.close();

    for (const auto& mapLevel: mapLevels) {
        std::vector<seed_range> remappedSeedRanges;

        while (!seedRanges.empty()) {
            const auto [start, end] = seedRanges.back();
            seedRanges.pop_back();

            for (const auto& map : mapLevel) {
                const auto& [mapDest, mapSrc, mapRange] = map;

                seedsize overlapStart = std::max(start, mapSrc);
                seedsize overlapEnd = std::min(end, mapSrc + mapRange);

                if (overlapStart >= overlapEnd) { continue; }

                seedsize mapDisplacement = mapDest - mapSrc;
                remappedSeedRanges.emplace_back(overlapStart + mapDisplacement, overlapEnd + mapDisplacement);

                if (overlapStart > start) {
                    seedRanges.emplace_back(start, overlapStart);
                }

                if (end > overlapEnd) {
                    seedRanges.emplace_back(overlapEnd, end);
                }

                goto foundremapping;
            }

            remappedSeedRanges.emplace_back(start, end);

foundremapping:
            continue;
        }

        seedRanges = remappedSeedRanges;
    }

    seedsize nearestLocation = 0x7FFFFFFF;
    for (const auto& sr : seedRanges) {
        nearestLocation = std::min(nearestLocation, sr.start);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Nearest location for part 2:\n" << nearestLocation << std::endl;
    std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() << " nanoseconds" << std::endl;
}

