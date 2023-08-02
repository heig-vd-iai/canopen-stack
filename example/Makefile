APP_NAME = app
CANOPEN_SRC = ../src
OBJECTS_SRC = $(CANOPEN_SRC)/objects
APP_SRC = .


INCLUDE_DIRS = \
	-I$(CANOPEN_SRC) \
	-I$(APP_SRC)


SOURCES = \
	$(APP_SRC)/example.cpp \
	$(CANOPEN_SRC)/od.cpp \
	$(CANOPEN_SRC)/node.cpp \
	$(CANOPEN_SRC)/nmt.cpp \
	$(CANOPEN_SRC)/heartbeat.cpp \
	$(CANOPEN_SRC)/sdo.cpp \
	$(CANOPEN_SRC)/pdo.cpp \
	$(CANOPEN_SRC)/sync.cpp \
	$(CANOPEN_SRC)/time.cpp \
	$(CANOPEN_SRC)/emergency.cpp \
	$(OBJECTS_SRC)/object_1A00.cpp \
	$(OBJECTS_SRC)/object_1800.cpp


CC = g++
FLAGS = --std=c++14
FLAGS += -Wall
# FLAGS += -Wextra
# FLAGS += -pedantic
FLAGS += -pthread


$(APP_NAME):
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(SOURCES) -o $@.out
