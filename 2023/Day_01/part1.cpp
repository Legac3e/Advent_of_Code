// https://adventofcode.com/2023/day/1

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

const char* INPUT_FILE = "input.txt";

int getFirstDigit(std::string& str) {
    for (auto it = str.begin(); it < str.end(); it++) {
        if (std::isdigit(*it)) {
            return *it - '0';
        }
    }

    std::cerr << "Unable to find the first digit." << std::endl;
    return -1;
}

int getLastDigit(const std::string& str) {
    for (auto it = str.rbegin(); it < str.rend(); it++) {
        if (std::isdigit(*it)) {
            return *it - '0';
        }
    }

    std::cerr << "Unable to find the last digit." << std::endl;
    return -1;
}

int main() {
    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    int sum = 0;
    std::string line;

    while (std::getline(inputfile, line)) {
        sum += (10*getFirstDigit(line)) + getLastDigit(line);
    }

    std::cout << sum << std::endl;

    inputfile.close();
}

