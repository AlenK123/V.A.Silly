#!/bin/make

CC = g++
CFLAGS = `pkg-config --libs --cflags opencv4` -Wall
INCLUDE = ../include
SRC = build.cpp neighboring_regions.cpp rois.cpp region_similarity.cpp

build: 
	$(CC) $(SRC) -o $@ -I$(INCLUDE) $(CFLAGS)

.PHONEY: all build clean

clean:
	rm -f build
