#ifndef _QUEUE_H
#define _QUEUE_H

//This queue will store void pointers.
typedef struct queue_struct queue_t;

queue_t *queue_alloc();
void queue_insert(queue_t *, void *);
void *queue_get(queue_t *);
void queue_destroy(queue_t **);

#endif
