/*  Heap.c

    Implementation for generic binary min heap used in priority queue.

    Max heaps can be implemented with this as well by simply inverting the
    output of the comparator function supplied at creation time.
    
    The idea here is to represent the heap as a black box for the rest of the
    codebase due to it's slightly complex and unintuitive internal logic
    (particularly in C). Hence the structures are defined in this file,
    although declared in heap.h. The rest of the codebase should use the type
    alias binary_heap_t for simplicity's sake however. */

#include "heap.h"

#include <assert.h>
#include <stddef.h>
#include <malloc.h>

/*  Constant definitions. */
#define DEFAULT_ARR_CAPACITY 16

/*  Forward declarations of helper functions. */
static inline int binary_heap_get_parent_index(int index);
static inline int binary_heap_get_left_child_index(int index);
static inline int binary_heap_get_right_child_index(int index);
static void binary_heap_bubble_last_element(binary_heap_t heap);

/*  Heap structure - this implementation uses the traditional array method,
    where a dynamic / resizing array is used to store the elements in order
    from top to bottom, left to right. E.g.
    
          A
         / \
        B   C
       / \  
      D   E
      
    Would be represented by the array [A, B, C, D, E] and vice-versa.
    
    Recall that a binary heap is always a "complete" binary tree and therefore
    for each subtree, the left side may be at most 1 element deeper than the
    right but otherwise the two must be balanced. */
struct binary_heap {
    /*  Elements are void* so as to be able to be used for any element type.
        We store an array of void pointers to represent the underlying array of
        elements that represent the heap. */
    void ** elems;

    /*  Heap size - total number of elements. */
    unsigned int size;

    /*  Capacity - total size of the array in terms of numbers of void
        pointers, i.e. array elements. */
    unsigned int capacity;

    /*  Comparator function used to determine the position of an element in
        the heap. Simply compares two generic elements and returns less than,
        greater than or equal to. Must be provided at creation time. */
    func_comparator_t comparator;

    /*  Free function that frees a single element. Used in the free_heap
        implementation. */
    func_free_t free_elem;
};

/*  Heap API implementation. */

/*  Initialising a heap is simple - just malloc space for the heap structure
    and the internal array, then set it's data members, using the supplied
    functions for the comparator and free element functions. */
binary_heap_t create_empty_heap(func_comparator_t comparator, func_free_t free_elem) {
    /*  Assert that functions were not NULL pointers. */
    assert(comparator);
    assert(free_elem);

    /*  Allocate memory for heap structure. */
    binary_heap_t new_heap = malloc(sizeof(struct binary_heap));
    assert(new_heap);

    /*  Allocate space for containing array. Use default capacity initially. */
    new_heap->elems = malloc(sizeof(void *) * DEFAULT_ARR_CAPACITY);
    assert(new_heap->elems);

    /*  Set initial heap properties. */
    new_heap->size = 0;
    new_heap->capacity = DEFAULT_ARR_CAPACITY;
    new_heap->comparator = comparator;
    new_heap->free_elem = free_elem;

    return new_heap;
};

/*  Freeing a heap simply involves iterating over all elements in the
    underlying array and freeing them with the function supplied at creation
    time, then proceeding to free the array itself and finally the heap
    structure. */
void free_heap(binary_heap_t heap) {
    /*  Assert that... */
    assert(heap);            /* ...heap exists. */
    assert(heap->free_elem); /* ...free function exists. */
    assert(heap->elems);     /* ...implementing array exists. */

    /*  Iterate through all elements. */
    int i;
    for (i = 0; i < heap->size; i++) {
        /*  Assert that element exists. */
        assert(heap->elems[i]);
        heap->free_elem(heap->elems[i]);
    }

    /*  Free underlying array. */
    free(heap->elems);

    /*  Free structure. */
    free(heap);
};

/*  Return the size field - hidden from outside the file. */
unsigned int binary_heap_size(binary_heap_t heap) {
    return heap->size;
};

/*  Inserting an element into a heap involves first adding the element to the
    next space in the existing complete binary tree (top to bottom, left to
    right) and then bubbling it up until it is in the correct place. This is
    O(log n) in the number of heap elements because any leaf node on a complete
    binary tree has at most a height of log n. The worst case scenario is where
    the node bubbles up to the top and then down another branch completely,
    taking 2 log n, as it must bubble up to the top (log n) and then back down
    again (another log n). */
int binary_heap_insert(binary_heap_t heap, void *elem) {
    assert(heap);
    assert(heap->elems);
    assert(heap->size <= heap->capacity);

    /*  Check if the array needs to be resized. */
    if (heap->size == heap->capacity) {
        heap->capacity = 2 * heap->capacity;
        heap->elems = realloc(heap->elems, sizeof(void *) * heap->capacity);
    }

    /*  Insert new element at size + 1. */
    heap->elems[heap->size] = elem;
    heap->size = heap->size + 1;

    /*  Bubble up to correct place. */
    binary_heap_bubble_last_element(heap);

    return 1;
};

/*  Return the top element. */
void * binary_heap_min(binary_heap_t heap) {
    if (heap->size == 0) {
        return NULL;
    } else {
        return heap->elems[0];
    }
};

/*  Remove the top element from the heap. This works by first extracting the
    element and saving it to a temporary variable to return. Then we swap it
    with the element in the last location in the array in order to preserve the
    complete binary tree property. We then try to swap it with one of its
    children until it is smaller than both children. */
void * binary_heap_pop_min(binary_heap_t heap) {
    void *min_val = binary_heap_min(heap);

    if (min_val == NULL) {
        return NULL;
    }

    heap->elems[0] = heap->elems[heap->size - 1];
    heap->elems[heap->size - 1] = 0;

    heap->size = heap->size - 1;

    int index = 0;
    int left_index = binary_heap_get_left_child_index(index);
    int right_index = binary_heap_get_right_child_index(index);
    int smaller_than_children = 1;

    /*  Iteratively swap moved element with children until its children are
        greater than it. */
    while (
        (left_index < heap->size || right_index < heap->size) &&
        smaller_than_children
    ) {
        int left_present;
        comparison_t left_compare;

        int right_present;
        comparison_t right_compare;

        /*  Check if left and right children are present. */
        left_present = left_index < heap->size;
        right_present = right_index < heap->size;

        /*  Compare current and left child elements. */
        if (left_present) {
            left_compare =
                heap->comparator(heap->elems[index], heap->elems[left_index]);
        }

        /*  Compare current and right child elements. */
        if (right_present) {
            right_compare =
                heap->comparator(heap->elems[index], heap->elems[right_index]);
        }

        /*  Initialise swap flags to 0 - we swap the element and the given
            direction child node if these are set to 1 at the end of the
            iteration. */
        int swap_left = 0;
        int swap_right = 0;

        /*  Bubble element down. */
        if (left_present && right_present) {
            /*  When both children are present, we compare the current element
                with both children to confirm that it is less then both, then
                we choose the smaller child element to swap. */
            if (left_compare == GREATER_THAN && right_compare == GREATER_THAN) {
                /*  Choose lesser of left and right to swap with. */
                comparison_t child_compare = heap->comparator(
                    heap->elems[left_index],
                    heap->elems[right_index]
                );

                /*  Swap in direction of smaller child. */
                if (child_compare == LESS_THAN || child_compare == EQUAL_TO) {
                    swap_left = 1;
                } else {
                    swap_right = 1;
                }
            } else if (left_compare == GREATER_THAN) {
                /*  Swap with left. */
                swap_left = 1;
            } else if (right_compare == GREATER_THAN) {
                /*  Swap with right. */
                swap_right = 1;
            }
        } else if(left_present && left_compare == GREATER_THAN) {
            /*  Left node is only child and current element is greater than it
                - swap. */
            swap_left = 1;
        } else if(right_present && right_compare == GREATER_THAN){
            /*  Right node is only child and current element is greater than it
                - swap. */
            swap_right = 1;
        }

        /*  Swap based on decided direction. */
        if (swap_left) {
            /*  Swap with left child. */
            void *temp = heap->elems[index];
            heap->elems[index] = heap->elems[left_index];
            heap->elems[left_index] = temp;

            /*  Update index. */
            index = left_index; 
        } else if(swap_right) {
            /*  Swap with right child. */
            void *temp = heap->elems[index];
            heap->elems[index] = heap->elems[right_index];
            heap->elems[right_index] = temp;

            /*  Update index. */
            index = right_index; 
        } else {
            /*  Done */
            smaller_than_children = 0;
        }

        left_index = binary_heap_get_left_child_index(index);
        right_index = binary_heap_get_right_child_index(index);
    }

    return min_val;
};

/*  Helper functions. */

/*  The parent element index in the array is given by (index - 1) div 2. We
    will now prove this. We start with the following definitions:
    
    Layer and index: a node in the heap is said to have a layer k and an index
    j. These are 0-based and correspond to the geometric interpretation you
    would expect.

    Child: a node at layer k and index j has children at layer k + 1 with
    indices 2j and 2j + 1. This node is the parent of its children. A simple
    lemma of this is that if j has binary representation b then the children
    have indices with binary representations b0 and b1. This follows from
    binary arithmetic.

    Heap array property: a node at index i in the array has i + 1 with the
    binary representation 0b1<bstr> where <bstr> has length n. The layer of
    node i is given by n and the index is given by <bstr>.

    Under these assumptions we can prove as follows:
        1)  Consider an arbitrary element i of the array with layer k and index
            j.

        2)  Suppose element i has a parent p with layer x and index y. Then
            x = k - 1 and j = 2y or j = 2y + 1 from (1). Let y have binary
            representation r then j has binary representation r0 or r1 by
            binary arithmetic.

        3)  From the heap array property and (2), p + 1 has binary
            representation 1r.
        
        4)  From the heap array property, (1) and (2), i + 1 has binary
            representation 1r0 or 1r1.

        5)  From (3), (4) and binary arithmetic i + 1 = (p + 1) * 2 or
            i + 1 = (p + 1) * 2 + 1.
        
        6)  From (5), i - 1 = (p + 1) * 2 - 2 or i - 1 = (p + 1) * 2 - 1.
            Simplifying gives i - 1 = 2p or i - 1 = 2p + 1. Observe that these
            two numbers have the same binary representation except for the
            least significant bit which is 0 in the former case and 1 in the
            latter.
        
        7) From (6) and the definition of div, (i - 1) div 2 = p as required. 

        QED. */
static inline int binary_heap_get_parent_index(int index) {
    return (index - 1) / 2;    
}

/*  The functions for getting the left and right child follow from the above 
    proof quite simply. */
static inline int binary_heap_get_left_child_index(int index) {
    return 2 * index + 1;
}

static inline int binary_heap_get_right_child_index(int index) {
    return 2 * index + 2;
}

/*  Precondition: apart from the last element, according to the order of a
    complete binary tree, the elements form a valid heap.
    
    Bubble:
        1)  Compare the element to its parent if present, otherwise end.
        
        2)  If it is smaller then swap their position and go back to (1).
            Otherwise end.           
        
        Note: the parent is guaranteed to be smaller than or equal to all
        elements below it so the order is preserved and there is no need
        to check the other child of the swapped out parent. */
static void binary_heap_bubble_last_element(binary_heap_t heap) {
    /*  Use int because heap_get_parent_index returns -1 when no parent
        exists. */
    int elem_index = heap->size - 1;
    int parent_index = binary_heap_get_parent_index(elem_index);
    assert(elem_index == 0 || parent_index != -1);

    while (
        elem_index != 0 &&
        heap->comparator(
            heap->elems[elem_index],
            heap->elems[parent_index]
        ) == LESS_THAN
    ) {
        /*  Swap element and parent. */
        void *temp = heap->elems[elem_index];
        heap->elems[elem_index] = heap->elems[parent_index];
        heap->elems[parent_index] = temp;

        /*  Update indices. */
        elem_index = parent_index;
        parent_index = binary_heap_get_parent_index(elem_index);
        assert(elem_index == 0 || parent_index != -1);
    }
};