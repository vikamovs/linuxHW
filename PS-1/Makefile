CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = do-command
SOURCES = do-command.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
