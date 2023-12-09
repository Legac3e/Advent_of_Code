// https://adventofcode.com/2023/day/3

#include <iostream>
#include <fstream>
#include <string>
//#include <string_view>
#include <vector>
#include <stdio.h>

const char* INPUT_FILE = "input.txt";

struct enginePart {
    size_t row{0};
    size_t col{0};
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
    for (size_t row = 0; row < engineBoard.size(); row++) {
        for (size_t col = 0; col < engineBoard[row].size(); col++) {
            if (!std::isdigit(engineBoard[row][col])) { continue; }

            parts.emplace_back();
            enginePart& part = parts.back();
            part.row=row;
            part.col=col;

            sscanf(&engineBoard[row][col], "%d%n", &part.value, &part.length);

            col += part.length;

            // if we can scan one character before, make that the new start
            if (part.col >= 1) {
                part.col--;
                part.length++;
            }

            // if we can scan the next character, increase the bounds
            if (part.col + part.length + 1 < engineBoard[row].size()) {
                part.length++;
            }
        }
    }

    inputfile.close();

    // process all of the numbers we found by scanning for symbols
    // within 1 character of it. std::max and std::min clamp our rows
    for (auto& part : parts) {
        for (size_t rowOffset = std::max(int(part.row)-1, 0); !part.found && rowOffset <= std::min(int(part.row)+1, int(engineBoard.size()-1)); rowOffset++) {
            for (size_t colOffset = 0; colOffset < part.length; colOffset++) {
                char symbol = engineBoard[rowOffset][part.col+colOffset];
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

