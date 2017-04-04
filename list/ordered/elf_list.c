#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)

typedef struct _Node Node;
struct _Node {
	void *key;
	Node *next;
};

//List will be crescently ordered, based on the 'greater' function.
//If the greater function is actually a lower function, it will be sorted decrescently.
//If the greater function is not a 'greater than or equal to', then duplicates
//  will be order such that older data appears first.
typedef struct _ElfList ElfList;
struct _ElfList {
	Node *first;
	bool (*greater)(void *a, void *b); // true if (a > b)
	int size;
};

static
Node *node_new(){
	return (Node *) calloc(sizeof(Node), 1);
}

/*
 * List functions
 */

//Creates a new ordered list, with comparison function 'greaterThan'.
ElfList *elf_list_new(bool (*greaterThan)(void*,void*)){
	ElfList *list = calloc(sizeof(ElfList), 1);
	list->greater = greaterThan;
	return list;
}

//Inserts 'data' into the list.
void elf_list_insert(ElfList *list_p, void *data){
	if(!list_p) ELF_DIE("Received null pointer");
	
	ElfList list = *list_p; //For efficiency
	
	Node *new = node_new();
	new->key = data;
	list_p->size++;

	Node *curr = list.first;
	if(curr == NULL || list.greater(curr->key, data)){ //if curr->key > data
		new->next = curr;
		list_p->first = new;
		return;
	}

	while(curr->next != NULL
	   && !list.greater(curr->next->key, data)){ //while next->key <= data
		curr = curr->next;
	}

	new->next = curr->next;
	curr->next = new;
}

//Destroys a list, doing nothing to the pointers stored in it.
void elf_list_destroy(ElfList **list_p){
	ElfList *list = *list_p;
	Node *node, *aux;

	if(list){
		node = list->first;
		while(node != NULL){
			aux = node->next;
			free(node);
			node = aux;
		}
		free(list);
		*list_p = NULL;
	}
}

//Destroys a list, using function 'func' to free the stored pointers.
void elf_list_destroy_f(ElfList **list_p, void (*func)(void*)){
	ElfList *list = *list_p;
	Node *node, *aux;

	if(list){
		node = list->first;
		while(node != NULL){
			aux = node->next;
			func(node->key);
			free(node);
			node = aux;
		}
		free(list);
		*list_p = NULL;
	}
}

//Removes and returns the item at index 'index'.
//If 'index' is out of range, nothing is done and NULL is returned.
void *elf_list_remove(ElfList *list, int index){
	int i;
	void *key;
	
	if(!list) ELF_DIE("Received null pointer");

	Node *prev, *cur = list->first;
	if(cur == NULL) return NULL;

	prev = NULL;
	for(i = 0; i < index; i++){
		prev = cur;
		cur = cur->next;
		if(cur == NULL) return NULL;
	}

	//At this point, cur is not NULL and positioned in the index-th element.
	//prev should be the previous node, or NULL if index = 0.
	key = cur->key;

	if(prev == NULL){
		list->first = cur->next;
	} else {
		prev->next = cur->next;
	}
	free(cur);

	return key;
}

//Returns the number of elements in the list.
int elf_list_size(const ElfList *list){
	if(!list) ELF_DIE("Received null pointer");
	return list->size;
}

//Returns the element at index 'index'.
//If 'index' is out of range, returns NULL.
void *elf_list_get(const ElfList *list, int index){
	int i;

	if(!list) ELF_DIE("Received null pointer");
	
	Node *cur = list->first;
	if(cur == NULL) return NULL;

	for(i = 0; i < index; i++){
		cur = cur->next;
		if(cur == NULL) return NULL;
	}

	return cur->key;
}

//Traverses all nodes in the list, applying 'func' in each stored pointer.
void elf_list_traverse(ElfList *list, void(*func)(void*)){
	Node *cur;
	
	if(!list) ELF_DIE("Received null pointer");
	
	cur = list->first;
	while(cur != NULL){
		func(cur->key);
		cur = cur->next;
	}
}
