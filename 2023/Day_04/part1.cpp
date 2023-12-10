// https://adventofcode.com/2023/day/4

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

const char* INPUT_FILE = "input.txt";

int main() {
    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }

    int totalPoints = 0;

    std::string line;
    while (std::getline(inputfile, line)) {
        bool winningTickets[100] = {0};
        size_t i;
        bool processNums = false;

        for (i = 0; i < line.size(); i++) {
            if (line[i] == ':') { processNums = true; continue; }
            if (line[i] == '|') { break; }
            if (!processNums || !std::isdigit(line[i])) { continue; }

            int num;
            int len;
            sscanf(&line[i], "%d%n", &num, &len);

            i += len - 1;

            winningTickets[num-1] = true;
        }

        int cardPoints = 0;
        for (; i < line.size(); i++) {
            if (!std::isdigit(line[i])) { continue; }

            int num;
            int len;
            sscanf(&line[i], "%d %n", &num, &len);

            i += len - 1;

            if (winningTickets[num-1]) {
                if (!cardPoints) {
                    cardPoints = 1;
                } else {
                    cardPoints *= 2;
                }
            }
        }

        totalPoints += cardPoints;
    }

    std::cout << "Sum of all card points for part 1:\n" << totalPoints << std::endl;
}

