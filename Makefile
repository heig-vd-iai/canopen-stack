CXX ?= g++
OD_DIR ?= tests/golden/minimal/cm
CONFIG ?= examples/minimal.yaml

WARNINGS = -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion \
           -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual \
           -Wnull-dereference -Wdouble-promotion -Wformat=2 \
           -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast
CXXFLAGS = -std=c++14 -Isrc -I$(OD_DIR) -include common.hpp -O2 -fPIC $(WARNINGS)

LIB = canopen-slave.so
LIB_SRCS = $(wildcard src/*.cpp src/*/*.cpp)
LIB_OBJS = $(patsubst src/%.cpp,build/lib/%.o,$(LIB_SRCS))

# Smallest stack that answers on the bus: NMT, SDO and heartbeat.
MINIMAL_SRCS = src/node.cpp src/nmt.cpp src/frame.cpp src/heartbeat.cpp \
               src/od_common.cpp src/od/odAccessor.cpp \
               $(wildcard src/sdo/*.cpp)
MINIMAL_OD_DIR ?= tests/golden/bootloader/cm
MINIMAL_OBJS = $(patsubst src/%.cpp,build/minimal/%.o,$(MINIMAL_SRCS))
MINIMAL_OD_SRCS = $(wildcard $(MINIMAL_OD_DIR)/*.cpp)
MINIMAL_OD_OBJS = $(patsubst $(MINIMAL_OD_DIR)/%.cpp,build/minimal-od/%.o,$(MINIMAL_OD_SRCS))
MINIMAL_CXXFLAGS = -std=c++14 -Isrc -I$(MINIMAL_OD_DIR) -include common.hpp -O2 -fPIC $(WARNINGS)

MINIMAL_LIB = canopen-slave-minimal.so
MINIMAL_TEST_BIN = build/tests-minimal
MINIMAL_TEST_SRCS = $(wildcard tests/cpp/minimal/*.cpp)
MINIMAL_TEST_OBJS = $(patsubst tests/cpp/minimal/%.cpp,build/minimal-tests/%.o,$(MINIMAL_TEST_SRCS))

OD_SRCS = $(wildcard $(OD_DIR)/*.cpp)
OD_OBJS = $(patsubst $(OD_DIR)/%.cpp,build/od/%.o,$(OD_SRCS))

TEST_BIN = build/tests
TEST_SRCS = $(wildcard tests/cpp/*.cpp)
TEST_OBJS = $(patsubst tests/cpp/%.cpp,build/tests-obj/%.o,$(TEST_SRCS))

EXAMPLE_BIN = build/example
EXAMPLE_SRCS = example/linux/main.cpp $(wildcard platform/linux/*.cpp)
EXAMPLE_OBJS = $(patsubst %.cpp,build/example-obj/%.o,$(EXAMPLE_SRCS))

all: lib

lib: $(LIB)

$(LIB): $(LIB_OBJS)
	$(CXX) -fPIC -shared -o $@ $^

test: $(TEST_BIN)

lib-minimal: $(MINIMAL_LIB)

$(MINIMAL_LIB): $(MINIMAL_OBJS) $(MINIMAL_OD_OBJS)
	$(CXX) -fPIC -shared -o $@ $^

test-minimal: $(MINIMAL_TEST_BIN)

$(MINIMAL_TEST_BIN): $(MINIMAL_OBJS) $(MINIMAL_OD_OBJS) $(MINIMAL_TEST_OBJS)
	$(CXX) -o $@ $^

build/minimal/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(MINIMAL_CXXFLAGS) -MMD -MP -c -o $@ $<

build/minimal-od/%.o: $(MINIMAL_OD_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(MINIMAL_CXXFLAGS) -MMD -MP -c -o $@ $<

build/minimal-tests/%.o: tests/cpp/minimal/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(MINIMAL_CXXFLAGS) -Itests/cpp -MMD -MP -c -o $@ $<

$(TEST_BIN): $(LIB_OBJS) $(OD_OBJS) $(TEST_OBJS)
	$(CXX) -o $@ $^

build/lib/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

build/od/%.o: $(OD_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

build/tests-obj/%.o: tests/cpp/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Itests/cpp -MMD -MP -c -o $@ $<

example: $(EXAMPLE_BIN)

$(EXAMPLE_BIN): $(LIB_OBJS) $(OD_OBJS) $(EXAMPLE_OBJS)
	$(CXX) -o $@ $^

build/example-obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Iplatform/linux -MMD -MP -c -o $@ $<

generate:
	uv run python -m canopen_generator $(CONFIG) -f \
		--local dist/cm --remote dist/cpu1 --eds dist --doc dist/docs

FORMAT_SRCS = $(wildcard src/*.cpp src/*.hpp src/*/*.cpp src/*/*.hpp) \
              $(wildcard platform/*/*.cpp platform/*/*.hpp) \
              $(wildcard platform/c2000/example/*.cpp example/linux/*.cpp) \
              $(wildcard tests/cpp/*.cpp tests/cpp/*.hpp tests/cpp/minimal/*.cpp)
FORMAT_SRCS := $(filter-out tests/cpp/doctest.h,$(FORMAT_SRCS))

format:
	clang-format -i $(FORMAT_SRCS)

format-check:
	clang-format --dry-run -Werror $(FORMAT_SRCS)

clean:
	$(RM) -r build dist $(LIB) $(MINIMAL_LIB)

-include $(LIB_OBJS:.o=.d) $(OD_OBJS:.o=.d) $(TEST_OBJS:.o=.d) $(EXAMPLE_OBJS:.o=.d) \
         $(MINIMAL_OBJS:.o=.d) $(MINIMAL_OD_OBJS:.o=.d) $(MINIMAL_TEST_OBJS:.o=.d)

.PHONY: all lib lib-minimal test test-minimal example generate format \
        format-check clean
