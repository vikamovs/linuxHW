CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = interactive-shell
SOURCES = shell.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
