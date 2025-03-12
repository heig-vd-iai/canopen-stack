CCX?=g++
CXFLAGS=-std=c++14 -O3 -fPIC #-Wall -Werror -pedantic
LDXFLAGS=
EXEC=canopen-slave.so
SRCS=$(wildcard src/*.cpp)
OBJS=$(patsubst src/%.cpp,build/%.o,$(SRCS))

all: $(EXEC)

-include $(OBJS:.o=.d)

canopen-slave.so: $(OBJS) | build
	$(CCX) -fPIC -shared -o $@ $^ $(LDXFLAGS)

build/%.o: src/%.cpp | build
	$(CCX) -o $@ -c $< $(CXFLAGS) -MMD -MP


build:
	mkdir -p build

doc:
	poetry install
	poetry run python -mcanopen_generator -f --all config.yaml

clean:
	$(RM) -f $(OBJS) $(EXEC) $(OBJS:.o=.d)
	$(RM) -r build dist

.PHONY: all clean doc
