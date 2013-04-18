CPPFLAGS = -std=c++0x
DEBUGFLAGS = -g3
OPTIMFLAGS = -O2

all: gmr
	g++ $(OPTIMFLAGS) $(CPPFLAGS) -o main main.cc Particle.o Gmr.o
.PHONY: all

main:
	g++ $(OPTIMFLAGS) $(CPPFLAGS) -o main main.cc Particle.o Gmr.o
.PHONY: main

gmr: particle
	g++ $(OPTIMFLAGS) $(CPPFLAGS) -c Gmr.cc -o Gmr.o
.PHONY: gmr

particle:
	g++ $(OPTIMFLAGS) $(CPPFLAGS) -c Particle.cc -o Particle.o
.PHONY: particle
