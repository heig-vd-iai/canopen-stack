CCX?=g++
CXFLAGS=-Idist/cm -Isrc -std=c++14 -include common.hpp -O3 -fPIC -Wextra -Wpedantic \
-Wconversion -Wsign-conversion -Wnon-virtual-dtor -Wold-style-cast \
-Woverloaded-virtual -Wnull-dereference -Wdouble-promotion -Wformat=2 \
-Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast

LDXFLAGS=
EXEC=canopen-slave.so
SRCS=$(wildcard src/*.cpp)
OBJS=$(patsubst src/%.cpp,build/%.o,$(SRCS))

all: doc $(EXEC)

-include $(OBJS:.o=.d)

canopen-slave.so: $(OBJS) | build
	$(CCX) -fPIC -shared -o $@ $^ $(LDXFLAGS)

build/%.o: src/%.cpp | build
	$(CCX) -o $@ -c $< $(CXFLAGS) -MMD -MP

build:
	mkdir -p build

doc:
	poetry install
	poetry run python -mcanopen_generator -f --all config-example.yaml

clean:
	$(RM) -f $(OBJS) $(EXEC) $(OBJS:.o=.d)
	$(RM) -r build dist

.PHONY: all clean doc
