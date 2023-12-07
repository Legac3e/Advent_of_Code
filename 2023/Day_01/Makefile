all: part1 part2 run

part1: part1.cpp
	g++ -O2 part1.cpp -o part1

part2: part2.cpp
	g++ -O2 part2.cpp -o part2

run: part1 part2
	@./part1 && ./part2

clean:
	rm -f part1 part2
