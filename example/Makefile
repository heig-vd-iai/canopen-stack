APP_NAME = app
CANOPEN_SRC = ../src/
APP_SRC = .


INCLUDE_DIRS = \
	-I$(CANOPEN_SRC) \
	-I$(APP_SRC)


SOURCES = \
	$(APP_SRC)/test.cpp \
	$(CANOPEN_SRC)od.cpp \


CC = g++
FLAGS = -std=c++11 -Wall -g $(INCLUDE_DIRS)


$(APP_NAME):
	$(CC) $(FLAGS) $(SOURCES) -o $@.out
