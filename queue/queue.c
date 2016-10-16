#include <stdlib.h>
#include <queue.h>

typedef struct node_struct node_t;
struct node_struct {
	node_t *front;
	data_t *key;
};

struct queue_struct {
	node_t *front;
	node_t *back;
	int count;
};

/*
 * Node functions
 */

static node_t *node_alloc(){
	return (node_t *) calloc(sizeof(node_t), 1);
}

static void node_destroy(node_t **node){
	if(*node){
		free(*node);
		*node = NULL;
	}
}


/*
 * Queue functions
 */

queue_t *queue_alloc(){
	return (queue_t *) calloc(sizeof(queue_t), 1);
}

void queue_enqueue(queue_t *q, data_t *data){
	node_t *new;
	
	if(!q) return;
	new = node_alloc();
	new->key = data;

	if(!q->count){
		q->front = q->back = new;
	} else {
		q->front->front = new;
		q->front = new;
	}
	(q->count)++;
}

data_t *queue_next(queue_t *q){
	data_t *ret = NULL;
	node_t *aux;

	if(!q) return ret;
	if(!q->count) return ret;
	(q->count)--;

	ret = q->back->key;
	if(!q->count){
		node_destroy(&q->back);
		q->back = q->front = NULL;
	}else{
		aux = q->back;
		q->back = q->back->front;
		node_destroy(&aux);
	}
	return ret;
}

//Destroy the queue, assuming (data_t *) can be freed with a simple free(data_t *).
void queue_destroy(queue_t **q_pointer){
	queue_t *q = *q_pointer;;

	if(q){
		while(q->count) free(queue_next(q));
		free(q);
		*q_pointer = NULL;
	}
}

//Destroy the queue, freeing each (data_t *) using function 'f'.
void queue_destroy_f(queue_t **q_pointer, void (*f)(data_t *p)){
	queue_t *q = *q_pointer;

	if(q){
		while(q->count) f(queue_next(q));
		free(q);
		*q_pointer = NULL;
	}
}

int queue_size(queue_t *q){
	if(!q) return -1;
	else return q->count;
}

data_t *queue_front(queue_t *q){
	if(!q || !q->count) return NULL;
	else return q->front->key;
}

data_t *queue_back(queue_t *q){
	if(!q || !q->count) return NULL;
	else return q->back->key;
}

void queue_invert(queue_t *q){
	if(!q || !q->count) return;
	node_t *curr, *next, *new;

	curr = q->back;
	new = NULL;
	while(curr){
		next = curr->front;
		curr->front = new;
		new = curr;
		curr = next;
	}
	
	new = q->front;
	q->front = q->back;
	q->back = new;
}
