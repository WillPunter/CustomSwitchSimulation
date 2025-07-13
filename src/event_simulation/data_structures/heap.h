/*  Heap.h
    
    Generic binary heap implementation. At present, this is simply used to
    implement the priority queue for the discrete event scheduler. Therefore
    it's API does not necessarily include the full suite of functions typically
    written as part of a traditional data structures and algorithms library. */

#ifndef BINHEAP_H
#define BINHEAP_H

struct binary_heap;

typedef struct binary_heap * binary_heap_t;

enum comparator_val {
    LESS_THAN,
    GREATER_THAN,
    EQUAL_TO
};

typedef enum comparator_val comparison_t;

/*  A comparator function compares two elements and produces a comparison value
    as an output. The first two parameters are the left and right elements
    and the last represents a generic argument for any external state that may
    be used in a comparison. */
typedef comparison_t (*func_comparator_t)(void *, void *, void *);

/*  A free function takes an element to free and an additional argument value
    which may be NULL. */
typedef void (*func_free_t)(void *, void *);

binary_heap_t create_empty_heap(func_comparator_t comparator, func_free_t free_elem);
void free_heap(binary_heap_t heap);
unsigned int binary_heap_size(binary_heap_t heap);
int binary_heap_insert(binary_heap_t heap, void *elem);
void * binary_heap_min(binary_heap_t heap);
void * binary_heap_pop_min(binary_heap_t heap);
void binary_heap_set_comparator_arg(binary_heap_t heap, void * arg);
void binary_heap_set_free_arg(binary_heap_t heap, void * arg);

#endif