// https://adventofcode.com/2023/day/1

//#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

const char* INPUT_FILE = "input.txt";

int getFirstDigit(const std::string& str) {
    auto upto = str.end();
    int value = -1;

    // find first occurance of a literal digit
    for (auto it = str.begin(); it < upto; it++) {
        if (std::isdigit(*it)) {
            upto = it;
            value = *it - '0';
            break;
        }
    }

    // find first occurance of a digit as an english word
    // (only going up to what we already found, or 3 before the end)
    upto = std::min(upto, str.end()-3);

    for (auto it = str.begin(); it < upto; it++) {
        switch (*it) {
            case 'o': {
                if(*(it+1)=='n' && *(it+2)=='e') { return 1; };
            } break;

            case 't': {
                if(*(it+1)=='w' && *(it+2)=='o') { return 2; };
                if(it < (upto-2) && *(it+1)=='h' && *(it+2)=='r' && *(it+3)=='e' && *(it+4)=='e') { return 3; };
            } break;

            case 'f': {
                if (it < (upto-1)) {
                    if(*(it+1)=='o' && *(it+2)=='u' && *(it+3)=='r') { return 4; };
                    if(*(it+1)=='i' && *(it+2)=='v' && *(it+3)=='e') { return 5; };
                }
            } break;

            case 's': {
                if(*(it+1)=='i' && *(it+2)=='x') { return 6; };
                if(it < (upto-2) && *(it+1)=='e' && *(it+2)=='v' && *(it+3)=='e' && *(it+4)=='n') { return 7; };
            } break;

            case 'e': {
                if(it < (upto-2) && *(it+1)=='i' && *(it+2)=='g' && *(it+3)=='h' && *(it+4)=='t') { return 8; };
            } break;

            case 'n': {
                if(it < (upto-1) && *(it+1)=='i' && *(it+2)=='n' && *(it+3)=='e') { return 9; };
            } break;
        }
    }

    if (value == -1) {
        std::cerr << "Unable to find the first digit." << std::endl;
    }

    return value;
}

int getLastDigit(std::string& str) {
    auto upto = str.rend();
    int value = -1;

    // find first occurance of a literal digit
    for (auto it = str.rbegin(); it < upto; it++) {
        if (std::isdigit(*it)) {
            upto = it;
            value = *it - '0';
            break;
        }
    }

    // find first occurance of a digit as an english word
    // (only going up to what we already found, or 3 before the end)
    upto = std::min(upto, str.rend()-3);

    for (auto it = str.rbegin(); it < upto; it++) {
        switch (*it) {
            case 'e': { // one, three, five, nine
                if(*(it+1)=='n' && *(it+2)=='o') { return 1; };
                if(it < (upto-1) && *(it+1)=='v' && *(it+2)=='i' && *(it+3)=='f') { return 5; };
                if(it < (upto-1) && *(it+1)=='n' && *(it+2)=='i' && *(it+3)=='n') { return 9; };
                if(it < (upto-2) && *(it+1)=='e' && *(it+2)=='r' && *(it+3)=='h' && *(it+4)=='t') { return 3; };
            } break;

            case 'o': { // two
                if(*(it+1)=='w' && *(it+2)=='t') { return 2; };
            } break;

            case 'r': { // four
                if (it < (upto-1) && *(it+1)=='u' && *(it+2)=='o' && *(it+3)=='f') { return 4; };
            } break;

            case 'x': { // six
                if(*(it+1)=='i' && *(it+2)=='s') { return 6; };
            } break;

            case 'n': { // seven
                if(it < (upto-2) && *(it+1)=='e' && *(it+2)=='v' && *(it+3)=='e' && *(it+4)=='s') { return 7; };
            } break;

            case 't': { // eight
                if(it < (upto-2) && *(it+1)=='h' && *(it+2)=='g' && *(it+3)=='i' && *(it+4)=='e') { return 8; };
            } break;
        }
    }

    if (value == -1) {
        std::cerr << "Unable to find the last digit." << std::endl;
    }

    return value;
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

