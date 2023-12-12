CXX = g++
CXXFLAGS = -std=c++2a -O2 -Wall -Wextra -Wpedantic -Werror
#CXXFLAGS = -std=c++2a -ggdb -Wall -Wextra -Wpedantic -Werror -fsanitize=undefined,address

SRC1 = part1.cpp
SRC2 = part2.cpp

EXE1 = part1
EXE2 = part2

all: $(EXE1) $(EXE2) run

$(EXE1): $(SRC1)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXE2): $(SRC2)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: $(EXE1) $(EXE2)
	@./$(EXE1) && ./$(EXE2)

clean:
	rm -f $(EXE1) $(EXE2)

.PHONY: all clean
