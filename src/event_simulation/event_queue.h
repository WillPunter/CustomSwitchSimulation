/*  event_queue.h

    This declares the API for the event queue for a discrete
    event simulation.
    
    The event queue is a priority queue which uses simulated time
    as the priority. The aim is to keep the priority queue as
    generic as possible (in terms of the representation of the
    data elements and the representation of the time - could be
    integer, double or even string dates if the user implements
    a comparison function).
    
    C generics can be a bit of a pain and certainly create quite
    nasty interfaces, so the API includes some default functions
    for constructing event queues. */

#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "./data_structures/heap.h"

struct event_queue;

struct event;

typedef struct event_queue * event_queue_t;
typedef struct event * event_t;

event_queue_t create_generic_event_queue(
    func_comparator_t time_comparator,
    func_free_t free_data,
    func_free_t free_time,
    void *free_data_arg,
    void *free_time_arg
);

event_queue_t create_queue_uint_time(
    func_free_t free_data,
    void *free_data_arg
);

event_queue_t create_queue_double_time(
    func_free_t free_data,
    void *free_data_arg
);

int event_queue_enqueue(event_queue_t queue, void *elem, void *time);

void event_queue_enqueue_uint_time(
    event_queue_t queue,
    void *elem,
    unsigned int time_val
);

void event_queue_enqueue_double_time(
    event_queue_t queue,
    void *elem,
    double time_val
);

unsigned int event_queue_size(event_queue_t queue);

void event_queue_peek(
    event_queue_t queue,
    void ** elem_out,
    void ** time_out
);

unsigned int event_queue_peek_uint_time(
    event_queue_t queue,
    void ** elem_out
);

double event_queue_peek_double_time(
    event_queue_t queue,
    void ** elem_out
);

void event_queue_dequeue(
    event_queue_t queue,
    void ** elem_out,
    void ** time_out
);

unsigned int event_queue_dequeue_uint_time(
    event_queue_t queue,
    void ** elem_out
);

double event_queue_dequeue_double_time(
    event_queue_t queue,
    void ** elem_out
);

event_queue_t free_event_queue(event_queue_t queue);

#endif