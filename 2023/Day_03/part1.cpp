// https://adventofcode.com/2023/day/3

#include <iostream>
#include <fstream>
#include <string>
//#include <string_view>
#include <vector>
#include <stdio.h>

const char* INPUT_FILE = "input.txt";

struct position {
    int row;
    int col;
};

struct enginePart {
    position pos {.row=-1, .col=-1};
    int value{0};
    int length {0};
    bool found{false};
};

int main() {
    std::vector<enginePart> candidateParts;
    std::vector<std::string> engineBoard;

    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }

    while (std::getline(inputfile, engineBoard.emplace_back())) {}

    for (size_t row = 0; row < engineBoard.size(); row++) {
        for (size_t col = 0; col < engineBoard[row].size(); col++) {
            if (!std::isdigit(engineBoard[row][col])) { continue; }

            candidateParts.emplace_back();
            candidateParts.back().pos.row=row;
            candidateParts.back().pos.col=col;

            sscanf(&engineBoard[row][col], "%d%n", &candidateParts.back().value, &candidateParts.back().length);

            col += candidateParts.back().length;

            if (candidateParts.back().pos.col >= 1) {
                candidateParts.back().pos.col--;
                candidateParts.back().length++;
            }

            if (candidateParts.back().pos.col + candidateParts.back().length + 1 < engineBoard[row].size()) {
                candidateParts.back().length++;
            }
        }
    }

    inputfile.close();

    for (size_t i = 0; i < candidateParts.size(); i++) {
        int row = candidateParts[i].pos.row;
        int colOffset = candidateParts[i].pos.col;
        char symbol;

        if (row > 0) { // check above
            for (int cCol = 0; cCol < candidateParts[i].length; cCol++) {
                symbol = engineBoard[row-1][cCol+colOffset];
                if (!std::isdigit(symbol) && symbol != '.') {
                    candidateParts[i].found = true;
                    break;
                }
            }
        }

        if (candidateParts[i].found) { continue; }

        // check neighbors
        symbol = engineBoard[row][colOffset];
        if (!std::isdigit(symbol) && symbol != '.') {
            candidateParts[i].found = true;
            continue;
        }

        symbol = engineBoard[row][colOffset + candidateParts[i].length - 1];

        if (!std::isdigit(symbol) && symbol != '.') {
            candidateParts[i].found = true;
            continue;
        }

        if (row + 1 < engineBoard.size()) { // check below
            for (int cCol = 0; cCol < candidateParts[i].length; cCol++) {
                symbol = engineBoard[row+1][cCol+colOffset];
                if (!std::isdigit(symbol) && symbol != '.') {
                    candidateParts[i].found = true;
                    break;
                }
            }
        }
    }

    int enginePartSum = 0;
    for (const auto& candPart : candidateParts) {
        if (candPart.found) {
            enginePartSum += candPart.value;

            /*
            std::cout << "candidate part w/ value: " << candPart.value
                      << " and {row,col}: {" << candPart.pos.row << "," << candPart.pos.col << "}"
                      << " and length " << candPart.length << "\n";
            */
        }
    }

    std::cout << "Sum of all part numbers for part 1:\n" << enginePartSum << std::endl;
}

