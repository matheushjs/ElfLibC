#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <elf_list.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)

//For the convenience of writing shorter code,
//the node structure used in this source file is
//simply called Node. The structure to be exported
//should be called ElfListIt.
typedef struct _ElfListIt Node;

//List will be crescently ordered, based on the 'greater' function.
//If the greater function is actually a lower function, it will be sorted decrescently.
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

// Documented in header file.
ElfList *elfList_new(bool (*greaterThan)(void*,void*)){
	ElfList *list = calloc(sizeof(ElfList), 1);
	list->greater = greaterThan;
	list->equal = NULL;
	return list;
}

// Documented in header file.
ElfList *elfList_newWithEqual(bool (*greaterThan)(void*,void*), bool (*equal)(void*,void*)){
	ElfList *list = calloc(sizeof(ElfList), 1);
	list->greater = greaterThan;
	list->equal = equal;
	return list;
}

// Documented in header file.
void elfList_insert(ElfList *list_p, void *data){
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

// Documented in header file.
void elfList_destroy(ElfList **list_p){
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

// Documented in header file.
void elfList_destroyF(ElfList **list_p, void (*func)(void*)){
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

// Documented in header file.
void *elfList_removeIndex(ElfList *list, int index){
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

// Documented in header file.
int elfList_size(const ElfList *list){
	if(!list) ELF_DIE("Received null pointer");
	return list->size;
}

// Documented in header file.
void *elfList_get(const ElfList *list, int index){
	int i;

	if(!list) ELF_DIE("Received null pointer");
	if(index < 0 || index >= list->size) return NULL;
	
	Node *cur = list->first;

	for(i = 0; i < index; i++)
		cur = cur->next;

	return cur->key;
}

// Documented in header file.
void elfList_traverse(ElfList *list, void(*func)(void*)){
	Node *cur;
	
	if(!list) ELF_DIE("Received null pointer");
	
	cur = list->first;
	while(cur != NULL){
		func(cur->key);
		cur = cur->next;
	}
}

// Documented in header file.
bool elfList_insertUnique(ElfList *list_p, void *data){
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

// Documented in header file.
int elfList_indexOf(ElfList *list_p, void *data){
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

// Documented in header file.
bool elfList_contains(ElfList *list, void *data){
	if(elfList_indexOf(list, data) != -1)
		return true;
	else return false;
}

// Documented in header file.
int elfList_count(ElfList *list_p, void *data){
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

// Documented in header file.
int elfList_removeValueF(ElfList *list_p, void *data, void(*free_f)(void*)){
	if(!list_p) ELF_DIE("Received null pointer");
	if(!list_p->equal) ELF_DIE("List does not have equal() function");

	ElfList list = *list_p; //For efficiency
	Node *prev, *curr = list.first;
	
	prev = NULL; //previous node
	while(curr != NULL && list.greater(data, curr->key)){
		prev = curr;
		curr = curr->next;
	}
	
	//At this point, 'prev' holds the last node before the nodes that will be removed.
	int count = 0;
	Node *aux;
	while(curr != NULL && list.equal(curr->key, data)){
		count++;
		aux = curr;
		curr = curr->next;
		if(free_f) free_f(aux);
		list_p->size--;
	}
	
	//At this point, 'curr' holds the first node after the removed nodes.
	prev->next = curr;
	return count;
}

// Documented in header file.
int elfList_removeValue(ElfList *list, void *data){
	return elfList_removeValueF(list, data, NULL);
}

// Documented in header file.
ElfListIt *elfList_getIterator(ElfList *list){
	if(!list) ELF_DIE("Received null pointer");
	return list->first;
}

bool elfList_pointerToIntGreaterFunc(void *a, void *b){
	return ELF_POINTER_TO_INT(a) > ELF_POINTER_TO_INT(b) ? true : false;
}

bool elfList_pointerToUIntGreaterFunc(void *a, void *b){
	return ELF_POINTER_TO_UINT(a) > ELF_POINTER_TO_UINT(b) ? true : false;
}
