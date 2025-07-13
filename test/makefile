# Define constants
CC := gcc

# Data structures
DATA_STRUCTURES := ./event_simulation/data_structures/
HEAP_INCLUDE := -I./../src/event_simulation/data_structures/ -I.
HEAP_SOURCE := ./../src/event_simulation/data_structures/heap.c

heap_test:
	$(CC) $(DATA_STRUCTURES)heap_test.c $(HEAP_SOURCE) $(HEAP_INCLUDE) -o $(DATA_STRUCTURES)heap_test

# Event queue
EVENT_QUEUE := ./event_simulation/
EVENT_QUEUE_INCLUDE := -I./../src/event_simulation/
EVENT_QUEUE_SRC := ./../src/event_simulation/event_queue.c

event_queue_test:
	$(CC) $(EVENT_QUEUE)queue_test_uint.c $(EVENT_QUEUE_SRC) $(HEAP_SOURCE) $(EVENT_QUEUE_INCLUDE) $(HEAP_INCLUDE) -o $(EVENT_QUEUE)queue_test_uint
	$(CC) $(EVENT_QUEUE)queue_test_double.c $(EVENT_QUEUE_SRC) $(HEAP_SOURCE) $(EVENT_QUEUE_INCLUDE) $(HEAP_INCLUDE) -o $(EVENT_QUEUE)queue_test_double

build: heap_test event_queue_test

test: build
	$(DATA_STRUCTURES)heap_test
	$(EVENT_QUEUE)queue_test_uint
	$(EVENT_QUEUE)queue_test_double