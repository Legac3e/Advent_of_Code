// https://adventofcode.com/2023/day/5

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct race_entry {
    long long int time;
    long long int recordDistance;
};

const char* INPUT_FILE = "input.txt";

int main() {
    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }

    std::vector<race_entry> races;

    for (int raceNum = 0; raceNum < 2; raceNum++) {
        int entries = 0;
        std::string line;
        std::getline(inputfile, line);

        size_t i;
        for (i = 0; i < line.size(); i++) {
            if (line[i] == ':') { i += 1; break; }
        }

        for (size_t j = i; j < line.size(); j++) {
            if (line[j] == ' ') {
                line.erase(j, 1);
                j = i;
            }
        }

        for (; i < line.size(); i++) {
            long long int num;
            int len;

            sscanf(&line[i], "%lld%n", &num, &len);

            if (raceNum == 0) {
                races.emplace_back(race_entry{num, 0});
            } else {
                races[entries++].recordDistance = num;
            }

            i += len - 1;
        }
    }

    long long int numWins = 0;

    for (const auto& race : races) {
        long long int i = 0;
        while (i * (race.time-i) < race.recordDistance) { i++; }

        numWins = race.time-(2*i)+1;
    }

    std::cout << "Ways you could have won:\n" << numWins << std::endl;
}

