#ifndef _ELF_VECTOR_H
#define _ELF_VECTOR_H

typedef struct _ElfVector ElfVector;

ElfVector *elfVector_new();
ElfVector *elfVector_new_withValue(int size, int value);
ElfVector *elfVector_new_fromArray(int **array, int size);
ElfVector *elfVector_new_random(int size, int min, int max);
ElfVector *elfVector_new_fromOther(const ElfVector *elf);

void elfVector_destroy(ElfVector **vec_p);
void elfVector_print(const ElfVector *elf);
int elfVector_size(const ElfVector *elf);
void elfVector_pushBack(ElfVector *elf, int value);
int elfVector_popBack(ElfVector *elf);
int elfVector_get(ElfVector *elf, int index);
void elfVector_put(ElfVector *elf, int index, int value);
void elfVector_swap(ElfVector *elf, int i, int j);
void elfVector_maxmin(ElfVector *elf, int *max, int *min);

void elfVector_qsort_ascend(ElfVector *elf);
void elfVector_qsort_descend(ElfVector *elf);
ElfVector *elfVector_qsort_ascendWithIndexes(ElfVector *elf);
ElfVector *elfVector_qsort_descendWithIndexes(ElfVector *elf);

int elfVector_back(const ElfVector *elf);
int elfVector_front(const ElfVector *elf);
int elfVector_search(const ElfVector *elf, int value);
int elfVector_count(const ElfVector *elf, int value);

int elfVector_search_sorted(const ElfVector *elf, int value);
int elfVector_count_sorted(const ElfVector *elf, int value);

void elfVector_insert(ElfVector *elf, int index, int value);
int elfVector_remove(ElfVector *elf, int index);


/* DOCUMENTATION
 
typedef ElfVector;
	- Type that holds the vector structure.
 
ElfVector *elfVector_new();
	- Returns a new vector, containing no elements.

ElfVector *elfVector_new_withValue(int size, int value);
	- Returns a new vector, containing 'size' elemente with value 'value'.
	- If 'value' is 0, elements are initialized using 'calloc'.

ElfVector *elfVector_new_fromArray(int **array, int size);
	- Returns a vector constructed with elements in 'array'.
	- 'array' should be a pointer to a dynamically allocated array of integers.
	- The pointer to int* is stolen, then set to NULL.

ElfVector *elfVector_new_random(int size, int min, int max);
	- Returns a new vector, containing 'size' random elements.
	- Elements will be between 'min' (inclusive) and 'max' (inclusive).
	- Inverting 'min' with 'max' does not matter.

ElfVector *elfVector_new_fromOther(const ElfVector *elf);
	- Returns a new vector, which is a copy of 'elf'.

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

void elfVector_swap(ElfVector *elf, int i, int j);
	- Swaps values 'i' and 'j' in the vector.
	- For efficiency, no checks are made regarding pointer/index validity.

void elfVector_maxmin(ElfVector *elf, int *max, int *min);
	- Finds the maximum and minimum numbers within the vector.
	- Returns on 'max' and 'min' if each is not NULL.

void elfVector_qsort_ascend(ElfVector *elf);
void elfVector_qsort_descend(ElfVector *elf);
	- Sorts elements of the vector.
	- Uses quicksort algorithm. Pivot is calculated by taking the median among the edge elements and the midle one,
	so it should be really hard to reach the worst case O(n^2) scenario.

ElfVector *elfVector_qsort_ascendWithIndexes(ElfVector *elf);
ElfVector *elfVector_qsort_descendWithIndexes(ElfVector *elf);
	- Sorts elements of the vector, as the other _qsort_ functions.
	- Returns a vector containing the index of elements prior to the sorting.
	- Example:
		- Before sorting
			Vector: 1 0 4 3
			Indexes: 0 1 2 3
		
		- After sorting (Elements in Indexes are swapped in unison with elements in Vector)
			Vector: 0 1 3 4
			Indexes: 1 0 3 2
	- WARNING: Sorting is UNSTABLE. Order among elements of same value is not maintained.

int elfVector_back(const ElfVector *elf);
	- Returns the last element of the vector, without removing it.
	- No checks regarding existence of the element are done. 

int elfVector_front(const ElfVector *elf);
	- Returns the first element of the vector, without removing it.
	- No checks regarding existence of the element are done. 

int elfVector_search(const ElfVector *elf, int value);
	- Searches for 'value' within the vector.
	- Sequential search.
	- Returns:
		The index of first 'value' found, if it exists.
		-1 if vector does not contain 'value'.

int elfVector_count(const ElfVector *elf, int value);
	- Counts how many times 'value' appear in the vector.
	- Sequential search.
	- Returns the counting.

int elfVector_search_sorted(const ElfVector *elf, int value);
	- Searches for 'value' within the vector, which should be sorted (crescently or not, doesn't matter).
	- If the vector is not sorted, behavior is undefined.
	- Binary search.
	- Returns:
		The index of first 'value' found, if it exists.
		-1 if vector does not contain 'value'.

int elfVector_count_sorted(const ElfVector *elf, int value);
	- Counts how many times 'value' appear in the vector, which should be sorted (crescently or not, doesn't matter).
	- If the vector is not sorted, behavior is undefined.
	- Binary search.
	- Returns the counting.

void elfVector_insert(ElfVector *elf, int index, int value);
	- Inserts 'value' behind the value in position 'index'.
	- The inserted value then becomes the value in position 'index'.
	  A warning message is printed if index is out of bounds.
	- The given 'index' must be within the vector bounds [0, size-1].
	- Expensive operation. Consider using lists if inserting is done frequently.

int elfVector_remove(ElfVector *elf, int index);
	- Removes the value in position 'index', shifting all following elements to the left.
	- The given 'index' must be within the vector bounds [0, size-1].
	  A warning message is printed if index is out of bounds.
	- Expensive operation. Consider using lists if removal is done frequently.
	- Return:
		the removed value
		0 if index is out of bounds

*/

#endif
