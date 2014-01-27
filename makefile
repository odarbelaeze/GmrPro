FLAGS = -O3

main: GmrHeisenberg.o Accumulator.o main.o
	g++ -std=c++11 $(FLAGS) -o main GmrHeisenberg.o Accumulator.o main.o

main.o: main.cc helpers.h
	g++ -std=c++11 $(FLAGS) -c main.cc

GmrHeisenberg.o: GmrHeisenberg.cc GmrHeisenberg.h helpers.h
	g++ -std=c++11 $(FLAGS) -c GmrHeisenberg.cc

Accumulator.o: Accumulator.cc Accumulator.h
	g++ -std=c++11 $(FLAGS) -c Accumulator.cc

clear:
	rm *.o main
.PHONY: clear

run: main
	./main
.PHONY: run

data: main
	./main > data
.PHONY: data

debug: FLAGS = -g3
debug: main
	mv main profile
