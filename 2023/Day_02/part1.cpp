// https://adventofcode.com/2023/day/2

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

const char* INPUT_FILE = "input.txt";

struct RGB_Cubes {
    int red;
    int green;
    int blue;
};

const RGB_Cubes reference = {.red=12, .green=13, .blue=14};

bool isGamePossible(std::string_view str) {
    RGB_Cubes cubes = {reference.red, reference.green, reference.blue};

    auto it = str.begin();

    while (it < str.end() && *it != ':') { it++; }

    for (; it < str.end(); it++) {
        if (!std::isdigit(*it)) {
            if (*it == ';') {
                cubes = {reference.red, reference.green, reference.blue};
            }

            continue;
        }

        int numCubes = (*it) - '0';

        if ((it+1) < str.end() && std::isdigit(*(it+1))) {
            numCubes *= 10;
            numCubes += *(it+1) - '0';
            it++;
        }

        switch (*(it+2)) {
            case 'r': {
                cubes.red -= numCubes;
                if (cubes.red < 0) { return false; }
            } break;

            case 'g': {
                cubes.green -= numCubes;
                if (cubes.green < 0) { return false; }
            } break;

            case 'b': {
                cubes.blue -= numCubes;
                if (cubes.blue < 0) { return false; }
            } break;

            default: {
            } break;
        }
    }

    return true;
}

int main() {
    int gameIDSum = 0;
    int gameID = 0;

    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }


    std::string line;

    while (std::getline(inputfile, line)) {
        gameID++;

        if (isGamePossible(line)) {
            gameIDSum += gameID;
        }
    }

    inputfile.close();

    std::cout << "Sum of possible Game IDs for part 1:\n" << gameIDSum << std::endl;
}

