#ifndef _ELF_VECTOR_H
#define _ELF_VECTOR_H

typedef struct _ElfVector ElfVector;

ElfVector *elfVector_new();
ElfVector *elfVector_new_withValue(int size, int value);
ElfVector *elfVector_new_random(int size, int min, int max);

void elfVector_destroy(ElfVector **vec_p);
void elfVector_print(const ElfVector *elf);
int elfVector_size(const ElfVector *elf);
void elfVector_pushBack(ElfVector *elf, int value);
int elfVector_popBack(ElfVector *elf);
int elfVector_get(ElfVector *elf, int index);
void elfVector_put(ElfVector *elf, int index, int value);
void elfVector_maxmin(ElfVector *elf, int *max, int *min);

void elfVector_qsort_ascend(ElfVector *elf);
void elfVector_qsort_descend(ElfVector *elf);

/* DOCUMENTATION
 
typedef ElfVector;
	- Type that holds the vector structure.
 
ElfVector *elfVector_new();
	- Returns a new vector, containing no elements.

ElfVector *elfVector_new_withValue(int size, int value);
	- Returns a new vector, containing 'size' elemente with value 'value'.
	- If 'value' is 0, elements are initialized using 'calloc'.

ElfVector *elfVector_new_random(int size, int min, int max);
	- Returns a new vector, containing 'size' random elements.
	- Elements will be between 'min' (inclusive) and 'max' (inclusive).
	- Inverting 'min' with 'max' does not matter.

void elfVector_destroy(ElfVector **vec_p);
	- Frees memory allocated for the vector.

void elfVector_print(const ElfVector *elf);
	- Prints the vector contents separated by spaces.
	- Print is not ended with a newline.

int elfVector_size(ElfVector *elf);
	- Returns the number of elements in vector, currently.

void elfVector_pushBack(ElfVector *elf, int value);
	- Appends 'value' to the end of the vector.

int elfVector_popBack(ElfVector *elf);
	- Removes 'value' at the end of the vector, and returns it.
	- No checks for the size of the vector are done. Attempting to pop an element from an empty
	  vector has undefined behavior.

int elfVector_get(ElfVector *elf, int index);
	- Returns the element at position 'index' of the vector.
	- Returns 0 if 'index' is out of bounds, printing an error message.

void elfVector_put(ElfVector *elf, int index, int value);
	- Replaces with 'value' the value in position 'index'.
	- If 'index' is out of bounds, prints an error message. Vector is not modified.

void elfVector_maxmin(ElfVector *elf, int *max, int *min);
	- Finds the maximum and minimum numbers within the vector.
	- Returns on 'max' and 'min' if each is not NULL.

void elfVector_qsort_ascend(ElfVector *elf);
void elfVector_qsort_descend(ElfVector *elf);
	- Sorts elements of the vector.
	- Uses quicksort algorithm. Pivot is calculated by taking the median among the edge elements and the midle one,
	so it should be really hard to reach the worst case O(n^2) scenario.

 */

#endif
