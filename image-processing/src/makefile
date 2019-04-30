#!/bin/make

CC = g++
CFLAGS = `pkg-config --libs --cflags opencv4` -Wall
INCLUDE = ../include
NUMPY = /home/alenk/.local/lib/python3.7/site-packages/numpy/core/include
SRC = build.cpp neighboring_regions.cpp rois.cpp region_similarity.cpp
CFLAGS+=$(shell python3-config --cflags)
LDFLAGS+=$(shell python3-config --ldflags)

build: 
	@$(CC) $(SRC) -o $@ -I$(INCLUDE) -I$(NUMPY) $(CFLAGS)
	$(info To run use ./build <image path> <number of processing threads> <debug verbose t/f>)
	
clean:
	rm -f build
