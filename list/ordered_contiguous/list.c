#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <list.h>

typedef struct node_struct node_t;
struct node_struct {
	data_t data;
	node_t *next;
};

struct list_struct {
	node_t *first;
	int size;
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

static void node_destroy_r(node_t **node){
	node_t *ptr;
	if(!*node) return;
	do{
		ptr = (*node)->next;
		node_destroy(node);
		node = &ptr;
	} while(*node);
}

static void node_destroy_rf(node_t **node, void (*free_data)(data_t)){
	node_t *ptr;
	if(!*node) return;
	do{
		ptr = (*node)->next;
		free_data((*node)->data);
		node_destroy(node);
		node = &ptr;
	} while(*node);
}

static void node_put_data(node_t *node, data_t *data){
	if(!node) return;
	memcpy(&node->data, data, sizeof(data_t));
}

//From 'node', walk forwards 'n' times and retrieve the reached node.
//Return NULL if out of bounds.
static node_t *node_walk(node_t *node, int n){
	if(n < 0) return NULL;
	while(n-- && node) node = node->next;
	return node;
}

/*
 * List functions
 */

list_t *list_alloc(){
	return (list_t *) calloc(sizeof(list_t), 1);
}

void list_destroy(list_t **list){
	if(*list){
		node_destroy_r(&(*list)->first);
		free(*list);
		*list = NULL;
	}
}

void list_destroy_f(list_t **list, void (*free_data)(data_t)){
	if(*list){
		node_destroy_rf(&(*list)->first, free_data);
		free(*list);
		*list = NULL;
	}
}

bool list_is_empty(list_t *list){
	return list->size ? FALSE : TRUE;
}

//cmp() should yield TRUE when comparing cmp(first_node, last_node);
void list_insert(list_t *list, data_t *data, bool (*cmp)(data_t *, data_t *)){
	node_t *curr, *prev = NULL;
	node_t *node;

	if(!list) return;
	node = node_alloc();
	node_put_data(node, data);

	if(list_is_empty(list)){
		list->first = node;
	} else {
		curr = list->first;
		for(; curr && cmp(&curr->data, data); )
			prev = curr, curr = curr->next;
		//Case where insertion happens before first node.
		if(!prev){
			node->next = curr;
			list->first = node;
		} else {
			prev->next = node;
			node->next = curr;
		}
	}
	list->size++;
}

data_t *list_retrieve(list_t *list, int index){
	node_t *node;
	data_t *data;
	if(!list || list_is_empty(list)) return NULL;
	node = node_walk(list->first, index);
	data = (data_t *) malloc(sizeof(data_t));
	memcpy(data, &node->data, sizeof(data_t));
	return data;
}

void list_remove(list_t *list, int index){
	if(!list) return;
	if(list_is_empty(list)) return;
	
	node_t *node, *aux;
	if(!index){
		aux = list->first->next;
		node_destroy(&list->first);
		list->first = aux;
	} else {
		node = node_walk(list->first, index-1);
		if(!node) return;	//out of bounds
		aux = node->next;
		node->next = aux->next;
		node_destroy(&aux);
	}
	list->size--;
}

void list_print_r(list_t *list, void (*print)(data_t *)){
	if(!list) return;
	node_t *curr = list->first;
	while(curr){
		print(&curr->data);
		printf(" ");
		curr = curr->next;
	}
}

int list_size(list_t *list){
	if(!list) return -1;
	return list->size;
}
