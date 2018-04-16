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
