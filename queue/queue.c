#include <stdlib.h>
#include <queue.h>

typedef struct node_struct node_t;
struct node_struct {
	node_t *head;
	node_t *tail;
	data_t *key;
};

struct queue_struct {
	node_t *head;
	node_t *tail;
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

void queue_push(queue_t *q, data_t *data){
	node_t *new;
	
	if(!q) return;
	new = node_alloc();
	new->key = data;

	if(!q->count){
		q->head = q->tail = new;
	} else {
		q->tail->tail = new;
		new->head = q->tail;
		q->tail = new;
	}
	(q->count)++;
}

data_t *queue_pop(queue_t *q){
	data_t *ret = NULL;

	if(!q) return ret;
	if(!q->count) return ret;
	(q->count)--;

	ret = q->head->key;
	if(!q->count){
		node_destroy(&q->head);
		q->tail = NULL;
	}else{
		q->head = q->head->tail;
		node_destroy(&q->head->head);
	}
	return ret;
}

//Destroy the queue, assuming (data_t *) can be freed with a simple free(data_t *).
void queue_destroy(queue_t **q_pointer){
	queue_t *q;

	if(*q_pointer){
		q = *q_pointer;
		while(q->count) free(queue_pop(q));
		free(q);
		q = NULL;
	}
}

//Destroy the queue, freeing each (data_t *) using function 'f'.
void queue_destroy_f(queue_t **q_pointer, void (*f)(data_t *p)){
	queue_t *q;

	if(*q_pointer){
		q = *q_pointer;
		while(q->count) f(queue_pop(q));
		free(q);
		q = NULL;
	}
}

int queue_size(queue_t *q){
	if(!q) return -1;
	else return q->count;
}

data_t *queue_front(queue_t *q){
	if(!q || !q->count) return NULL;
	else return q->head->key;
}

data_t *queue_back(queue_t *q){
	if(!q || !q->count) return NULL;
	else return q->tail->key;
}

void queue_invert(queue_t *q){
	if(!q || !q->count) return;
	node_t *curr, *aux;

	curr = q->head;
	
	while(curr){
		aux = curr->tail;
		curr->tail = curr->head;
		curr->head = aux;
		curr = curr->head;
	}
	
	aux = q->head;
	q->head = q->tail;
	q->tail = aux;
}
