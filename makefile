CC = g++
CPPFLAGS = -std=c++0x
DEBUGFLAGS = -g3
OPTIMFLAGS = -O3
OBJECTS = Helpers.o Particle.o System.o main.o

main: $(OBJECTS)
	$(CC) $(OPTIMFLAGS) $(CPPFLAGS) -o main $(OBJECTS)
.PHONY: main

%.o: %.cc
	$(CC) $(OPTIMFLAGS) $(CPPFLAGS) -c $<

run: all
	./main
.PHONY: run

clean:
	rm *.o main
.PHONY: clean
