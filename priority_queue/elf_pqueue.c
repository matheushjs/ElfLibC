#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include <elf_vector.h>
#include <elf_pqueue.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)

#define RIGHT(X)  ((2*X)+1)
#define LEFT(X)   ((2*X)+2)
#define PARENT(X) ((X-1)/2)

typedef struct _ElfPQueue {
	ElfVector *values;
	ElfVector *pr;
	bool maxFirst;
} ElfPQueue;

// Documented in header file.
ElfPQueue *elfPQueue_new_maxFirst(){
	ElfPQueue *new;
	new = malloc(sizeof(ElfPQueue));
	new->values = elfVector_new();
	new->pr = elfVector_new();
	new->maxFirst = true;

	return new;
}

// Documented in header file.
ElfPQueue *elfPQueue_new_minFirst(){
	ElfPQueue *new;
	new = malloc(sizeof(ElfPQueue));
	new->values = elfVector_new();
	new->pr = elfVector_new();
	new->maxFirst = false;

	return new;
}

// Documented in header file.
void elfPQueue_destroy(ElfPQueue **elf_p){
	if(elf_p){
		ElfPQueue *elf = *elf_p;

		elfVector_destroy(&elf->values);
		elfVector_destroy(&elf->pr);
		free(elf);
		*elf_p = NULL;
	}
}

// Documented in header file.
int elfPQueue_size(const ElfPQueue *elf){
	return elfVector_size(elf->values);
}

//Pops-up element in position 'index', until it's in correct position.
static
void elfPQueue_heapify_up(ElfPQueue *elf, int index){
	int parent_idx;
	int child, parent;

	while(index != 0){
		parent_idx = PARENT(index);
		parent = elfVector_get(elf->pr, parent_idx);
		child = elfVector_get(elf->pr, index);

		if(child > parent){
			elfVector_swap(elf->pr, parent_idx, index);
			elfVector_swap(elf->values, parent_idx, index);
			index = parent_idx;
		} else break;
	}
}

//Pops-down element in position 'index', until it's in correct position.
static
void elfPQueue_heapify_down(ElfPQueue *elf, int index){
	int size, hi_child;
	int cur, right, left;

	size = elfPQueue_size(elf);
	while(true){
		cur = elfVector_get(elf->pr, index);

		hi_child = index;
		if( RIGHT(index) < size
		&& (right = elfVector_get(elf->pr, RIGHT(index))) > cur){
			cur = right;
			hi_child = RIGHT(index);
		}
		if( LEFT(index) < size
		&& (left = elfVector_get(elf->pr, LEFT(index))) > cur){
			cur = left;
			hi_child = LEFT(index);
		}
		
		if(hi_child == index)
			break;

		elfVector_swap(elf->pr, hi_child, index);
		elfVector_swap(elf->values, hi_child, index);
		index = hi_child;
	}
}

// Documented in header file.
void elfPQueue_push(ElfPQueue *elf, int value, int priority){
	if(!elf) ELF_DIE("Received NULL pointer.");
	if(!elf->maxFirst)
		priority = -priority;

	elfVector_pushBack(elf->values, value);
	elfVector_pushBack(elf->pr, priority);
	elfPQueue_heapify_up(elf, elfPQueue_size(elf) - 1);
}

// Documented in header file.
int elfPQueue_pop(ElfPQueue *elf, int *priority){
	if(!elf) ELF_DIE("Received NULL pointer.");
	
	int size = elfPQueue_size(elf);
	if(size == 0){
		fprintf(stderr, "Attempted to pop from empty queue at '%s'.\n", __func__);
		return INT_MIN;
	}

	elfVector_swap(elf->pr, 0, size-1);
	elfVector_swap(elf->values, 0, size-1);

	int prio  = elfVector_popBack(elf->pr);
	int value = elfVector_popBack(elf->values);

	if(size != 1) //Means size is 0 after popping.
		elfPQueue_heapify_down(elf, 0);

	if(priority)
		*priority = elf->maxFirst ? prio : -prio;
	return value;
}