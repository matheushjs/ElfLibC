#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <slist.h>

#define die(X) fprintf(stderr, "%s:%d:%s(): ERROR: %s", __FILE__, __LINE__, __func__, X "\n"), exit(EXIT_FAILURE)

typedef struct node_struct node_t;
struct node_struct {
	data_t data;
	node_t *next;
};

struct list_struct {
	node_t *first;
	int size;
};

static void (*free_data)(data_t *) = NULL;
static void (*copy_data)(data_t *, data_t *) = NULL;
static bool (*cmp_data)(data_t *, data_t *) = NULL;
static bool (*equal_data)(data_t *, data_t *) = NULL;
static void (*print_data)(data_t *) = NULL;

/*
 * Node functions
 */

static
node_t *node_alloc(){
	return (node_t *) calloc(sizeof(node_t), 1);
}

static
void node_destroy(node_t **node){
	if(*node){
		if(free_data) free_data(&(*node)->data);
		free(*node);
		*node = NULL;
	}
}

static
node_t *node_create(data_t *data){
	node_t *node = node_alloc();
	if(!copy_data) memcpy(&node->data, data, sizeof(data_t));
	else copy_data(&node->data, data);
	return node;
}

//From 'node', walk forwards 'n' times and retrieve the reached node.
//Return NULL if out of bounds.
static
node_t *node_walk(node_t *node, int n){
	if(n < 0) return NULL;
	while(n-- && node) node = node->next;
	return node;
}

/*
 * List functions
 */

void list_set_free_data(void (*free_func)(data_t *)){ free_data = free_func; }
void list_set_copy_data(void (*copy)(data_t *, data_t *)){ copy_data = copy; }
void list_set_cmp_data(bool (*cmp)(data_t *, data_t *)){ cmp_data = cmp; }
void list_set_equal_data(bool (*equal)(data_t *, data_t *)){ equal_data = equal; }
void list_set_print_data(void (*print)(data_t *)){ print_data = print; }

list_t *list_alloc(){
	return (list_t *) calloc(sizeof(list_t), 1);
}

void list_destroy(list_t **list){
	if(!*list) return;
	list_t *l;
	node_t *node, *prev;

	l = *list;
	node = l->first;
	if(!node) return;
	
	while(node){
		node = (prev = node, node->next);
		node_destroy(&prev);
	}
	
	free(l);
	*list = NULL;
}

bool list_is_empty(list_t *list){
	return list->size ? FALSE : TRUE;
}

void list_insert(list_t *list, data_t *data){
	node_t *curr, *prev = NULL;
	node_t *node;

	if(!list) return;
	if(!cmp_data){ die("Comparison function is not set.\n"); }
	if(!equal_data){ die("Equality function is not set.\n"); }
	node = node_create(data); 

	if(list_is_empty(list)){
		list->first = node;
	} else {
		curr = list->first;
		while(curr && cmp_data(&curr->data, data))
			curr = (prev = curr, curr->next);
		
		//If data is in the list already.
		if(curr && equal_data(&curr->data, data)){ node_destroy(&node); return; }

		//Case where insertion happens before first node.
		if(!prev){
			node->next = list->first;
			list->first = node;
		} else {
			prev->next = node;
			node->next = curr;
		}
	}
	list->size++;
}

//Returns NULL if element 'key' isn't found.
//If found, stores the element's index in 'index'
data_t *list_search(list_t *list, data_t *key, int *index){
	int i = 0;
	node_t *curr;

	if(!cmp_data){ die("Comparison function is not set.\n"); }
	if(!equal_data){ die("Equality function is not set.\n"); }
	if(list){
		curr = list->first;
		while(curr && cmp_data(&curr->data, key))
			curr = (i++, curr->next);

		if(curr && equal_data(&curr->data, key)){
			if(index) *index = i-1;
			return &curr->data;
		}
	}

	if(index) *index = -1;
	return NULL;
}

data_t *list_retrieve(list_t *list, int index){
	if(!list || index < 0 || index > list->size) return NULL;
	node_t *node;
	node = node_walk(list->first, index);
	return &node->data;
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

void list_remove_key(list_t *list, data_t *key){
	node_t *prev = NULL, *curr;

	if(!cmp_data){ die("Comparison function is not set.\n"); }
	if(!equal_data){ die("Equality function is not set.\n"); }
	if(!list || list_is_empty(list)) return;
	
	curr = list->first;
	while(curr && cmp_data(&curr->data, key))
		curr = (prev = curr, curr->next);

	if(curr && equal_data(&curr->data, key)){
		if(prev) prev->next = curr->next;
		else list->first = curr->next;
		node_destroy(&curr);
		list->size--;
	}
}

void list_print(list_t *list){
	if(!list) return;
	if(!print_data){ fprintf(stderr, "Print function is not set\n"); return; }
	node_t *curr = list->first;
	while(curr){
		print_data(&curr->data);
		printf(" ");
		curr = curr->next;
	}
}

int list_size(list_t *list){
	if(!list) return -1;
	return list->size;
}
