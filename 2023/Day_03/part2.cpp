// https://adventofcode.com/2023/day/3

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>

const char* INPUT_FILE = "input.txt";

struct gear {
    int row{0};
    int col{0};
};

struct numPos {
    int row;
    int col;
};

int main() {
    int64_t sumOfGearProducts = 0;
    std::vector<gear> gears;
    std::vector<std::string> engineBoard;

    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }

    while (std::getline(inputfile, engineBoard.emplace_back())) {}

    // find all of the gears
    for (int row = 0; row < int(engineBoard.size()); row++) {
        for (int col = 0; col < int(engineBoard[row].size()); col++) {
            if (engineBoard[row][col] == '*') {
                gears.emplace_back(gear{row, col});
            }
        }
    }

    inputfile.close();

    std::vector<numPos> numberIndex;

    for (auto& gear : gears) {
        numberIndex.clear();

        for (int row = std::max(gear.row-1, 0); row <= std::min(gear.row+1, int(engineBoard.size())-1); row++) {
            bool foundnumber = false;
            for (int col = std::max(gear.col-1, 0); col <= std::min(gear.col+1, int(engineBoard[row].size())-1); col++) {
                if (std::isdigit(engineBoard[row][col])) {
                    if (!foundnumber) {
                        int tempcol = col;

                        // find the start of that number
                        while(tempcol - 1 >= 0 && std::isdigit(engineBoard[row][tempcol-1])) {
                            tempcol--;
                        }

                        numberIndex.emplace_back(numPos{row, tempcol});
                    }

                    foundnumber = true;
                } else {
                    foundnumber = false;
                }
            }
        }

        if (numberIndex.size() == 2) {
            int num1;
            int num2;

            sscanf(&engineBoard[numberIndex[0].row][numberIndex[0].col], "%d", &num1);
            sscanf(&engineBoard[numberIndex[1].row][numberIndex[1].col], "%d", &num2);

            sumOfGearProducts += num1 * num2;
        }
    }

    std::cout << "Sum of all gear products for part 2:\n" << sumOfGearProducts << std::endl;
}

