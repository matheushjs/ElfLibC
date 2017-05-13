#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)
#define ELF_MAX(X,Y) ((X)>(Y)?X:Y)
#define ELF_MIN(X,Y) ((X)<(Y)?X:Y)

/* This vector structure starts with size 0.
 * User may only require operations to be done in elements already existent in the vector.
 * There may be no gaps in the vector, like inserting at position 10 when size is 5, creating 4 gaps.
 * User may grow the vector linearly, though.
 * Insertion may only be done in elements that already exist.
 */

#define INITIAL_CAPACITY 16

typedef struct _ElfVector {
	int *vector;
	int capacity;
	int size;
} ElfVector;

// Finds the lowest power of 2 that is higher than 'num'.
// Returns the minimum between the lowest power of 2 and 'min'.
static inline
int nextpow2(int num, int min){
	int pow = 1;
	while(pow < num)
		pow <<= 1;
	return ELF_MIN(pow,min);
}

// Grows the vector once
static inline
void elfVector_grow(ElfVector *elf){
	int capacity, *newvec;

	capacity = elf->capacity << 1;
	newvec = malloc(sizeof(int) * capacity);

	memcpy(newvec, elf->vector, sizeof(int) * elf->capacity);
	free(elf->vector);

	elf->vector = newvec;
	elf->capacity = capacity;
}

// Shrinks the vector once
static inline
void elfVector_shrink(ElfVector *elf){
	int capacity, *newvec;

	capacity = elf->capacity >> 1;
	newvec = malloc(sizeof(int) * capacity);

	memcpy(newvec, elf->vector, sizeof(int) * capacity);
	free(elf->vector);
	
	elf->vector = newvec;
	elf->capacity = capacity;
}

// Documented in header file.
ElfVector *elfVector_new(){
	ElfVector *new;
	new = malloc(sizeof(ElfVector));
	new->size = 0;
	new->capacity = INITIAL_CAPACITY;
	new->vector = malloc(sizeof(int) * INITIAL_CAPACITY);
	return new;
}

// Documented in header file.
ElfVector *elfVector_new_withValue(int size, int value){
	ElfVector *new;
	new = malloc(sizeof(ElfVector));
	new->size = size;

	int capacity = nextpow2(size, INITIAL_CAPACITY);
	new->capacity = capacity;
	if(value == 0){
		new->vector = calloc(sizeof(int), capacity);
	} else {
		int i;
		new->vector = malloc(sizeof(int) * capacity);
		for(i = 0; i < size; i++)
			new->vector[i] = value;
	}

	return new;
}

// Documented in header file.
void elfVector_destroy(ElfVector **vec_p){
	if(vec_p){
		ElfVector *vec = *vec_p;
		free(vec->vector);
		free(vec);
		*vec_p = NULL;
	}
}

// Documented in header file.
void elfVector_print(const ElfVector *elf){
	int i, n;
	for(i = 0, n = elf->size; i < n; i++)
		printf("%d ", elf->vector[i]);
}
