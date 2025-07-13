#include "test.h"
#include "event_queue.h"

#include <stdlib.h>
#include <stdio.h>

struct data_elem {
    int data;
};

typedef struct data_elem * data_elem_t;

static void free_data_elem(void *data_elem, void *arg) {
    free(data_elem);
}

static data_elem_t create_data_elem(int val) {
    data_elem_t data = malloc(sizeof(struct data_elem));
    data->data = val;
    return data;
}

DEFINE_TEST(queue_uint_create_and_destroy)
    event_queue_t queue = create_queue_uint_time(
        free_data_elem,
        NULL
    );

    ASSERT_EQ(event_queue_size(queue), 0);

    free_event_queue(queue);
END_TEST

DEFINE_TEST(queue_uint_enqueue_1)
    event_queue_t queue = create_queue_uint_time(
        free_data_elem,
        NULL
    );

    data_elem_t elem_1 = create_data_elem(10);
    event_queue_enqueue_uint_time(queue, (void *) elem_1, 15);

    void * elem_1_peek;
    unsigned int elem_1_time = event_queue_peek_uint_time(queue, &elem_1_peek);
    elem_1 = (data_elem_t) elem_1_peek;

    ASSERT_EQ(elem_1->data, 10)
    ASSERT_EQ(elem_1_time, 15)

    free_event_queue(queue);
END_TEST

DEFINE_TEST(queue_uint_enqueue_2)
    event_queue_t queue = create_queue_uint_time(
        free_data_elem,
        NULL
    );

    data_elem_t elem_1 = create_data_elem(10);
    event_queue_enqueue_uint_time(queue, (void *) elem_1, 15);

    void * elem_1_peek;
    unsigned int elem_1_time = event_queue_peek_uint_time(queue, &elem_1_peek);
    elem_1 = (data_elem_t) elem_1_peek;

    ASSERT_EQ(elem_1->data, 10)
    ASSERT_EQ(elem_1_time, 15)


    data_elem_t elem_2 = create_data_elem(5);
    event_queue_enqueue_uint_time(queue, (void *) elem_2, 10);

    void * elem_2_peek;
    unsigned int elem_2_time = event_queue_peek_uint_time(queue, &elem_2_peek);
    elem_2 = (data_elem_t) elem_2_peek;

    ASSERT_EQ(elem_2->data, 5)
    ASSERT_EQ(elem_2_time, 10)

    
    data_elem_t elem_3 = create_data_elem(2);
    event_queue_enqueue_uint_time(queue, (void *) elem_3, 20);

    void * elem_3_peek;
    unsigned int elem_3_time = event_queue_peek_uint_time(queue, &elem_3_peek);
    elem_3 = (data_elem_t) elem_3_peek;

    ASSERT_EQ(elem_3->data, 5)
    ASSERT_EQ(elem_3_time, 10)

    free_event_queue(queue);
END_TEST

DEFINE_TEST(queue_uint_size_1)
    event_queue_t queue = create_queue_uint_time(
        free_data_elem,
        NULL
    );

    ASSERT_EQ(event_queue_size(queue), 0)

    data_elem_t elem_1 = create_data_elem(10);
    event_queue_enqueue_uint_time(queue, (void *) elem_1, 15);
    ASSERT_EQ(event_queue_size(queue), 1)

    data_elem_t elem_2 = create_data_elem(5);
    event_queue_enqueue_uint_time(queue, (void *) elem_2, 10);
    ASSERT_EQ(event_queue_size(queue), 2)

    data_elem_t elem_3 = create_data_elem(2);
    event_queue_enqueue_uint_time(queue, (void *) elem_3, 20);
    ASSERT_EQ(event_queue_size(queue), 3)

    free_event_queue(queue);
END_TEST

DEFINE_TEST(queue_uint_dequeue_1)
    event_queue_t queue = create_queue_uint_time(
        free_data_elem,
        NULL
    );

    ASSERT_EQ(event_queue_size(queue), 0)

    data_elem_t elem_1 = create_data_elem(10);
    event_queue_enqueue_uint_time(queue, (void *) elem_1, 15);
    ASSERT_EQ(event_queue_size(queue), 1)

    void * elem_1_dequeue;
    unsigned int elem_1_time =
        event_queue_dequeue_uint_time(queue, &elem_1_dequeue);
    elem_1 = (data_elem_t) elem_1_dequeue;
    ASSERT_EQ(elem_1->data, 10);
    free_data_elem(elem_1_dequeue, NULL);

    ASSERT_EQ(event_queue_size(queue), 0)

    free_event_queue(queue);
END_TEST

DEFINE_TEST(queue_uint_dequeue_2)
    event_queue_t queue = create_queue_uint_time(
        free_data_elem,
        NULL
    );

    ASSERT_EQ(event_queue_size(queue), 0)

    data_elem_t elem_1 = create_data_elem(1);
    event_queue_enqueue_uint_time(queue, (void *) elem_1, 20);
    ASSERT_EQ(event_queue_size(queue), 1)

    data_elem_t elem_2 = create_data_elem(2);
    event_queue_enqueue_uint_time(queue, (void *) elem_2, 30);
    ASSERT_EQ(event_queue_size(queue), 2)

    data_elem_t elem_3 = create_data_elem(3);
    event_queue_enqueue_uint_time(queue, (void *) elem_3, 10);
    ASSERT_EQ(event_queue_size(queue), 3)

    data_elem_t elem_4 = create_data_elem(4);
    event_queue_enqueue_uint_time(queue, (void *) elem_4, 40);
    ASSERT_EQ(event_queue_size(queue), 4)


    void *dequeue_1_ptr;
    unsigned int dequeue_1_time =
        event_queue_dequeue_uint_time(queue, &dequeue_1_ptr);
    data_elem_t dequeue_1 = (data_elem_t) dequeue_1_ptr;
    ASSERT_EQ(dequeue_1->data, 3)
    ASSERT_EQ(dequeue_1_time, 10)
    free_data_elem(dequeue_1_ptr, NULL);

    void *dequeue_2_ptr;
    unsigned int dequeue_2_time =
        event_queue_dequeue_uint_time(queue, &dequeue_2_ptr);
    data_elem_t dequeue_2 = (data_elem_t) dequeue_2_ptr;
    ASSERT_EQ(dequeue_2->data, 1)
    ASSERT_EQ(dequeue_2_time, 20)
    free_data_elem(dequeue_2_ptr, NULL);

    void *dequeue_3_ptr;
    unsigned int dequeue_3_time =
        event_queue_dequeue_uint_time(queue, &dequeue_3_ptr);
    data_elem_t dequeue_3 = (data_elem_t) dequeue_3_ptr;
    ASSERT_EQ(dequeue_3->data, 2)
    ASSERT_EQ(dequeue_3_time, 30)
    free_data_elem(dequeue_3_ptr, NULL);

    void *dequeue_4_ptr;
    unsigned int dequeue_4_time =
        event_queue_dequeue_uint_time(queue, &dequeue_4_ptr);
    data_elem_t dequeue_4 = (data_elem_t) dequeue_4_ptr;
    ASSERT_EQ(dequeue_4->data, 4)
    ASSERT_EQ(dequeue_4_time, 40)
    free_data_elem(dequeue_4_ptr, NULL);

    ASSERT_EQ(event_queue_size(queue), 0)

    free_event_queue(queue);
END_TEST

DEFINE_TEST(queue_uint_size_2)
    event_queue_t queue = create_queue_uint_time(
        free_data_elem,
        NULL
    );

    ASSERT_EQ(event_queue_size(queue), 0)

    data_elem_t elem_1 = create_data_elem(10);
    event_queue_enqueue_uint_time(queue, (void *) elem_1, 1);
    ASSERT_EQ(event_queue_size(queue), 1)

    data_elem_t elem_2 = create_data_elem(5);
    event_queue_enqueue_uint_time(queue, (void *) elem_2, 2);
    ASSERT_EQ(event_queue_size(queue), 2)

    data_elem_t elem_3 = create_data_elem(2);
    event_queue_enqueue_uint_time(queue, (void *) elem_3, 3);
    ASSERT_EQ(event_queue_size(queue), 3)

    void * elem_1_ptr;
    event_queue_dequeue_uint_time(queue, &elem_1_ptr);
    ASSERT_EQ(event_queue_size(queue), 2)
    free(elem_1_ptr);

    void * elem_2_ptr;
    event_queue_dequeue_uint_time(queue, &elem_2_ptr);
    ASSERT_EQ(event_queue_size(queue), 1)
    free(elem_2_ptr);

    void * elem_3_ptr;
    event_queue_dequeue_uint_time(queue, &elem_3_ptr);
    ASSERT_EQ(event_queue_size(queue), 0)
    free(elem_3_ptr);

    free_event_queue(queue);
END_TEST

REGISTER_TESTS(
    queue_uint_create_and_destroy,
    queue_uint_enqueue_1,
    queue_uint_enqueue_2,
    queue_uint_size_1,
    queue_uint_dequeue_1,
    queue_uint_dequeue_2,
    queue_uint_size_2
)