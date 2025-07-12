# Define constants
CC := gcc

# Data structures
DATA_STRUCTURES := ./event_simulation/data_structures/
heap_test:
	$(CC) $(DATA_STRUCTURES)heap_test.c -I. -o $(DATA_STRUCTURES)heap_test

build: heap_test

test: build
	$(DATA_STRUCTURES)heap_test