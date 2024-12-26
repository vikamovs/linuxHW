CXX = g++
CXXFLAGS = -Wall -std=c++17

all: first second

first: first.o
	$(CXX) $(CXXFLAGS) -o first first.o

second: second.o
	$(CXX) $(CXXFLAGS) -o second second.o

first.o: first.cpp shared_array.h
	$(CXX) $(CXXFLAGS) -c first.cpp

second.o: second.cpp shared_array.h
	$(CXX) $(CXXFLAGS) -c second.cpp

clean:
	rm -f *.o first second
