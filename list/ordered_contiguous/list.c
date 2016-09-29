#include <stdlib.h>
#include <stdio.h>

typedef int data_t;

typedef enum {FALSE, TRUE} bool;

typedef struct node_struct node_t;
struct node_struct {
	data_t data;
	node_t *next;
};

typedef struct list_struct list_t;
struct list_struct {
	node_t *first;
	int size;
};

//Should yield TRUE when comparing cmp(first_node, last_node);
static bool (*cmp)(data_t *, data_t *);


/*
 * Node functions
 */

void node_destroy_recursive(node_t *node){
	node_t *ptr;
	if(!node) return;
	do{
		ptr = node->next;
		free(node);
		node = ptr;
	} while(node);
}


/*
 * List functions
 */

void list_set_cmp(bool (*function)(data_t *, data_t *)){
	cmp = function;
}

list_t *list_alloc(){
	return (list_t *) calloc(sizeof(list_t), 1);
}

void list_destroy(list_t **list){
	if(*list){
		node_destroy_recursive((*list)->first);
		free(*list);
	}
}

bool list_is_empty(list_t *list){
	return list->size ? FALSE : TRUE;
}

void list_insert(list_t *list, data_t *data){
	if(!list) return;
	if(list_is_empty(list)){
		//Adds first node
		return;
	} else {
		//Inserts node where it's due.
	}
}

data_t *list_access(list_t *list, int index){

}

void list_remove(list_t *list, int index){
	if(!list) return;
	if(list_is_empty(list)) return;
}

int main(int argc, char *argv[]){
	bool function(data_t *d1, data_t *d2){ return *d1 < *d2 ? TRUE : FALSE; }
	list_set_cmp(function);
	return 0;
}
