CC = g++
SRC = use_python.cpp
OBJ = run
INCLUDE_DIR = ../include
NUMPY = /home/alenk/.local/lib/python3.7/site-packages/numpy/core/include
CFLAGS = `pkg-config --libs --cflags opencv4` -Wall
CFLAGS+=$(shell python3-config --cflags)
LDFLAGS+=$(shell python3-config --ldflags)

all:
	@$(CC) $(SRC) -I$(INCLUDE_DIR) -I$(NUMPY) $(CFLAGS) $(LDFLAGS) -w -o $(OBJ) -std=c++11
