LDFLAGS = -ljsoncpp
DEBUGFLAGS = -g3 -pg
RELEASEFLAGS = -O3
LIBS = lib/simplevector.o lib/particle.o lib/system.o

debug:
	g++ $(DEBUGFLAGS) -c simplevector.cc -o lib/simplevector.o
	g++ $(DEBUGFLAGS) -c particle.cc -o lib/particle.o
	g++ $(DEBUGFLAGS) -c system.cc -o lib/system.o
	g++ $(DEBUGFLAGS) -o bin/main main.cc $(LIBS) $(LDFLAGS)

release:
	g++ $(RELEASEFLAGS) -c simplevector.cc -o lib/simplevector.o
	g++ $(RELEASEFLAGS) -c particle.cc -o lib/particle.o
	g++ $(RELEASEFLAGS) -c system.cc -o lib/system.o
	g++ $(RELEASEFLAGS) -o bin/main main.cc $(LIBS) $(LDFLAGS)

test:
	g++ $(DEBUGFLAGS) -c simplevector.cc -o lib/simplevector.o
	g++ $(DEBUGFLAGS) -o bin/testsimplevector test/testsimplevector.cc $(LIBS)