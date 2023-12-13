// https://adventofcode.com/2023/day/5

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <chrono>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string_view>
#include <charconv>

const char* INPUT_FILE = "input.txt";
const size_t MAP_COUNT = 7;
const size_t MAP_ENTRY_SIZE = 40;

#define NUM_LOOPS 100000
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

void parse(const std::string_view& content, std::vector<seedsize_t>& seeds, std::array<std::vector<map_entry>, MAP_COUNT>& maps);
seedsize_t part1(const std::vector<seedsize_t>& seeds, const std::array<std::vector<map_entry>, MAP_COUNT>& maps);
seedsize_t part2(const std::vector<seedsize_t>& seeds, std::vector<seed_range>& seedRanges, std::vector<seed_range>& remappedSeedRanges, const std::array<std::vector<map_entry>, MAP_COUNT>& maps);
seedsize_t parseSeed(std::string_view& line);
map_entry parseMapEntry(std::string_view& line);
std::string_view readFile(const std::string& filename);

int main() {
    std::array<std::vector<map_entry>, MAP_COUNT> maps;
    std::vector<seedsize_t> seeds;
    std::vector<seed_range> seedRanges;
    std::vector<seed_range> remappedSeedRanges;

    for (auto& map: maps) {
        map.reserve(40);
    }
    seeds.reserve(20);
    seedRanges.reserve(100);
    remappedSeedRanges.reserve(100);

    auto file_read_time_start = std::chrono::high_resolution_clock::now();
    std::string_view fileContent = readFile(INPUT_FILE);
    auto file_read_time_end = std::chrono::high_resolution_clock::now();

    auto start_time = std::chrono::high_resolution_clock::now();
    parse(fileContent, seeds, maps);
    auto end_parse_time = std::chrono::high_resolution_clock::now();

    volatile seedsize_t nearestLocationPart1 = part1(seeds, maps);
    auto end_part1_time = std::chrono::high_resolution_clock::now();
    volatile seedsize_t nearestLocationPart2 = part2(seeds, seedRanges, remappedSeedRanges, maps);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto file_read_time = std::chrono::duration_cast<std::chrono::nanoseconds>(file_read_time_end - file_read_time_start).count();
    auto total_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    auto parsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_parse_time - start_time).count();
    auto solution_time_part1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_part1_time - end_parse_time).count();
    auto solution_time_part2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - end_part1_time).count();

    std::cout << "Nearest location for part 1: " << nearestLocationPart1 << "\n";
    std::cout << "Nearest location for part 2: " << nearestLocationPart2 << "\n\n";

    std::cout << "Reading in the file*: " << file_read_time/1000.0 << " microseconds (*not included in any of the below calculations)\n\n";

    std::cout << std::left << std::setfill(' ')
              << std::setw(9) << "Parsing:"
              << std::setw(7) << parsed_time/1000.0 << " microseconds\n";

    std::cout << std::left << std::setfill(' ')
              << std::setw(9) << "Part 1:"
              << std::setw(7) << solution_time_part1/1000.0 << " microseconds\n";

    std::cout << std::left << std::setfill(' ')
              << std::setw(9) << "Part 2:"
              << std::setw(7) << solution_time_part2/1000.0  << " microseconds\n";

    std::cout << std::left << std::setfill(' ')
              << std::setw(9) << "Total:"
              << std::setw(7) << total_elapsed/1000.0  << " microseconds\n\n";

    std::cout << "+--------------------------------------------------+\n";
    std::cout << "|  Starting benchmarks now... (" << NUM_LOOPS << " iterations)  |\n";
    std::cout << "+--------------------------------------------------+\n\n";

    start_time = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < NUM_LOOPS; i++) {
        seeds.clear();
        for (auto& map : maps) {
            map.clear();
        }
        parse(fileContent, seeds, maps);
    }
    end_parse_time = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < NUM_LOOPS; i++) {
        nearestLocationPart1 = part1(seeds, maps);
    }
    end_part1_time = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < NUM_LOOPS; i++) {
        seedRanges.clear();
        remappedSeedRanges.clear();
        nearestLocationPart2 = part2(seeds, seedRanges, remappedSeedRanges, maps);
    }
    end_time = std::chrono::high_resolution_clock::now();

    total_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() / NUM_LOOPS;
    parsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_parse_time - start_time).count() / NUM_LOOPS;
    solution_time_part1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_part1_time - end_parse_time).count() / NUM_LOOPS;
    solution_time_part2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - end_part1_time).count() / NUM_LOOPS;

    std::cout << std::left << std::setfill(' ')
              << std::setw(9) << "Parsing:"
              << std::setw(7) << parsed_time/1000.0 << " microseconds / iteration\n";

    std::cout << std::left << std::setfill(' ')
              << std::setw(9) << "Part 1:"
              << std::setw(7) << solution_time_part1/1000.0 << " microseconds / iteration\n";

    std::cout << std::left << std::setfill(' ')
              << std::setw(9) << "Part 2:"
              << std::setw(7) << solution_time_part2/1000.0  << " microseconds / iteration\n";

    std::cout << std::left << std::setfill(' ')
              << std::setw(9) << "Total:"
              << std::setw(7) << total_elapsed/1000.0  << " microseconds / iteration\n";

    munmap(const_cast<char*>(fileContent.data()), fileContent.size());
}


seedsize_t parseSeed(std::string_view& line) {
    seedsize_t seed;
    size_t spacePos = line.find(' ');

    if (spacePos == std::string_view::npos) {
        spacePos = line.size();
    }

    auto [ptr, ec] = std::from_chars(line.data(), line.data() + spacePos, seed);
    if (ec == std::errc()) {
        line.remove_prefix(std::min(spacePos + 1, line.size()));
        return seed;
    }

    return 0;
}

map_entry parseMapEntry(std::string_view& line) {
    seedsize_t dest = parseSeed(line);
    seedsize_t src = parseSeed(line);
    seedsize_t range = parseSeed(line);
    return {dest, src, range};
}

void parse(const std::string_view& content, std::vector<seedsize_t>& seeds, std::array<std::vector<map_entry>, MAP_COUNT>& maps) {
    size_t start = 0;
    size_t end = 0;

    if ((start=content.find(':', start)) == std::string_view::npos) {
        std::cerr << "Invalid data, unable to read in seeds" << std::endl;
        exit(1);
    }

    start += 2;

    {   // parse seeds
        end = content.find('\n');
        std::string_view line = content.substr(start, end - start);

        while (!line.empty()) {
            seeds.emplace_back(parseSeed(line));
            seeds.emplace_back(parseSeed(line));
        }

        start = end + 1;
    }

    size_t mapIndex = -1;
    while (start < content.size()) {
        if ((end=content.find('\n', start)) == std::string_view::npos) {
            end = content.size();
        }

        std::string_view line = content.substr(start, end - start);

        if (line.back() == ':') {
            mapIndex++;
        } else {
            while (!line.empty()) {
                maps[mapIndex].emplace_back(parseMapEntry(line));
            }
        }

        start = end + 1;
    }
}

std::string_view readFile(const std::string& filename) {
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return {};
    }

    // Get file size
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error getting file size");
        close(fd);
        return {};
    }

    void* mapped = mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return {};
    }

    close(fd);

    return std::string_view(static_cast<char*>(mapped), sb.st_size);
}

seedsize_t part1(const std::vector<seedsize_t>& seeds, const std::array<std::vector<map_entry>, MAP_COUNT>& maps) {
    seedsize_t nearestLocation = 0xFFFFFFFF;

    for (const auto& s: seeds) {
        seedsize_t dest = s;

        for (const auto& map : maps) {
            for (const auto& mapEntry : map) {
                if (mapEntry.src <= dest && dest < mapEntry.src+mapEntry.range) {
                    seedsize_t offset = mapEntry.dest - mapEntry.src;
                    dest += offset;
                    break;
                }
            }
        }

        nearestLocation = std::min(nearestLocation, dest);
    }

    return nearestLocation;
}

seedsize_t part2(const std::vector<seedsize_t>& seeds, std::vector<seed_range>& seedRanges, std::vector<seed_range>& remappedSeedRanges, const std::array<std::vector<map_entry>, MAP_COUNT>& maps) {
    for (size_t i = 1; i < seeds.size(); i+=2) {
        seedRanges.emplace_back(seeds[i-1], seeds[i-1]+seeds[i]);
    }

    seedsize_t nearestLocation = 0xFFFFFFFF; // unsigned int max

    for (const auto& map: maps) {
        // for each mapping level, we will create a new vector of
        // what seed ranges there are when accounting for transformations
        // that occur in this map (which then propogate to future maps)
        remappedSeedRanges.clear();

        while (!seedRanges.empty()) {
            // we will process every seed value we have so far and we will only
            // reinsert the original values into the new seed ranges vector if
            // we don't find any overlap in the current map level (vector of map_entries)
            const auto [start, end] = seedRanges.back();
            seedRanges.pop_back(); // just get rid of this value to reduce complex checks later

            for (const auto& mapEntry : map) {
                // -------     seed_range (start and end)
                //     ------- map_entry  (src through src+range)
                // ^   ^ ^     arrows point to start, overlapStart and end=overlapEnd respectively
                seedsize_t overlapStart = std::max(start, mapEntry.src);
                seedsize_t overlapEnd = std::min(end, mapEntry.src + mapEntry.range);

                if (overlapStart >= overlapEnd) { continue; } // no overlap

                seedsize_t newSeedOffset = mapEntry.dest - mapEntry.src; // or, how far away is src from the destination
                remappedSeedRanges.emplace_back(seed_range{overlapStart + newSeedOffset, overlapEnd + newSeedOffset});
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
            remappedSeedRanges.emplace_back(seed_range{start, end});

foundNewInterval:
            continue;
        }

        // seedRanges is empty, so we repopulate it with all our new intervals for the next stage
        seedRanges = remappedSeedRanges;
    }

    for (const auto& sr : seedRanges) {
        nearestLocation = std::min(nearestLocation, sr.start);
    }

    return nearestLocation;
}

