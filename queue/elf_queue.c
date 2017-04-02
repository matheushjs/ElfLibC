#include <stdio.h>
#include <stdlib.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)

/* Pointers diagram:
 *
 * back <- node <- ... <- node <- front
 */
typedef struct _Node Node;
struct _Node {
	Node *back;
	void *key;
};

typedef struct _ElfQueue ElfQueue;
struct _ElfQueue {
	Node *front;
	Node *back;
	int count;
};

/*
 * Node functions
 */

static Node *node_alloc(){
	return (Node *) calloc(sizeof(Node), 1);
}

static void node_destroy(Node **node){
	if(*node){
		free(*node);
		*node = NULL;
	}
}

/*
 * Queue functions
 */

ElfQueue *elf_queue_alloc(){
	return (ElfQueue *) calloc(sizeof(ElfQueue), 1);
}

//Inserts a node at the back of the queue.
void elf_queue_push(ElfQueue *q, void *data){
	Node *new;
	
	if(!q) ELF_DIE("Received NULL queue");
	new = node_alloc();
	new->key = data;

	if(q->count == 0){
		q->front = q->back = new;
	} else {
		q->back->back = new;
		q->back = new;
	}
	(q->count)++;
}

//Removes and returns the front element.
//Returns NULL if queue is empty.
void *elf_queue_pop(ElfQueue *q){
	Node *node;
	void *ret;

	if(!q) ELF_DIE("Received NULL queue");
	if(!q->count) return NULL;

	node = q->front;
	ret = node->key;
	(q->count)--;

	q->front = node->back;
	if(!q->count) q->back = NULL;

	node_destroy(&node);

	return ret;
}

//Destroys the queue. Nothing is done to the stored pointers.
void elf_queue_destroy(ElfQueue **q_pointer){
	ElfQueue *q = *q_pointer;
	Node *node, *aux;

	if(q != NULL){
		node = q->front;
		while(node != NULL){
			aux = node;
			node = node->back;
			node_destroy(&aux);
		}
		free(q);
		*q_pointer = NULL;
	}
}

//Destroy the queue, freeing each key using function 'func'.
void elf_queue_destroy_f(ElfQueue **q_pointer, void(*func)(void*)){
	ElfQueue *q = *q_pointer;
	Node *node, *aux;

	if(q != NULL){
		node = q->front;
		while(node != NULL){
			aux = node;
			node = node->back;
			func(aux->key);
			node_destroy(&aux);
		}
		free(q);
		*q_pointer = NULL;
	}
}

int elf_queue_size(const ElfQueue *q){
	if(!q) ELF_DIE("Received NULL queue");
	return q->count;
}

//Access the front element, without removing it.
void *elf_queue_front(const ElfQueue *q){
	if(!q) ELF_DIE("Received NULL queue");
	if(!q->count) return NULL;
	return q->front->key;
}

//Access the back element, without removing it.
void *elf_queue_back(const ElfQueue *q){
	if(!q) ELF_DIE("Received NULL queue");
	if(!q->count) return NULL;
	return q->back->key;
}

//Inverts the queue.
void elf_queue_invert(ElfQueue *q){
	if(!q) ELF_DIE("Received NULL queue");

	/* Starts on the front node.
	 * for all nodes:
	 * 	Store the address of the next node
	 * 	Make current node point to the last address stored
	 * 	Store the current node's address
	 * 	node = next node
	 */

	Node *curr, *stored, *next;

	curr = q->front;
	stored = NULL;
	while(curr){
		next = curr->back;	//address of next node to go
		curr->back = stored;	//Current node points to the stored node
		stored = curr;		//Current node is now the stored node
		curr = next;		//Go to next node
	}
	
	next = q->front; //'next' is now being used as a temporary
	q->front = q->back;
	q->back = next;
}

//Traverse the queue, from front to back, applying function 'func' in each element.
void elf_queue_traverse(const ElfQueue *q, void(*func)(void *)){
	if(!q) ELF_DIE("Received NULL queue");

	Node *node = q->front;
	while(node != NULL){
		func(node->key);
		node = node->back;
	}
}