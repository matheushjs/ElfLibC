#include <stdlib.h>
#include <queue.h>

//front		    back
//n1 -> n2 -> n3 -> n4
typedef struct node_struct node_t;
struct node_struct{
	void *key;
	node_t *back;
};

struct queue_struct {
	node_t *front, *back;
};

queue_t *queue_alloc(){
	return (queue_t *) calloc(sizeof(queue_t), 1);
}

void queue_insert(queue_t *queue, void *p){
	node_t *node = calloc(sizeof(node_t), 1);
	node->key = p;
	if(queue->back)
		queue->back->back = node;
	else
		queue->front = node;
	queue->back = node;
}

void *queue_get(queue_t *queue){
	node_t *node;
	void *ret;
	node = queue->front;
	if(!node) return NULL;
	ret = node->key;
	queue->front = node->back;
	free(node);
	if(!queue->front) queue->back = NULL;
	return ret;
}

void queue_destroy(queue_t **queue_p){
	node_t *aux, *node;
	queue_t *queue = *queue_p;

	if(!queue) return;
	node = queue->front;
	while(node){
		aux = node;
		node = node->back;
		free(aux);
	}
	free(queue);
	*queue_p = NULL;
};
