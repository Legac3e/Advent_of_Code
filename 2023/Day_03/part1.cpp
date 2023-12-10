// https://adventofcode.com/2023/day/3

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>

const char* INPUT_FILE = "input.txt";

struct enginePart {
    int row{0};
    int col{0};
    int value{0};
    int length{0};
    bool found{false};
};

int main() {
    std::vector<enginePart> parts;
    std::vector<std::string> engineBoard;

    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }

    while (std::getline(inputfile, engineBoard.emplace_back())) {}

    // find all groups of digits and extract the integer value,
    // and store its location for easy processing in the next step
    for (int row = 0; row < int(engineBoard.size()); row++) {
        for (int col = 0; col < int(engineBoard[row].size()); col++) {
            if (!std::isdigit(engineBoard[row][col])) {
                continue;
            }

            parts.emplace_back(enginePart{row, col, 0, 0, false});
            enginePart& part = parts.back();

            sscanf(&engineBoard[row][col], "%d%n", &part.value, &part.length);

            col += part.length;

            int temp = std::max(part.col-1, 0);
            if (temp != part.col) {
                part.col--;
                part.length++;
            }
        }
    }

    inputfile.close();

    // process all of the numbers we found by scanning for symbols
    // within 1 character of it. std::max and std::min clamp our rows
    for (auto& part : parts) {
        for (int row = std::max(part.row-1, 0); !part.found && row <= std::min(part.row+1, int(engineBoard.size())-1); row++) {
            for (int col = part.col; col <= std::min(part.col+part.length, int(engineBoard[row].size())-1); col++) {
                char symbol = engineBoard[row][col];

                if (!std::isdigit(symbol) && symbol != '.') {
                    part.found = true;
                    break;
                }
            }
        }
    }

    int enginePartSum = 0;
    for (const auto& part : parts) {
        if (part.found) {
            enginePartSum += part.value;
        }
    }

    std::cout << "Sum of all part numbers for part 1:\n" << enginePartSum << std::endl;
}

