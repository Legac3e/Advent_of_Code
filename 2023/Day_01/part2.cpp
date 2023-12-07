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

std::string_view numbers[] = {
    "zero", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine"
};

digit_pair get_digits(std::string_view str) {
    digit_pair digits {-1, -1};

    for (auto it = str.begin(); it < str.end(); it++) {
        switch (*it) {
            case 'z': { // "zero" b/c I still believe in 0, even if the elves don't
                if (std::string_view(it, 4) == numbers[0]) { digits.last = 0; }
            } break;

            case 'o': { // "one"
                if (std::string_view(it, 3) == numbers[1]) { digits.last = 1; }
            } break;

            case 't': { // "two", "three"
                if (std::string_view(it, 3) == numbers[2]) { digits.last = 2; break; }
                if (std::string_view(it, 5) == numbers[3]) { digits.last = 3; }
            } break;

            case 'f': { // "four", "five"
                if (std::string_view(it, 4) == numbers[4]) { digits.last = 4; break; }
                if (std::string_view(it, 4) == numbers[5]) { digits.last = 5; }
            } break;

            case 's': { // "six", "seven"
                if (std::string_view(it, 3) == numbers[6]) { digits.last = 6; break; }
                if (std::string_view(it, 5) == numbers[7]) { digits.last = 7; }
            } break;

            case 'e': { // "eight"
                if (std::string_view(it, 5) == numbers[8]) { digits.last = 8; }
            } break;

            case 'n': { // "nine"
                if (std::string_view(it, 4) == numbers[9]) { digits.last = 9; }
            } break;

            default: { // maybe it's an actual digit
                if (std::isdigit(*it)) { digits.last = *it - '0'; }
            } break;
        }

        if (digits.first == -1) {
            digits.first = digits.last;
        }
    }

    if (digits.first == -1 || digits.last == -1) {
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

    std::cout << "Calibration value for part 2:\n" << calibrationValue << std::endl;
}

