// https://adventofcode.com/2023/day/1

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

const char* INPUT_FILE = "input.txt";

struct digit_pair {
    int8_t first;
    int8_t last;
};

digit_pair get_digits(std::string_view str) {
    digit_pair digits {-1, -1};

    for (auto it = str.begin(); it < str.end(); it++) {
        if (std::isdigit(*it)) {
            digits.last = *it - '0';

            if (digits.first == -1) {
                digits.first = digits.last;
            }
        }
    }

    if (!digits.first || !digits.last) {
        std::cerr << "Error when finding the digits on line:\n\t" << str << std::endl;
        exit(1);
    }

    return digits;
}

int main() {
    int calibrationValue = 0;

    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file:\n\t" << INPUT_FILE << std::endl;
        exit(1);
    }

    std::string line;

    while (std::getline(inputfile, line)) {
        digit_pair digits = get_digits(line);
        calibrationValue += (10*digits.first) + digits.last;
    }

    inputfile.close();

    std::cout << "Calibration value for part 1:\n" << calibrationValue << std::endl;
}

