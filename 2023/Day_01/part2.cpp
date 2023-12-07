// https://adventofcode.com/2023/day/1

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

const char* INPUT_FILE = "input.txt";

struct calibrated_values {
    char first;
    char last;
};

int main() {
    std::ifstream inputfile(INPUT_FILE);

    if (!inputfile.good()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    int sum = 0;
    std::string line;
    while (std::getline(inputfile, line)) {
        calibrated_values vals {0, 0};

        for (size_t i = 0; i < line.size(); i++) {
            switch (std::tolower(line[i])) {
                case 'o': { //3
                    if ((i+2) < line.size() && line[i+1] == 'n' && line[i+2] == 'e') {
                        vals.last = '1';
                        if (!vals.first) {
                            vals.first = '1';
                        }
                    }
                } break;

                case 't': {// 3, 5
                    if ((i+2) < line.size() && line[i+1] == 'w' && line[i+2] == 'o') {
                        vals.last = '2';
                        if (!vals.first) {
                            vals.first = '2';
                        }
                    }

                    if ((i+4) < line.size() && line[i+1] == 'h' && line[i+2] == 'r' && line[i+3] == 'e' && line[i+4] == 'e') {
                        vals.last = '3';
                        if (!vals.first) {
                            vals.first = '3';
                        }
                    }
                } break;

                case 'f': { //4
                    if ((i+3) < line.size() && line[i+1] == 'o' && line[i+2] == 'u' && line[i+3] == 'r') {
                        vals.last = '4';
                        if (!vals.first) {
                            vals.first = '4';
                        }
                    }

                    if ((i+3) < line.size() && line[i+1] == 'i' && line[i+2] == 'v' && line[i+3] == 'e') {
                        vals.last = '5';
                        if (!vals.first) {
                            vals.first = '5';
                        }
                    }
                } break;

                case 's':{ //3
                    if ((i+2) < line.size() && line[i+1] == 'i' && line[i+2] == 'x') {
                        vals.last = '6';
                        if (!vals.first) {
                            vals.first = '6';
                        }
                    }

                    if ((i+4) < line.size() && line[i+1] == 'e' && line[i+2] == 'v' && line[i+3] == 'e' && line[i+4] == 'n') {
                        vals.last = '7';
                        if (!vals.first) {
                            vals.first = '7';
                        }
                    }
                } break;

                case 'e': {//5
                    if ((i+4) < line.size() && line[i+1] == 'i' && line[i+2] == 'g' && line[i+3] == 'h' && line[i+4] == 't') {
                        vals.last = '8';
                        if (!vals.first) {
                            vals.first = '8';
                        }
                    }
                } break;

                case 'n': {//4
                    if ((i+3) < line.size() && line[i+1] == 'i' && line[i+2] == 'n' && line[i+3] == 'e') {
                        vals.last = '9';
                        if (!vals.first) {
                            vals.first = '9';
                        }
                    }
                } break;

                /*
                case 'z': {//4
                    if ((i+4) < line.size() && line[i+1] == 'e' && line[i+2] == 'r' && line[i+3] == 'o') {
                        vals.last = '0';
                        if (!vals.first) {
                            vals.first = '0';
                        }
                    }
                } break;
                */

                default: {
                    if (std::isdigit(line[i])) {
                        vals.last = line[i];
                        if (!vals.first) {
                            vals.first = line[i];
                        }
                    }
                } break;
            }
        }

        sum += (10*(vals.first - '0')) + (vals.last - '0');
    }

    std::cout << sum << std::endl;

    inputfile.close();
}
