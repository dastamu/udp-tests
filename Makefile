# Configuration variables
CXX      := g++
CXXFLAGS := -std=c++20 -O3 -Wall -Wextra
BUILD_DIR := build-make
TARGET   := $(BUILD_DIR)/udp1.exe
SRC      := src/udp1.cpp

# Default rule (triggered by typing just 'make')
all: $(TARGET)

# Executable build rule
$(TARGET): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Project directory cleanup rule
clean:
	rm $(TARGET)

# Declaration of goals that are not physical files
.PHONY: all clean
