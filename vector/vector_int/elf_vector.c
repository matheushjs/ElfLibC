#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <elf_vector.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)
#define ELF_MAX(X,Y) ((X)>(Y)?X:Y)
#define ELF_MIN(X,Y) ((X)<(Y)?X:Y)
#define ELF_ABS(X) ((X)<0?-(X):X)
#define ELF_MEDIAN(X, Y, Z) (X<=Y?(X>=Z?X:(Z<=Y?Z:Y)):(Y>=Z?Y:(X<=Z?X:Z)))

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
	return ELF_MAX(pow,min);
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
	if(size < 0){
		fprintf(stderr, "Received negative size at %s.", __func__);
		size = 0;
	}

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
ElfVector *elfVector_new_random(int size, int min, int max){
	if(size < 0){
		fprintf(stderr, "Received negative size at %s.", __func__);
		size = 0;
	}

	ElfVector *new;
	static int salt = 0;

	new = malloc(sizeof(ElfVector));
	new->size = size;

	int capacity = nextpow2(size, INITIAL_CAPACITY);
	new->capacity = capacity;
	new->vector = malloc(sizeof(int) * capacity);

	srand(time(NULL)+salt);
	salt++;

	int i, delta;
	if(max < min){
		i = max;
		max = min;
		min = max;
	}
	
	delta = max-min+1;
	for(i = 0; i < size; i++)
		new->vector[i] = min + (rand()%delta);

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
	if(!elf) ELF_DIE("NULL pointer received!");

	int i, n;
	for(i = 0, n = elf->size; i < n; i++)
		printf("%d ", elf->vector[i]);
}

// Documented in header file.
int elfVector_size(const ElfVector *elf){
	if(!elf) ELF_DIE("NULL pointer received!");
	return elf->size;
}

// Documented in header file.
void elfVector_pushBack(ElfVector *elf, int value){
	if(!elf) ELF_DIE("NULL pointer received!");

	int idx = elf->size;
	if(idx == elf->capacity)
		elfVector_grow(elf);

	(elf->size)++;
	elf->vector[idx] = value;
}

// Documented in header file.
int elfVector_popBack(ElfVector *elf){
	if(!elf) ELF_DIE("NULL pointer received!");

	int retval, idx;
	
	idx = (elf->size -= 1);
	retval = elf->vector[idx];

	// Shrinks only if at 25% of capacity.
	if(elf->capacity != INITIAL_CAPACITY && idx == (elf->capacity >> 2))
		elfVector_shrink(elf);

	return retval;
}

// Documented in header file.
int elfVector_get(ElfVector *elf, int index){
	if(!elf) ELF_DIE("NULL pointer received!");

	if(index < 0 || index >= elf->size){
		fprintf(stderr, "Index out of bounds (%d) received at %s.\n", index, __func__);
		return 0;
	}

	return elf->vector[index];
}

// Documented in header file.
void elfVector_put(ElfVector *elf, int index, int value){
	if(!elf) ELF_DIE("NULL pointer received!");

	if(index < 0 || index >= elf->size){
		fprintf(stderr, "Index out of bounds (%d) received at %s.\n", index, __func__);
		return;
	}

	elf->vector[index] = value;
}

// Documented in header file.
void elfVector_maxmin(ElfVector *elf, int *max, int *min){
	if(!elf) ELF_DIE("NULL pointer received!");
	if(elf->size == 0){
		fprintf(stderr, "Can't get maxmin of an empty vector, at %s.\n", __func__);
		return;
	}

	int aux, i, n, ma, mi;

	ma = mi = elf->vector[0];
	for(i = 1, n = elf->size; i < n; i++){
		aux = elf->vector[i];
		if(aux > ma) ma = aux;
		else if(aux < mi) mi = aux;
	}

	if(max) *max = ma;
	if(min) *min = mi;
}

// Partition procedure of quicksort.
// Selects a pivot among the elemnts being sorted.
// Separate the vector in 2 parts:
// Part 1 has only elements lower than or equal to pivot.
// Part 2 has only elements higher than or equal to pivot.
static
void partition_ascend(int *vec, int left, int right, int *left_of_mid, int *right_of_mid){
	int pivot, aux;

	pivot = ELF_MEDIAN(vec[left], vec[right], vec[(left+right)/2]);
	
	while(left <= right){
		while(vec[left] < pivot) left++;
		while(vec[right] > pivot) right--;
		if(left > right) break;
		
		// Swap
		aux = vec[left];
		vec[left] = vec[right];
		vec[right] = aux;

		left++;
		right--;
	}

	// left/right inverted positions after applying the partition procedure.
	*left_of_mid = right;
	*right_of_mid = left;
}

// Descending equivalent of partition_ascend.
static
void partition_descend(int *vec, int left, int right, int *left_of_mid, int *right_of_mid){
	int pivot, aux;

	pivot = ELF_MEDIAN(vec[left], vec[right], vec[(left+right)/2]);
	
	while(left <= right){
		while(vec[left] > pivot) left++;
		while(vec[right] < pivot) right--;
		if(left > right) break;
		
		// Swap
		aux = vec[left];
		vec[left] = vec[right];
		vec[right] = aux;

		left++;
		right--;
	}

	// left/right inverted positions after applying the partition procedure.
	*left_of_mid = right;
	*right_of_mid = left;
}

// Recursive part of quicksort.
static
void quicksort_op_ascend(int *vec, int left, int right){
	int i, j;

	if(right <= left) return;
	partition_ascend(vec, left, right, &i, &j);
	quicksort_op_ascend(vec, left, i);
	quicksort_op_ascend(vec, j, right);
}

// Descending equivalent of quicksort_op_ascend.
static
void quicksort_op_descend(int *vec, int left, int right){
	int i, j;

	if(right <= left) return;
	partition_descend(vec, left, right, &i, &j);
	quicksort_op_descend(vec, left, i);
	quicksort_op_descend(vec, j, right);
}

// Documented in header file.
void elfVector_qsort_ascend(ElfVector *elf){
	if(!elf) ELF_DIE("NULL pointer received!");
	if(elf->size <= 1) return;

	quicksort_op_ascend(elf->vector, 0, elf->size - 1);
}

// Documented in header file.
void elfVector_qsort_descend(ElfVector *elf){
	if(!elf) ELF_DIE("NULL pointer received!");
	if(elf->size <= 1) return;

	quicksort_op_descend(elf->vector, 0, elf->size - 1);
}
