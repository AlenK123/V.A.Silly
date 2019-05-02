CC = g++
SRC = use_python.cpp
OBJ = run
INCLUDE_DIR = ../include
CFLAGS = `pkg-config --libs --cflags opencv4` -Wall
CFLAGS+=$(shell python3-config --cflags)
LDFLAGS+=$(shell python3-config --ldflags)

all:
	@$(CC) $(SRC) -I$(INCLUDE_DIR) $(CFLAGS) $(LDFLAGS) -w -o $(OBJ) -std=c++11
