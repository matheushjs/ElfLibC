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
#ifndef _ELF_VECTOR_H
#define _ELF_VECTOR_H

typedef struct _ElfIntVector ElfIntVector;

ElfIntVector *elfIntVector_new();
ElfIntVector *elfIntVector_new_withValue(int size, int value);
ElfIntVector *elfIntVector_new_fromArray(int **array, int size);
ElfIntVector *elfIntVector_new_random(int size, int min, int max);
ElfIntVector *elfIntVector_new_fromOther(const ElfIntVector *elf);

void elfIntVector_destroy(ElfIntVector **vec_p);
void elfIntVector_print(const ElfIntVector *elf);
int elfIntVector_size(const ElfIntVector *elf);
void elfIntVector_pushBack(ElfIntVector *elf, int value);
int elfIntVector_popBack(ElfIntVector *elf);
int elfIntVector_get(ElfIntVector *elf, int index);
void elfIntVector_put(ElfIntVector *elf, int index, int value);
void elfIntVector_swap(ElfIntVector *elf, int i, int j);
void elfIntVector_maxmin(ElfIntVector *elf, int *max, int *min);

void elfIntVector_qsort_ascend(ElfIntVector *elf);
void elfIntVector_qsort_descend(ElfIntVector *elf);
ElfIntVector *elfIntVector_qsort_ascendWithIndexes(ElfIntVector *elf);
ElfIntVector *elfIntVector_qsort_descendWithIndexes(ElfIntVector *elf);

int elfIntVector_back(const ElfIntVector *elf);
int elfIntVector_front(const ElfIntVector *elf);
int elfIntVector_search(const ElfIntVector *elf, int value);
int elfIntVector_count(const ElfIntVector *elf, int value);

int elfIntVector_search_sorted(const ElfIntVector *elf, int value);
int elfIntVector_count_sorted(const ElfIntVector *elf, int value);

void elfIntVector_insert(ElfIntVector *elf, int index, int value);
int elfIntVector_remove(ElfIntVector *elf, int index);

int *elfIntVector_getData(ElfIntVector *elf);


/* DOCUMENTATION
 
typedef ElfIntVector;
	- Type that holds the vector structure.
 
ElfIntVector *elfIntVector_new();
	- Returns a new vector, containing no elements.

ElfIntVector *elfIntVector_new_withValue(int size, int value);
	- Returns a new vector, containing 'size' elemente with value 'value'.
	- If 'value' is 0, elements are initialized using 'calloc'.

ElfIntVector *elfIntVector_new_fromArray(int **array, int size);
	- Returns a vector constructed with elements in 'array'.
	- 'array' should be a pointer to a dynamically allocated array of integers.
	- The pointer to int* is stolen, then set to NULL.

ElfIntVector *elfIntVector_new_random(int size, int min, int max);
	- Returns a new vector, containing 'size' random elements.
	- Elements will be between 'min' (inclusive) and 'max' (inclusive).
	- Inverting 'min' with 'max' does not matter.

ElfIntVector *elfIntVector_new_fromOther(const ElfIntVector *elf);
	- Returns a new vector, which is a copy of 'elf'.

void elfIntVector_destroy(ElfIntVector **vec_p);
	- Frees memory allocated for the vector.

void elfIntVector_print(const ElfIntVector *elf);
	- Prints the vector contents separated by spaces.
	- Print is not ended with a newline.

int elfIntVector_size(ElfIntVector *elf);
	- Returns the number of elements in vector, currently.

void elfIntVector_pushBack(ElfIntVector *elf, int value);
	- Appends 'value' to the end of the vector.

int elfIntVector_popBack(ElfIntVector *elf);
	- Removes 'value' at the end of the vector, and returns it.
	- No checks for the size of the vector are done. Attempting to pop an element from an empty
	  vector has undefined behavior.

int elfIntVector_get(ElfIntVector *elf, int index);
	- Returns the element at position 'index' of the vector.
	- Returns 0 if 'index' is out of bounds, printing an error message.

void elfIntVector_put(ElfIntVector *elf, int index, int value);
	- Replaces with 'value' the value in position 'index'.
	- If 'index' is out of bounds, prints an error message. Vector is not modified.

void elfIntVector_swap(ElfIntVector *elf, int i, int j);
	- Swaps values 'i' and 'j' in the vector.
	- For efficiency, no checks are made regarding pointer/index validity.

void elfIntVector_maxmin(ElfIntVector *elf, int *max, int *min);
	- Finds the maximum and minimum numbers within the vector.
	- Returns on 'max' and 'min' if each is not NULL.

void elfIntVector_qsort_ascend(ElfIntVector *elf);
void elfIntVector_qsort_descend(ElfIntVector *elf);
	- Sorts elements of the vector.
	- Uses quicksort algorithm. Pivot is calculated by taking the median among the edge elements and the midle one,
	so it should be really hard to reach the worst case O(n^2) scenario.

ElfIntVector *elfIntVector_qsort_ascendWithIndexes(ElfIntVector *elf);
ElfIntVector *elfIntVector_qsort_descendWithIndexes(ElfIntVector *elf);
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

int elfIntVector_back(const ElfIntVector *elf);
	- Returns the last element of the vector, without removing it.
	- No checks regarding existence of the element are done. 

int elfIntVector_front(const ElfIntVector *elf);
	- Returns the first element of the vector, without removing it.
	- No checks regarding existence of the element are done. 

int elfIntVector_search(const ElfIntVector *elf, int value);
	- Searches for 'value' within the vector.
	- Sequential search.
	- Returns:
		The index of first 'value' found, if it exists.
		-1 if vector does not contain 'value'.

int elfIntVector_count(const ElfIntVector *elf, int value);
	- Counts how many times 'value' appear in the vector.
	- Sequential search.
	- Returns the counting.

int elfIntVector_search_sorted(const ElfIntVector *elf, int value);
	- Searches for 'value' within the vector, which should be sorted (crescently or not, doesn't matter).
	- If the vector is not sorted, behavior is undefined.
	- Binary search.
	- Returns:
		The index of first 'value' found, if it exists.
		-1 if vector does not contain 'value'.

int elfIntVector_count_sorted(const ElfIntVector *elf, int value);
	- Counts how many times 'value' appear in the vector, which should be sorted (crescently or not, doesn't matter).
	- If the vector is not sorted, behavior is undefined.
	- Binary search.
	- Returns the counting.

void elfIntVector_insert(ElfIntVector *elf, int index, int value);
	- Inserts 'value' behind the value in position 'index'.
	- The inserted value then becomes the value in position 'index'.
	  A warning message is printed if index is out of bounds.
	- The given 'index' must be within the vector bounds [0, size-1].
	- Expensive operation. Consider using lists if inserting is done frequently.

int elfIntVector_remove(ElfIntVector *elf, int index);
	- Removes the value in position 'index', shifting all following elements to the left.
	- The given 'index' must be within the vector bounds [0, size-1].
	  A warning message is printed if index is out of bounds.
	- Expensive operation. Consider using lists if removal is done frequently.
	- Return:
		the removed value
		0 if index is out of bounds

int *elfIntVector_getData(ElfIntVector *elf);
	- Returns the inner array of the vector.
	- Elements may be modified, but not removed/inserted.
*/

#endif
