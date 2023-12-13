CXX = g++
CXXFLAGS = -std=c++2a -O2 -Wall -Wextra -Wpedantic -Werror
#CXXFLAGS = -std=c++2a -ggdb -Wall -Wextra -Wpedantic -Werror -fsanitize=undefined,address

SRC = main.cpp
EXE = main

all: $(EXE) run

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: $(EXE)
	@./$(EXE)

clean:
	rm -f $(EXE)

.PHONY: all clean
