#compiler and flags

CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic


#directories

BUILD_DIR = build

SRC_DIR = src

INCLUDE_DIR = include

DATA_DIR = data

EXECUTABLE = ReacKin2


#source files

SRCS = $(wildcard $(SRC_DIR)/*.cpp)


#object files

OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))


#include directories for the compiler

INC_DIRS = -I$(INCLUDE_DIR)


#default target: build the executable

all: $(BUILD_DIR)/$(EXECUTABLE)


#create the build directory if it doesn't exist

$(BUILD_DIR):

	mkdir -p $@


#compile source files into object files in the build directory

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)

	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@


#link object files to create executable

$(BUILD_DIR)/$(EXECUTABLE): $(OBJS)

	$(CXX) $(CXXFLAGS) $(OBJS) -o $@


#clean up

clean:

	rm -rf $(BUILD_DIR)


#phony targets

.PHONY: all clean
