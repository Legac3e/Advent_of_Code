// https://adventofcode.com/2023/day/4

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <cstdint>

const char* INPUT_FILE = "input.txt";
const int MAX_COPIES = 214;

struct ScratchCardCopy {
    int num;
    int val;
};

int main() {
    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }

    int scratchCardsProcessed = 0;
    ScratchCardCopy scratchCardCopies[MAX_COPIES] = {0, 0};

    std::string line;
    while (std::getline(inputfile, line)) {
        bool winningTickets[100] = {0};
        size_t i;

        for (i = 0; i < line.size(); i++) {
            if (line[i] == ':') { i += 2; break; }
        }

        for (; i < line.size(); i++) {
            if (line[i] == '|') { break; }
            if (!std::isdigit(line[i])) { continue; }

            int num;
            int len;
            sscanf(&line[i], "%d%n", &num, &len);

            i += len - 1;

            winningTickets[num-1] = true;
        }

        int matches = 0;
        int cardPoints = 0;
        for (; i < line.size(); i++) {
            if (!std::isdigit(line[i])) { continue; }

            int num;
            int len;
            sscanf(&line[i], "%d %n", &num, &len);

            i += len - 1;

            if (winningTickets[num-1]) {
                matches++;
                if (!cardPoints) {
                    cardPoints = 1;
                } else {
                    cardPoints *= 2;
                }
            }
        }

        scratchCardCopies[scratchCardsProcessed].num++;
        scratchCardCopies[scratchCardsProcessed].val = cardPoints;

        for (int j = 0; j < matches && scratchCardsProcessed + j + 1 < MAX_COPIES; j++) {
            // increase the number of copies for as many cards ahead as we have matches
            scratchCardCopies[scratchCardsProcessed+j+1].num += scratchCardCopies[scratchCardsProcessed].num;
        }

        scratchCardsProcessed++;
    }

    int totalPoints = 0;
    int totalScratchCards = 0;

    for (int i = 0; i < MAX_COPIES; i++) {
        totalPoints += scratchCardCopies[i].num * scratchCardCopies[i].val;
        totalScratchCards += scratchCardCopies[i].num;
    }

    std::cout << "Total scratchcard count for part 2:\n" << totalScratchCards << std::endl;
    std::cout << "(Bonus for part 2: they would be worth " << totalPoints << " points)" << std::endl;
}

