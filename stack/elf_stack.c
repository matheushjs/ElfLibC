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
#include <stdlib.h>
#include <stdio.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)

typedef struct _ElfStack ElfStack;
struct _ElfStack {
	void **array; //Array of void pointers.
	int size;
	int capacity;
};

static
void elfStack_grow(ElfStack *s){
	s->capacity = s->capacity << 1; //Doubles capacity
	s->array = realloc(s->array, sizeof(void *) * s->capacity);
}

static
void elfStack_shrink(ElfStack *s){
	s->capacity = s->capacity >> 1; //Halves capacity
	s->array = realloc(s->array, sizeof(void *) * s->capacity);
}

ElfStack *elfStack_new(){
	ElfStack *s = malloc(sizeof(ElfStack));
	s->capacity = 8; //Initial (and minimum) capacity.
	s->array = malloc(sizeof(void *) * s->capacity);
	s->size = 0;
	return s;
}

//Destroys the stack, doing nothing to the stored pointers.
void elfStack_destroy(ElfStack **s){
	if(*s != NULL){
		free( (*s)->array );
		free(*s);
		*s = NULL;
	}
}

//Destroys the stack, applying 'func' to each stored pointer.
void elfStack_destroy_f(ElfStack **s_pointer, void(*func)(void*)){
	ElfStack *s = *s_pointer;
	int i, n;
	if(s != NULL){
		for(i = 0, n = s->size; i < n; i++)
			func(s->array[i]);
		free( s->array );
		free(s);
		*s_pointer = NULL;
	}
}

//Inserts 'data' on the top of the stack
void elfStack_push(ElfStack *s, void *data){
	if(!s) ELF_DIE("Received NULL pointer");
	s->array[s->size] = data;
	(s->size)++;
	if(s->size == s->capacity)
		elfStack_grow(s);
}

//Removes and returns the top element of the stack.
//Returns NULL if stack is empty.
void *elfStack_pop(ElfStack *s){
	if(!s) ELF_DIE("Received NULL pointer");
	if(s->size == 0) return NULL;
	(s->size)--;
	void *ret = s->array[s->size];

	//Checks if stack should be shrunk.
	if(s->size > 8 && s->size == (s->capacity>>1) )
		elfStack_shrink(s);

	return ret;
}

//Returns the top element, without removing it.
//Returns NULL if stack is empty.
void *elfStack_top(const ElfStack *s){
	if(!s) ELF_DIE("Received NULL pointer");
	if(s->size == 0) return NULL;
	return s->array[s->size - 1];
}

int elfStack_size(const ElfStack *s){
	return s->size;
}

//Traverse the stack, from top to bottom, applying 'func' to each element.
void elfStack_traverse(const ElfStack *s, void(*func)(void*)){
	if(!s) ELF_DIE("Received NULL pointer");
	int i;
	for(i = s->size - 1; i >= 0; i--)
		func(s->array[i]);
}

//Traverse the stack, from bottom to top, applying 'func' to each element.
void elfStack_traverse_inv(const ElfStack *s, void(*func)(void*)){
	if(!s) ELF_DIE("Received NULL pointer");
	int i, n;
	for(i=0, n=s->size; i < n; i++)
		func(s->array[i]);
}
