#!/bin/make
CC = g++

CFLAGS = `pkg-config --libs --cflags opencv4` -Wall -O3 -lpthread
CFLAGS+=$(shell python3-config --cflags)
LDFLAGS+=$(shell python3-config --ldflags)

INCLUDE = ../include
SRC = build.cpp neighboring_regions.cpp rois.cpp region_similarity.cpp use_python.cpp s_except.cpp model.cpp detect_objects.cpp

VAS_exec:
	@$(CC) $(SRC) -o $@ -I$(INCLUDE) $(CFLAGS) $(LDFLAGS)
	$(info To run use ./VAS_exec <media path> <number of processing threads>)
	
clean:
	@rm -f VAS_exec
