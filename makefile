LDFLAGS = -ljsoncpp
DEBUGFLAGS = -g3 -ggdb -pg
RELEASEFLAGS = -O3
LIBS = lib/simplevector.o lib/particle.o lib/system.o

main: release
	g++ $(RELEASEFLAGS) -c src/gmrsystem.cc -o lib/gmrsystem.o
	g++ $(RELEASEFLAGS) -o bin/main src/main.cc lib/gmrsystem.o lib/libgmrpro.a $(LDFLAGS)

debug:
	g++ $(DEBUGFLAGS) -c src/simplevector.cc -o lib/simplevector.o
	g++ $(DEBUGFLAGS) -c src/particle.cc -o lib/particle.o
	g++ $(DEBUGFLAGS) -c src/system.cc -o lib/system.o
	ar -cqv lib/libgmrpro.a $(LIBS)

release:
	g++ $(RELEASEFLAGS) -c src/simplevector.cc -o lib/simplevector.o
	g++ $(RELEASEFLAGS) -c src/particle.cc -o lib/particle.o
	g++ $(RELEASEFLAGS) -c src/system.cc -o lib/system.o
	ar -cr lib/libgmrpro.a $(LIBS)

maindebug: debug
	g++ $(DEBUGFLAGS) -c src/gmrsystem.cc -o lib/gmrsystem.o
	g++ $(DEBUGFLAGS) -o bin/main src/main.cc lib/gmrsystem.o $(LIBS) $(LDFLAGS)