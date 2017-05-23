#ifndef _ELF_PQUEUE_H
#define _ELF_PQUEUE_H

#include <elf_int_vector.h>

typedef struct _ElfPQueue ElfPQueue;

ElfPQueue *elfPQueue_new_maxFirst();
ElfPQueue *elfPQueue_new_minFirst();
void elfPQueue_destroy(ElfPQueue **elf_p);
int elfPQueue_size(const ElfPQueue *elf);
void elfPQueue_push(ElfPQueue *elf, int value, int priority);
int elfPQueue_pop(ElfPQueue *elf, int *priority);

/* DOCUMENTATION

typedef ElfPQueue;
	- Structure that represents a priority queue.
	- User may add pairs (value, priority) of integers to the queue.
	- Elements are popped by max/min priority first.

ElfPQueue *elfPQueue_new_maxFirst();
	- Returns a priority queue where elements of max priority are popped first.

ElfPQueue *elfPQueue_new_minFirst();
	- Returns a priority queue where elements of min priority are popped first.

void elfPQueue_destroy(ElfPQueue **elf_p);
	- Frees memory allocated to the given priority queue.

int elfPQueue_size(const ElfPQueue *elf);
	- Returns the number of elements within the queue.

void elfPQueue_push(ElfPQueue *elf, int value, int priority);
	- Inserts a pair (value, priority) of integers to the priority queue.

int elfPQueue_pop(ElfPQueue *elf, int *priority);
	- Pops a (value) from the queue, returning it.
	- If 'priority' is not NULL, it receives the priority of the popped value.
*/

#endif
