#!/bin/make

CC = g++
CFLAGS = `pkg-config --libs --cflags opencv4` -Wall
#INCLUDE = include

build:
	$(CC) build.cpp -o $@ $(CFLAGS)

.PHONEY: all build clean

clean:
	rm -f build
