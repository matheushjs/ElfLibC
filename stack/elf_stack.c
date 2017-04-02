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
void elf_stack_grow(ElfStack *s){
	s->capacity = s->capacity << 1; //Doubles capacity
	s->array = realloc(s->array, sizeof(void *) * s->capacity);
}

static
void elf_stack_shrink(ElfStack *s){
	s->capacity = s->capacity >> 1; //Halves capacity
	s->array = realloc(s->array, sizeof(void *) * s->capacity);
}

ElfStack *elf_stack_new(){
	ElfStack *s = malloc(sizeof(ElfStack));
	s->capacity = 8; //Initial (and minimum) capacity.
	s->array = malloc(sizeof(void *) * s->capacity);
	s->size = 0;
	return s;
}

//Destroys the stack, doing nothing to the stored pointers.
void elf_stack_destroy(ElfStack **s){
	if(*s != NULL){
		free( (*s)->array );
		free(*s);
		*s = NULL;
	}
}

//Destroys the stack, applying 'func' to each stored pointer.
void elf_stack_destroy_f(ElfStack **s_pointer, void(*func)(void*)){
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
void elf_stack_push(ElfStack *s, void *data){
	if(!s) ELF_DIE("Received NULL pointer");
	s->array[s->size] = data;
	(s->size)++;
	if(s->size == s->capacity)
		elf_stack_grow(s);
}

//Removes and returns the top element of the stack.
//Returns NULL if stack is empty.
void *elf_stack_pop(ElfStack *s){
	if(!s) ELF_DIE("Received NULL pointer");
	if(s->size == 0) return NULL;
	(s->size)--;
	void *ret = s->array[s->size];

	//Checks if stack should be shrunk.
	if(s->size > 8 && s->size == (s->capacity>>1) )
		elf_stack_shrink(s);

	return ret;
}

//Returns the top element, without removing it.
//Returns NULL if stack is empty.
void *elf_stack_top(const ElfStack *s){
	if(!s) ELF_DIE("Received NULL pointer");
	if(s->size == 0) return NULL;
	return s->array[s->size - 1];
}

int elf_stack_size(const ElfStack *s){
	return s->size;
}

//Traverse the stack, from top to bottom, applying 'func' to each element.
void elf_stack_traverse(const ElfStack *s, void(*func)(void*)){
	if(!s) ELF_DIE("Received NULL pointer");
	int i;
	for(i = s->size - 1; i >= 0; i--)
		func(s->array[i]);
}

//Traverse the stack, from bottom to top, applying 'func' to each element.
void elf_stack_traverse_inv(const ElfStack *s, void(*func)(void*)){
	if(!s) ELF_DIE("Received NULL pointer");
	int i, n;
	for(i=0, n=s->size; i < n; i++)
		func(s->array[i]);
}
