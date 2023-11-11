EXECUTABLE = app.out
CXX ?= g++
CXXFLAGS = --std=c++14 -g -Wall -Wextra -pedantic -pthread

SRC_DIR = ../src
EXAMPLE_DIR = ../example
BUILD_DIR = build
INCLUDES = -I$(SRC_DIR) -I$(EXAMPLE_DIR)
MAIN_SOURCE = example.cpp
LIB_SOURCES = $(shell find $(SRC_DIR) -type f -name "*.cpp")
SOURCES = ${LIB_SOURCES} ${MAIN_SOURCE}
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LIB_SOURCES)) $(BUILD_DIR)/${MAIN_SOURCE:.cpp=.o}

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/${MAIN_SOURCE:.cpp=.o}: ${MAIN_SOURCE} | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/objects

clean:
	$(RM) -rf $(BUILD_DIR) $(EXECUTABLE)

.PHONY: clean all