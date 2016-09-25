#include <queue.h>

typedef struct node_struct node_t;
struct node_struct {
	node_t *head;
	node_t *tail;
	data_t key;
};

typedef struct queue_struct queue_t;
struct queue_struct {
	node_t *head;
	node_t *tail;
};
