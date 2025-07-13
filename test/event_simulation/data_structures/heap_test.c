#include "test.h"
#include "heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*  Define example heap structures. */
struct node {
    int value;
};

typedef struct node * node_t;

static node_t make_node(int value) {
    node_t new_node = malloc(sizeof(struct node));
    assert(new_node);
    new_node->value = value;
    return new_node;
}

static int read_node(node_t node) {
    return node->value;
}

void free_node(void *node, void *arg) {
    free(node);
}

comparison_t comparator(void *lhs, void *rhs, void *arg) {
    node_t node_l = (node_t) lhs;
    node_t node_r = (node_t) rhs; 

    if (node_l->value < node_r->value) {
        return LESS_THAN;
    } else if (node_l->value > node_r->value) {
        return GREATER_THAN;
    } else {
        return EQUAL_TO;
    }
}

DEFINE_TEST(heap_create_empty)
    binary_heap_t heap = create_empty_heap(comparator, free_node);
    ASSERT_EQ(binary_heap_size(heap), 0)
    free_heap(heap);
END_TEST

DEFINE_TEST(heap_length)
    binary_heap_t heap = create_empty_heap(comparator, free_node);

    node_t first_node = make_node(1);
    binary_heap_insert(heap, (void *) first_node);
    ASSERT_EQ(binary_heap_size(heap), 1)

    node_t second_node = make_node(2);
    binary_heap_insert(heap, (void *) second_node);
    ASSERT_EQ(binary_heap_size(heap), 2)

    node_t third_node = make_node(3);
    binary_heap_insert(heap, (void *) third_node);
    ASSERT_EQ(binary_heap_size(heap), 3)

    free_heap(heap);
END_TEST

DEFINE_TEST(heap_peek_min)
    binary_heap_t heap = create_empty_heap(comparator, free_node);

    node_t first_node = make_node(10);
    binary_heap_insert(heap, (void *) first_node);

    void * min = binary_heap_min(heap);
    ASSERT_EQ(read_node((node_t) min), 10)

    free_heap(heap);
END_TEST

DEFINE_TEST(heap_insert_1)
    binary_heap_t heap = create_empty_heap(comparator, free_node);

    node_t first_node = make_node(3);
    binary_heap_insert(heap, (void *) first_node);

    void * min = binary_heap_min(heap);
    ASSERT_EQ(read_node((node_t) min), 3)

    node_t second_node = make_node(1);
    binary_heap_insert(heap, (void *) second_node);

    min = binary_heap_min(heap);
    ASSERT_EQ(read_node((node_t) min), 1)

    node_t third_node = make_node(2);
    binary_heap_insert(heap, (void *) third_node);

    min = binary_heap_min(heap);
    ASSERT_EQ(read_node((node_t) min), 1)

    free_heap(heap);
END_TEST

DEFINE_TEST(heap_insert_2)
    binary_heap_t heap = create_empty_heap(comparator, free_node);

    node_t first_node = make_node(5);
    binary_heap_insert(heap, (void *) first_node);

    void * min = binary_heap_min(heap);
    ASSERT_EQ(read_node((node_t) min), 5)

    node_t second_node = make_node(4);
    binary_heap_insert(heap, (void *) second_node);

    min = binary_heap_min(heap);
    ASSERT_EQ(read_node((node_t) min), 4)

    node_t third_node = make_node(3);
    binary_heap_insert(heap, (void *) third_node);

    min = binary_heap_min(heap);
    ASSERT_EQ(read_node((node_t) min), 3)

    node_t fourth_node = make_node(2);
    binary_heap_insert(heap, (void *) fourth_node);

    min = binary_heap_min(heap);
    ASSERT_EQ(read_node((node_t) min), 2)

    node_t fifth_node = make_node(1);
    binary_heap_insert(heap, (void *) fifth_node);

    min = binary_heap_min(heap);
    ASSERT_EQ(read_node((node_t) min), 1)

    free_heap(heap);
END_TEST

DEFINE_TEST(heap_pop_1)
    binary_heap_t heap = create_empty_heap(comparator, free_node);

    node_t first_node = make_node(3);
    binary_heap_insert(heap, (void *) first_node);
    ASSERT_EQ(binary_heap_size(heap), 1)

    void * min = binary_heap_pop_min(heap);
    ASSERT_EQ(read_node((node_t) min), 3)
    ASSERT_EQ(binary_heap_size(heap), 0)

    free_heap(heap);   
END_TEST

DEFINE_TEST(heap_pop_2)
    binary_heap_t heap = create_empty_heap(comparator, free_node);

    node_t first_node = make_node(3);
    binary_heap_insert(heap, (void *) first_node);
    ASSERT_EQ(binary_heap_size(heap), 1)

    node_t second_node = make_node(1);
    binary_heap_insert(heap, (void *) second_node);
    ASSERT_EQ(binary_heap_size(heap), 2)

    void *min = binary_heap_pop_min(heap);
    ASSERT_EQ(read_node((node_t) min), 1)

    min = binary_heap_min(heap);
    ASSERT_EQ(read_node((node_t) min), 3)

    free_heap(heap);   
END_TEST

REGISTER_TESTS(
    heap_create_empty,
    heap_length,
    heap_peek_min,
    heap_insert_1,
    heap_insert_2,
    heap_pop_1,
    heap_pop_2
)