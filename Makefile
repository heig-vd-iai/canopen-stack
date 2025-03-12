CCX?=g++
CXFLAGS=-std=c++20 -O3 -Wall -Werror -pedantic
EXEC=main
SRCS=$(wildcard src/*.cpp)
OBJS=$(patsubst src/%.cpp,build/%.o,$(SRCS))

all: $(EXEC)

-include $(OBJS:.o=.d)

$(EXEC): $(OBJS) | build
	$(CCX) -o $@ $^ $(LDXFLAGS)

build/%.o: src/%.cpp | build
	$(CCX) -o $@ -c $< $(CXFLAGS) -MMD -MP

clean:
	$(RM) -f $(OBJS) $(EXEC) $(OBJS:.o=.d)

build:
	mkdir -p build

.PHONY: all clean
