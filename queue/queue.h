#ifndef _QUEUE_H
#define _QUEUE_H

//This queue was made so that only pointers can be stored.
//data_t should be the data type you want to store.
typedef struct person_struct data_t;

typedef enum {FALSE, TRUE} bool;
typedef struct queue_struct queue_t;

queue_t *queue_alloc();
void queue_enqueue(queue_t *, data_t *);
data_t *queue_next(queue_t *);
void queue_destroy(queue_t **);
void queue_destroy_f(queue_t **, void (*)(data_t *));
int queue_size(queue_t *);
data_t *queue_front(queue_t *);
data_t *queue_back(queue_t *);
void queue_invert(queue_t *);

#endif
