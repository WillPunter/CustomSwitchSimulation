/*  event_queue.c

    Implementation of event queue for discrete event simulation. */

#include "event_queue.h"
#include <stddef.h>
#include <assert.h>
#include <malloc.h>

/*  Event structure implementation. The idea is to keep this as generic as
    possible, meaning that an event is just a combination of two void pointers.
    The first points to a data payload of the users choice. The second points
    to a time value. The comparator passed to the event queue should perform
    comparison on the time values. */
struct event {
    void *data;
    void *time;
};

/*  Event queue implementation. */
struct event_queue {
    binary_heap_t heap;
    func_comparator_t time_comparator;
    func_free_t free_data;
    func_free_t free_time;
    void * free_data_arg;
    void * free_time_arg;
};

/*  Basic time representations - uint and double. */
struct uint_time {
    unsigned int time;
};

typedef struct uint_time * uint_time_t;

struct double_time {
    double time;
};

typedef struct double_time * double_time_t;

/*  Forward declare functions for unsigned integer and doubled based time
    structures for use in default event queues. */
static uint_time_t create_uint_time(unsigned int time_val);
static void free_uint_time(void * time_ptr, void * arg);
static comparison_t uint_time_comparator(void *lhs, void *rhs, void *arg);
static double_time_t create_double_time(double time_val);
static void free_double_time(void * time_ptr, void * arg);
static comparison_t double_time_comparator(void *lhs, void *rhs, void *arg);

/*  Event comparator - this is necessary because our priority queue is
    internally represented as a heap of events, where an event stores
    two generic pointers (to a data payload and to a time value). Event
    comparison should simply just defer to the time comparison for a
    discrete event simulation. */
static comparison_t event_comparator(void *lhs, void *rhs, void *queue_ptr) {
    event_queue_t queue = (event_queue_t) queue_ptr;
    
    event_t event_l = (event_t) lhs;
    event_t event_r = (event_t) rhs;

    return queue->time_comparator(event_l->time, event_r->time, NULL);

    return LESS_THAN;
}

static void event_free(void *event_ptr, void *queue_ptr) {
    event_queue_t queue = (event_queue_t) queue_ptr;

    event_t event = (event_t) event_ptr;

    /*  Free data payload. */
    queue->free_data(event->data, queue->free_data_arg);

    /*  Free time. */
    queue->free_time(event->time, queue->free_time_arg);

    /*  Free event. */
    free(event);

    return;
}

/*  Create generic event queue. */
event_queue_t create_generic_event_queue(
    func_comparator_t time_comparator,
    func_free_t free_data,
    func_free_t free_time,
    void *free_data_arg,
    void *free_time_arg
) {
    /*  Allocate space for event queue structure. */
    event_queue_t queue = malloc(sizeof(struct event_queue));
    assert(queue);

    /*  Time comparator compares two time elements, but we need the
        heap to compare on two event elements using their time values.
  
        So we need to create an event comparator using the time comparator. */
    queue->time_comparator = time_comparator;

    /*  Set free functions for data and time inside elment structure. */
    queue->free_data = free_data;
    queue->free_time = free_time;

    /*  Set arguments passed to said free functions. */
    queue->free_data_arg = free_data_arg;
    queue->free_time_arg = free_time_arg;

    /*  Create heap with provided comparator */
    queue->heap = create_empty_heap(event_comparator, event_free);
    binary_heap_set_comparator_arg(queue->heap, queue);
    binary_heap_set_free_arg(queue->heap, queue);

    return queue;
}

/*  Create queue using integer time. */
event_queue_t create_queue_uint_time(
    func_free_t free_data,
    void *free_data_arg
) {
    return create_generic_event_queue(
        uint_time_comparator,
        free_data,
        free_uint_time,
        free_data_arg,
        NULL
    );
}

/*  Create queue using double time. */
event_queue_t create_queue_double_time(
    func_free_t free_data,
    void *free_data_arg
) {
    return create_generic_event_queue(
        double_time_comparator,
        free_data,
        free_double_time,
        free_data_arg,
        NULL
    );
}

/*  Insert element into queue. */
int event_queue_enqueue(event_queue_t queue, void *elem, void *time) {
    event_t event = malloc(sizeof(struct event));
    assert(time);
    event->data = elem;
    event->time = time;

    return binary_heap_insert(queue->heap, event);
}

/*  Insert element into queue with uint time. */
void event_queue_enqueue_uint_time(
    event_queue_t queue,
    void *elem,
    unsigned int time_val
) {
    uint_time_t time = create_uint_time(time_val);
    event_queue_enqueue(queue, elem, (void *) time);
}

/*  Insert element into queue with double time. */
void event_queue_enqueue_double_time(
    event_queue_t queue,
    void *elem,
    double time_val
) {
    double_time_t time = create_double_time(time_val);
    event_queue_enqueue(queue, elem, (void *) time);
}

/*  Get queue size. */
unsigned int event_queue_size(event_queue_t queue) {
    return binary_heap_size(queue->heap);
};

/*  Peek at element at the front of the queue. */
void event_queue_peek(
    event_queue_t queue,
    void ** elem_out,
    void ** time_out
) {
    event_t event = binary_heap_min(queue->heap);
    *elem_out = event->data;
    *time_out = event->time;
}

/*  Peek at element with unsigned int time. */
unsigned int event_queue_peek_uint_time(
    event_queue_t queue,
    void ** elem_out
) {
    event_t event = binary_heap_min(queue->heap);
    *elem_out = event->data;
    
    uint_time_t time = (uint_time_t) event->time;

    return time->time;
}

/*  Peek at element with double time. */
double event_queue_peek_double_time(
    event_queue_t queue,
    void ** elem_out
) {
    event_t event = binary_heap_min(queue->heap);
    *elem_out = event->data;

    double_time_t time = (double_time_t) event->time;

    return time->time;
}

/*  Pop element from queue. This takes a queue, and two void double pointers
    which upon the return of the function will point to the element and time
    structures respectively.*/
void event_queue_dequeue(
    event_queue_t queue,
    void ** elem_out,
    void ** time_out
) {
    event_t event_popped = binary_heap_pop_min(queue->heap);

    *elem_out = event_popped->data;
    *time_out = event_popped->time;

    /*  Free the event structure as no longer needed. */
    free(event_popped);
};

/*  Pop element from queue with unsigned int time representation. This inteface
    is slightly different to the generic dequeue interface since it uses an
    output parameter for the data element but returns the time as a uint.
    This allows the underlying time structure to be deallocated without the
    caller having to worry about this. */
unsigned int event_queue_dequeue_uint_time(
    event_queue_t queue,
    void ** elem_out
) {
    void * time_ptr;
    event_queue_dequeue(queue, elem_out, &time_ptr);

    uint_time_t time = (uint_time_t) time_ptr;

    unsigned int time_val = time->time;

    /*  Free time structure. */
    free(time);

    return time_val;
}

/*  Popping an eleemt from a queue with double time representaiton is similar
    to the above, just with a different type. */
double event_queue_dequeue_double_time(
    event_queue_t queue,
    void ** elem_out
) {
    void * time_ptr;
    event_queue_dequeue(queue, elem_out, &time_ptr);

    double_time_t time = (double_time_t) time_ptr;

    unsigned int time_val = time->time;

    /*  Free time structure. */
    free(time);

    return time_val;
}

/*  Free event queue. */
event_queue_t free_event_queue(event_queue_t queue) {
    /*  Free the underlying heap - the event free function
        should free the event structures as well as the
        underyling time and data payloads. */
    free_heap(queue->heap);
}

/*  Time representation functions. */
static uint_time_t create_uint_time(unsigned int time_val) {
    uint_time_t time = malloc(sizeof(struct uint_time));
    time->time = time_val;
    return time;
}

static void free_uint_time(void * time_ptr, void * arg) {
    free((uint_time_t) time_ptr);
}

static comparison_t uint_time_comparator(void *lhs, void *rhs, void *arg) {
    uint_time_t time_l = (uint_time_t) lhs;
    uint_time_t time_r = (uint_time_t) rhs;

    if (time_l->time < time_r->time) {
        return LESS_THAN;
    } else if (time_l->time > time_r->time) {
        return GREATER_THAN;
    } else {
        return EQUAL_TO;
    }
}

static double_time_t create_double_time(double time_val) {
    double_time_t time = malloc(sizeof(struct double_time));
    time->time = time_val;
    return time;
}

static void free_double_time(void * time_ptr, void * arg) {
    free((double_time_t) time_ptr);
}

static comparison_t double_time_comparator(void *lhs, void *rhs, void *arg) {
    double_time_t time_l = (double_time_t) lhs;
    double_time_t time_r = (double_time_t) rhs;

    if (time_l->time < time_r->time) {
        return LESS_THAN;
    } else if (time_l->time > time_r->time) {
        return GREATER_THAN;
    } else {
        return EQUAL_TO;
    }
}