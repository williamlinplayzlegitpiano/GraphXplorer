CXX=g++
CXXFLAGS?=-Wall -pedantic -g -O0 -std=c++17
OUTFILES=GraphTest

all: $(OUTFILES)

GraphTest: GraphTest.cpp Graph.cpp Graph.h
	$(CXX) $(CXXFLAGS) -o GraphTest GraphTest.cpp Graph.cpp

gprof: GraphTest.cpp Graph.cpp Graph.h
	make clean
	$(CXX) $(CXXFLAGS) -pg -o GraphTest GraphTest.cpp Graph.cpp

clean:
	$(RM) $(OUTFILES) *.o
