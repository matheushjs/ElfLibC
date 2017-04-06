#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <elf_list.h>

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
	bool (*equal)(void *a, void *b); //true if (a == b)
	int size;
};

static inline
Node *node_new(){
	return (Node *) calloc(sizeof(Node), 1);
}

/*
 * List functions
 */

//Creates a new ordered list, with comparison function 'greaterThan'.
//By not having 'equal', some operations are not supported.
ElfList *elf_list_new(bool (*greaterThan)(void*,void*)){
	ElfList *list = calloc(sizeof(ElfList), 1);
	list->greater = greaterThan;
	list->equal = NULL;
	return list;
}

//Creates a new ordered list, with comparison function 'greaterThan'
//  and equality function 'equal'.
ElfList *elf_list_newWithEqual(bool (*greaterThan)(void*,void*), bool (*equal)(void*,void*)){
	ElfList *list = calloc(sizeof(ElfList), 1);
	list->greater = greaterThan;
	list->equal = equal;
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
	Node *prev = NULL;
	while(curr != NULL && !list.greater(curr->key, data)){ // while curr < data
		prev = curr;
		curr = curr->next;
	}

	if(prev == NULL){
		new->next = list_p->first;
		list_p->first = new;
	} else {
		new->next = prev->next;
		prev->next = new;
	}
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
void elf_list_destroyF(ElfList **list_p, void (*func)(void*)){
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
	if(index < 0 || index >= list->size) return NULL;

	Node *prev, *cur = list->first;

	prev = NULL;
	for(i = 0; i < index; i++){
		prev = cur;
		cur = cur->next;
	}

	//At this point, cur is not NULL and positioned in the index-th element.
	//prev should be the previous node, or NULL if index = 0.
	key = cur->key;

	if(index == 0){
		list->first = cur->next;
	} else {
		prev->next = cur->next;
	}
	free(cur);

	list->size--;
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
	if(index < 0 || index >= list->size) return NULL;
	
	Node *cur = list->first;

	for(i = 0; i < index; i++)
		cur = cur->next;

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

//Inserts 'data' into the list if it's not a duplicate.
//If list doesn't have an 'equal' function, exits the program.
//Returns true is 'data' is unique, false if it's a duplicate.
bool elf_list_insertUnique(ElfList *list_p, void *data){
	if(!list_p) ELF_DIE("Received null pointer");
	if(!list_p->equal) ELF_DIE("List does not have equal() function");

	ElfList list = *list_p; //For efficiency
	
	Node *new = node_new();
	new->key = data;

	Node *curr = list.first;
	Node *prev = NULL;
	while(curr != NULL && list.greater(data, curr->key)){ // while data > curr
		prev = curr;
		curr = curr->next;
	}

	if(curr != NULL && list.equal(curr->key, data)){
		free(new);
		return false;
	} else if(prev == NULL){
		new->next = list_p->first;
		list_p->first = new;
	} else {
		new->next = prev->next;
		prev->next = new;
	}
	list_p->size++;
	return true;
}

//If list contains 'data', return its index. Return -1 otherwise.
int elf_list_indexOf(ElfList *list_p, void *data){
	if(!list_p) ELF_DIE("Received null pointer");
	if(!list_p->equal) ELF_DIE("List does not have equal() function");
	
	ElfList list = *list_p; //For efficiency
	Node *curr = list.first;
	
	int count = 0;
	while(curr != NULL && list.greater(data, curr->key)){
		curr = curr->next;
		count++;
	}

	if(curr != NULL && list.equal(curr->key, data)){
		return count;
	} else return -1;
}

//Returns true if the list contains 'data'.
bool elf_list_contains(ElfList *list, void *data){
	if(elf_list_indexOf(list, data) != -1)
		return true;
	else return false;
}

//Returns the number of elements 'data' within the list.
int elf_list_count(ElfList *list_p, void *data){
	if(!list_p) ELF_DIE("Received null pointer");
	if(!list_p->equal) ELF_DIE("List does not have equal() function");
	
	ElfList list = *list_p; //For efficiency
	Node *curr = list.first;
	
	while(curr != NULL && list.greater(data, curr->key))
		curr = curr->next;
	
	int count = 0;
	while(curr != NULL && list.equal(curr->key, data)){
		count++;
		curr = curr->next;
	}
	return count;
}
