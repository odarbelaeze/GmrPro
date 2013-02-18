LDFLAGS = -ljsoncpp
DEBUGFLAGS = -g3 -pg
RELEASEFLAGS = -O3
LIBS = lib/simplevector.o lib/particle.o lib/system.o

debug:
	g++ $(DEBUGFLAGS) -c src/simplevector.cc -o lib/simplevector.o
	g++ $(DEBUGFLAGS) -c src/particle.cc -o lib/particle.o
	g++ $(DEBUGFLAGS) -c src/system.cc -o lib/system.o

release:
	g++ $(RELEASEFLAGS) -c src/simplevector.cc -o lib/simplevector.o
	g++ $(RELEASEFLAGS) -c src/particle.cc -o lib/particle.o
	g++ $(RELEASEFLAGS) -c src/system.cc -o lib/system.o