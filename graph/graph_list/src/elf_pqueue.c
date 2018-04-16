/*
 * MIT License
 * 
 * Copyright (c) 2018 Matheus H. J. Saldanha <mhjsaldanha@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include <elf_int_vector.h>
#include <elf_pqueue.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)

#define RIGHT(X)  ((2*X)+1)
#define LEFT(X)   ((2*X)+2)
#define PARENT(X) ((X-1)/2)

typedef struct _ElfPQueue {
	ElfIntVector *values;
	ElfIntVector *pr;
	bool maxFirst;
} ElfPQueue;

// Documented in header file.
ElfPQueue *elfPQueue_new_maxFirst(){
	ElfPQueue *new;
	new = malloc(sizeof(ElfPQueue));
	new->values = elfIntVector_new();
	new->pr = elfIntVector_new();
	new->maxFirst = true;

	return new;
}

// Documented in header file.
ElfPQueue *elfPQueue_new_minFirst(){
	ElfPQueue *new;
	new = malloc(sizeof(ElfPQueue));
	new->values = elfIntVector_new();
	new->pr = elfIntVector_new();
	new->maxFirst = false;

	return new;
}

// Documented in header file.
void elfPQueue_destroy(ElfPQueue **elf_p){
	if(elf_p){
		ElfPQueue *elf = *elf_p;

		elfIntVector_destroy(&elf->values);
		elfIntVector_destroy(&elf->pr);
		free(elf);
		*elf_p = NULL;
	}
}

// Documented in header file.
int elfPQueue_size(const ElfPQueue *elf){
	return elfIntVector_size(elf->values);
}

//Pops-up element in position 'index', until it's in correct position.
static
void elfPQueue_heapify_up(ElfPQueue *elf, int index){
	int parent_idx;
	int child, parent;

	while(index != 0){
		parent_idx = PARENT(index);
		parent = elfIntVector_get(elf->pr, parent_idx);
		child = elfIntVector_get(elf->pr, index);

		if(child > parent){
			elfIntVector_swap(elf->pr, parent_idx, index);
			elfIntVector_swap(elf->values, parent_idx, index);
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
		cur = elfIntVector_get(elf->pr, index);

		hi_child = index;
		if( RIGHT(index) < size
		&& (right = elfIntVector_get(elf->pr, RIGHT(index))) > cur){
			cur = right;
			hi_child = RIGHT(index);
		}
		if( LEFT(index) < size
		&& (left = elfIntVector_get(elf->pr, LEFT(index))) > cur){
			cur = left;
			hi_child = LEFT(index);
		}
		
		if(hi_child == index)
			break;

		elfIntVector_swap(elf->pr, hi_child, index);
		elfIntVector_swap(elf->values, hi_child, index);
		index = hi_child;
	}
}

// Documented in header file.
void elfPQueue_push(ElfPQueue *elf, int value, int priority){
	if(!elf) ELF_DIE("Received NULL pointer.");
	if(!elf->maxFirst)
		priority = -priority;

	elfIntVector_pushBack(elf->values, value);
	elfIntVector_pushBack(elf->pr, priority);
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

	elfIntVector_swap(elf->pr, 0, size-1);
	elfIntVector_swap(elf->values, 0, size-1);

	int prio  = elfIntVector_popBack(elf->pr);
	int value = elfIntVector_popBack(elf->values);

	if(size != 1) //Means size is 0 after popping.
		elfPQueue_heapify_down(elf, 0);

	if(priority)
		*priority = elf->maxFirst ? prio : -prio;
	return value;
}
