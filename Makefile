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

OD_SRCS = $(wildcard $(OD_DIR)/*.cpp)
OD_OBJS = $(patsubst $(OD_DIR)/%.cpp,build/od/%.o,$(OD_SRCS))

TEST_BIN = build/tests
TEST_SRCS = $(wildcard tests/cpp/*.cpp)
TEST_OBJS = $(patsubst tests/cpp/%.cpp,build/tests-obj/%.o,$(TEST_SRCS))

all: lib

lib: $(LIB)

$(LIB): $(LIB_OBJS)
	$(CXX) -fPIC -shared -o $@ $^

test: $(TEST_BIN)

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

generate:
	uv run python -m canopen_generator $(CONFIG) -f \
		--local dist/cm --remote dist/cpu1 --eds dist --doc dist/docs

FORMAT_SRCS = $(wildcard src/*.cpp src/*.hpp src/*/*.cpp src/*/*.hpp) \
              $(wildcard tests/cpp/*.cpp tests/cpp/*.hpp)
FORMAT_SRCS := $(filter-out tests/cpp/doctest.h,$(FORMAT_SRCS))

format:
	clang-format -i $(FORMAT_SRCS)

format-check:
	clang-format --dry-run -Werror $(FORMAT_SRCS)

clean:
	$(RM) -r build dist $(LIB)

-include $(LIB_OBJS:.o=.d) $(OD_OBJS:.o=.d) $(TEST_OBJS:.o=.d)

.PHONY: all lib test generate format format-check clean
