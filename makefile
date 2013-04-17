CPPFLAGS = -std=c++0x
DEBUGFLAGS = -g3
OPTIMFLAGS = -O2

all:
	g++ $(OPTIMFLAGS) $(CPPFLAGS) gmr.cc -o gmr

debug:
	g++ $(DEBUGFLAGS) $(OPTIMFLAGS) $(CPPFLAGS) gmr.cc -o gmr