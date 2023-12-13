// https://adventofcode.com/2023/day/5

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <chrono>
//#include <string_view>

#define NUM_LOOPS 100000
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

std::string readFileIntoString(const std::string& path) {
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '" << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

void parse(const std::string& file_content, std::vector<seedsize_t>& seeds, std::array<std::vector<map_entry>, MAP_COUNT>& mapLevels) {
    std::istringstream iss(file_content);
    size_t mapIndex = -1;

    // Process the first line for seeds
    std::string line;
    line.reserve(300);

    std::getline(iss, line);
    std::istringstream seed_stream(line);
    seed_stream.ignore(6);

    seedsize_t seed;
    while (seed_stream >> seed) {
        seeds.emplace_back(seed);
    }

    // Process the rest of the file
    while (std::getline(iss, line)) {
        if (line.empty()) {
            mapIndex++;
            continue;
        } else if (line.back() == ':') {
            continue;
        }

        seedsize_t dest, src, range;

        sscanf(line.c_str(), "%u %u %u", &dest, &src, &range);
        mapLevels[mapIndex].emplace_back(map_entry{dest, src, range});
    }
}

seedsize_t part1(const std::vector<seedsize_t>& seeds, const std::array<std::vector<map_entry>, MAP_COUNT>& mapLevels) {
    seedsize_t nearestLocation = 0xFFFFFFFF;

    for (const auto& s: seeds) {
        seedsize_t dest = s;

        for (const auto& mapLevel : mapLevels) {
            for (const auto& map : mapLevel) {
                if (map.src <= dest && dest < map.src+map.range) {
                    seedsize_t offset = map.dest - map.src;
                    dest += offset;
                    break;
                }
            }
        }

        nearestLocation = std::min(nearestLocation, dest);
    }

    return nearestLocation;
}

seedsize_t part2(const std::vector<seedsize_t>& seeds, const std::array<std::vector<map_entry>, MAP_COUNT>& mapLevels) {
    std::vector<seed_range> seedRangesOG;
    seedRangesOG.reserve(10);

    for (size_t i = 1; i < seeds.size(); i+=2) {
        seedRangesOG.emplace_back(seeds[i-1], seeds[i-1]+seeds[i]);
    }

    seedsize_t nearestLocation = 0xFFFFFFFF; // unsigned int max

    // for (int t = 0; t < NUM_LOOPS; t++) {
    std::vector<seed_range> seedRanges = seedRangesOG;

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

    for (const auto& sr : seedRanges) {
        nearestLocation = std::min(nearestLocation, sr.start);
    }

    return nearestLocation;
}

int main() {
    auto file_read_time_start = std::chrono::high_resolution_clock::now();
    std::string file_content = readFileIntoString("input.txt");
    auto file_read_time_end = std::chrono::high_resolution_clock::now();

    std::vector<seedsize_t> seeds;
    std::array<std::vector<map_entry>, MAP_COUNT> mapLevels;
    seeds.reserve(30);

    auto start_time = std::chrono::high_resolution_clock::now();
    parse(file_content, seeds, mapLevels);
    auto end_parse_time = std::chrono::high_resolution_clock::now();

    volatile seedsize_t nearestLocationPart1 = part1(seeds, mapLevels);
    auto end_part1_time = std::chrono::high_resolution_clock::now();
    volatile seedsize_t nearestLocationPart2 = part2(seeds, mapLevels);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto file_read_time = std::chrono::duration_cast<std::chrono::nanoseconds>(file_read_time_end - file_read_time_start).count();
    auto total_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    auto parsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_parse_time - start_time).count();
    auto solution_time_part1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_part1_time - end_parse_time).count();
    auto solution_time_part2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - end_part1_time).count();

    std::cout << "Nearest location for part 1: " << nearestLocationPart1 << "\n";
    std::cout << "Nearest location for part 2: " << nearestLocationPart2 << "\n\n";

    std::cout << "Time spent reading in the file (not included in any of the future timings): " << file_read_time/1000.0 << " microseconds\n\n";

    std::cout << "Time to parse: " << parsed_time/1000.0 << " microseconds\n";
    std::cout << "Time to solve part 1: " << solution_time_part1/1000.0 << " microseconds\n";
    std::cout << "Time to solve part 2: " << solution_time_part2/1000.0 << " microseconds\n";
    std::cout << "Total time elapsed: " << total_elapsed/1000.0 << " microseconds\n\n";

    std::cout << "Starting benchmarks now...\n";

    start_time = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < NUM_LOOPS; i++) {
        seeds.clear();
        mapLevels = {};
        parse(file_content, seeds, mapLevels);
    }
    end_parse_time = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < NUM_LOOPS; i++) {
        nearestLocationPart1 = part1(seeds, mapLevels);
    }
    end_part1_time = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < NUM_LOOPS; i++) {
        nearestLocationPart2 = part2(seeds, mapLevels);
    }
    end_time = std::chrono::high_resolution_clock::now();

    total_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() / NUM_LOOPS;
    parsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_parse_time - start_time).count() / NUM_LOOPS;
    solution_time_part1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_part1_time - end_parse_time).count() / NUM_LOOPS;
    solution_time_part2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - end_part1_time).count() / NUM_LOOPS;

    std::cout << "Time to parse per iteration: " << parsed_time/1000.0 << " microseconds\n";
    std::cout << "Time to solve part 1 per iteration: " << solution_time_part1/1000.0 << " microseconds\n";
    std::cout << "Time to solve part 2 per iteration: " << solution_time_part2/1000.0 << " microseconds\n";
    std::cout << "Total time elapsed per full iteration of parse + part1 + part2: " << total_elapsed/1000.0 << " microseconds\n";
}

