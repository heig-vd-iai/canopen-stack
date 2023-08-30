EXECUTABLE = app.out
CXX = g++
CXXFLAGS = --std=c++14
CXXFLAGS += -g
CXXFLAGS += -Wall
# CXXFLAGS += -Wextra
# CXXFLAGS += -pedantic
CXXFLAGS += -pthread

SRC_DIR = ../src
EXAMPLE_DIR = ../example
BUILD_DIR = build
INCLUDES = -I$(SRC_DIR) -I$(EXAMPLE_DIR)
MAIN_SOURCE = example.cpp
LIB_SOURCES = $(shell find $(SRC_DIR) -type f -name "*.cpp")
SOURCES = ${LIB_SOURCES} ${MAIN_SOURCE}
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LIB_SOURCES)) $(BUILD_DIR)/${MAIN_SOURCE:.cpp=.o}

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/${MAIN_SOURCE:.cpp=.o}: ${MAIN_SOURCE}
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	-rm -rf $(BUILD_DIR) $(EXECUTABLE)
