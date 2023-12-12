// https://adventofcode.com/2023/day/5

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <chrono>

const size_t MAP_COUNT = 7;

#define seedsize_t unsigned int

struct seed_range {
    seedsize_t start;
    seedsize_t end;
};

struct map_entry {
    seedsize_t dest;
    seedsize_t src;
    seedsize_t range;
};

const char* INPUT_FILE = "input.txt";

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }

    // for all of the paired numbers that represent [seedstart, range]
    std::vector<seed_range> seedRanges;

    std::string line;
    std::getline(inputfile, line);

    for (size_t i = 0; i < line.size(); i++) { // read in the seeds
        if (!std::isdigit(line[i])) { continue; }

        int numDigitsRead;
        seedsize_t start;
        seedsize_t range;

        sscanf(&line[i], "%u %u%n", &start, &range, &numDigitsRead);

        // [start, range] = [78, 14] ->
        // [start, end]   = [78, 78+14] = [78, 92]
        seedsize_t end = start+range;
        seedRanges.emplace_back(seed_range{start, end});

        i += numDigitsRead - 1;
    }

    // level 0 is seed->soil, level 1 is soil->fertilizer, etc.
    std::array<std::vector<map_entry>, MAP_COUNT> mapLevels;
    size_t mapIndex = -1; // size_t max, which will overflow to 0 in the first iteration below

    while (std::getline(inputfile, line)) {
        if (!std::isdigit(line[0])) { // consume 2 lines if the input doesn't start with a number
            std::getline(inputfile, line);
            mapIndex++;
            continue;
        }

        seedsize_t dest, src, range;
        sscanf(line.c_str(), "%u %u %u", &dest, &src, &range);

        mapLevels[mapIndex].emplace_back(map_entry{dest, src, range});
    }

    inputfile.close();

    std::vector<seed_range> newSeedRanges;
    for (const auto& mapLevel: mapLevels) {
        // for each mapping level, we will create a new vector of
        // what seed ranges there are when accounting for transformations
        // that occur in this map (which then propogate to future maps)
        newSeedRanges.clear();

        while (!seedRanges.empty()) {
            // we will process every seed value we have so far and we will only
            // reinsert the original values into the new seed ranges vector if
            // we don't find any overlap in the current map level (vector of map_entries)
            const auto [start, end] = seedRanges.back();
            seedRanges.pop_back(); // just get rid of this value to reduce complex checks later

            for (const auto& map : mapLevel) {
                // -------     seed_range (start and end)
                //     ------- map_entry  (src through src+range)
                // ^   ^ ^     arrows point to start, overlapStart and end=overlapEnd respectively
                seedsize_t overlapStart = std::max(start, map.src);
                seedsize_t overlapEnd = std::min(end, map.src + map.range);

                if (overlapStart >= overlapEnd) { continue; } // no overlap

                seedsize_t newSeedOffset = map.dest - map.src; // or, how far away is src from the destination
                newSeedRanges.emplace_back(seed_range{overlapStart + newSeedOffset, overlapEnd + newSeedOffset});
                // we extracted this interval, no need for further processing within this interval
                // for the current map (otherwise, that'd mean we have one input->two outputs)

                if (overlapStart > start) {
                    // we still need to process some of what came before the interval we just found
                    // so we reappend this into our OG seed ranges, incase some of the seeds
                    // that weren't captured in the current map could overlap with a different map
                    seedRanges.emplace_back(seed_range{start, overlapStart});
                }

                if (end > overlapEnd) {
                    // same as above, but for values that come after the interval we just captured
                    seedRanges.emplace_back(seed_range{overlapEnd, end});
                }

                // because we did find a new interval, and cut the interval up into one or more
                // pieces, we can skip putting the entire original interval to be processed by
                // the next map... that'd undo a lot of what we are trying to accomplish :P
                goto foundNewInterval;
            }

            // we didn't find any overlapping intervals, so we can put the unaltered range into
            // the new seeds list, which will continue to be processed by the next levels of mapping.
            newSeedRanges.emplace_back(seed_range{start, end});

foundNewInterval:
            continue;
        }

        // seedRanges is empty, so we repopulate it with all our new intervals for the next stage
        seedRanges = newSeedRanges;
    }

    seedsize_t nearestLocation = 0xFFFFFFFF; // unsigned int max
    for (const auto& sr : seedRanges) {
        nearestLocation = std::min(nearestLocation, sr.start);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Nearest location for part 2:\n" << nearestLocation << std::endl;
    std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() << " nanoseconds" << std::endl;
}

