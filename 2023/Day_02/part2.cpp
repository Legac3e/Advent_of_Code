// https://adventofcode.com/2023/day/2

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

int CalculateGamesPower(std::string_view str) {
    RGB_Cubes highest = {0, 0, 0};
    RGB_Cubes cubes = {0, 0, 0};

    auto it = str.begin();

    while (it < str.end() && *it != ':') { it++; }

    for (; it < str.end(); it++) {
        if (!std::isdigit(*it)) {
            if (*it == ';') {
                highest.red = std::max(highest.red, cubes.red);
                highest.green = std::max(highest.green, cubes.green);
                highest.blue = std::max(highest.blue, cubes.blue);
                cubes = {0, 0, 0};
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
                cubes.red += numCubes;
            } break;

            case 'g': {
                cubes.green += numCubes;
            } break;

            case 'b': {
                cubes.blue += numCubes;
            } break;

            default: {
            } break;
        }
    }

    return std::max(highest.red, cubes.red)
         * std::max(highest.green, cubes.green)
         * std::max(highest.blue, cubes.blue);
}

int main() {
    int powerSum = 0;

    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }


    std::string line;

    while (std::getline(inputfile, line)) {
        powerSum += CalculateGamesPower(line);
    }

    inputfile.close();

    std::cout << "Sum of the power levels for all Games in part 1:\n" << powerSum << std::endl;
}

