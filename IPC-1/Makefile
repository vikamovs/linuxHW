CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread

TARGET = signal-echo
OBJECTS = signal-echo.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

signal-echo.o: signal-echo.cpp
	$(CXX) $(CXXFLAGS) -c signal-echo.cpp

clean:
	rm -f $(TARGET) $(OBJECTS)
