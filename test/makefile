# Define constants
CC := gcc

# Data structures
DATA_STRUCTURES := ./event_simulation/data_structures/
HEAP_INCLUDE := ./../src/event_simulation/data_structures/
HEAP_SOURCE := $(HEAP_INCLUDE)heap.c

heap_test:
	$(CC) $(DATA_STRUCTURES)heap_test.c $(HEAP_SOURCE) -I. -I$(HEAP_INCLUDE) -o $(DATA_STRUCTURES)heap_test

build: heap_test

test: build
	$(DATA_STRUCTURES)heap_test