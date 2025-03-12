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

clean:
	$(RM) -f $(OBJS) $(EXEC) $(OBJS:.o=.d)

build:
	mkdir -p build

.PHONY: all clean
