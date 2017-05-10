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

//List will be crescently ordered.
typedef struct _ElfList ElfList;
struct _ElfList {
	Node *first;
	int size;
};



static inline
Node *node_new(){
	return (Node *) calloc(sizeof(Node), 1);
}

// Documented in header file.
ElfList *elfList_new(){
	return calloc(sizeof(ElfList), 1);
}

// Documented in header file.
void elfList_insert(ElfList *list, int key){
	if(!list) ELF_DIE("Received null pointer");
	if(key == ELF_INVALID_INT) ELF_DIE("Attempted to insert ELF_INVALID_INT");
	
	Node *new = node_new();
	new->key = key;
	list->size++;

	Node *curr = list->first;
	Node *prev = NULL;
	while(curr != NULL && curr->key < key){
		prev = curr;
		curr = curr->next;
	}

	if(prev == NULL){
		new->next = list->first;
		list->first = new;
	} else {
		new->next = curr;
		prev->next = new;
	}
}

// Documented in header file.
bool elfList_insertUnique(ElfList *list, int key){
	if(!list) ELF_DIE("Received null pointer");
	if(key == ELF_INVALID_INT) ELF_DIE("Attempted to insert ELF_INVALID_INT");

	Node *new = node_new();
	new->key = key;

	Node *curr = list->first;
	Node *prev = NULL;
	while(curr != NULL && curr->key < key){
		prev = curr;
		curr = curr->next;
	}

	if(curr != NULL && curr->key == key){
		free(new);
		return false;
	} else if(prev == NULL){
		new->next = list->first;
		list->first = new;
	} else {
		new->next = curr;
		prev->next = new;
	}
	
	list->size++;
	return true;
}

// Documented in header file.
int elfList_indexOf(ElfList *list, int key){
	if(!list) ELF_DIE("Received null pointer");
	
	Node *curr = list->first;
	int count = 0;
	while(curr != NULL && curr->key < key){
		curr = curr->next;
		count++;
	}
	if(curr != NULL && curr->key == key)
		return count;
	else return -1;
}

// Documented in header file.
bool elfList_contains(ElfList *list, int key){
	if(elfList_indexOf(list, key) != -1)
		return true;
	else return false;
}

// Documented in header file.
int elfList_count(ElfList *list, int key){
	if(!list) ELF_DIE("Received null pointer");

	Node *curr = list->first;
	while(curr != NULL && curr->key != key)
		curr = curr->next;
	if(curr == NULL) return 0;
	
	int count = 0;
	while(curr != NULL && curr->key == key){
		curr = curr->next;
		count++;
	}
	return count;
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
int elfList_remove(ElfList *list, int index){
	int i;
	int key;
	
	if(!list) ELF_DIE("Received null pointer");
	if(index < 0 || index >= list->size) return ELF_INVALID_INT;

	Node *prev, *cur = list->first;

	prev = NULL;
	for(i = 0; i < index; i++){
		prev = cur;
		cur = cur->next;
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

	list->size--;
	return key;
}

// Documented in header file.
int elfList_size(const ElfList *list){
	if(!list) ELF_DIE("Received null pointer");
	return list->size;
}

// Documented in header file.
int elfList_get(const ElfList *list, int index){
	int i;

	if(!list) ELF_DIE("Received null pointer");
	if(index < 0 || index >= list->size) return ELF_INVALID_INT;
	
	Node *cur = list->first;

	for(i = 0; i < index; i++)
		cur = cur->next;

	return cur->key;
}

// Documented in header file.
void elfList_traverse(ElfList *list, void(*func)(int)){
	Node *cur;
	
	if(!list) ELF_DIE("Received null pointer");
	
	cur = list->first;
	while(cur != NULL){
		func(cur->key);
		cur = cur->next;
	}
}

// Documented in header file.
ElfListIt *elfList_getIterator(ElfList *list){
	if(!list) ELF_DIE("Received null pointer");
	return list->first;
}
