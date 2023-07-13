APP_NAME = app
CANOPEN_SRC = ../src/
APP_SRC = .


INCLUDE_DIRS = \
	-I$(CANOPEN_SRC) \
	-I$(APP_SRC)


SOURCES = \
	$(APP_SRC)/test.cpp \
	$(CANOPEN_SRC)/od.cpp \
	$(CANOPEN_SRC)/node.cpp \
	$(CANOPEN_SRC)/nmt.cpp


CC = g++
FLAGS = -std=c++11 -Wall -g
FLAGS += -pthread


$(APP_NAME):
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(SOURCES) -o $@.out
