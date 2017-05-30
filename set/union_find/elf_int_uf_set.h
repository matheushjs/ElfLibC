#ifndef _ELF_SET_H
#define _ELF_SET_H

typedef struct _ElfIntUFSet ElfIntUFSet;

ElfIntUFSet *elfIntUFSet_new(int size);
void elfIntUFSet_destroy(ElfIntUFSet **set_p);
int elfIntUFSet_size(ElfIntUFSet *set);
int elfIntUFSet_find(ElfIntUFSet *set, int elem);
void elfIntUFSet_union(ElfIntUFSet *set, int e1, int e2);

/* DOCUMENTATION
 
typedef ElfIntUFSet
	- Elf Integer Union-Find set
	- Structure that represents a set that uses Union-Find algorithms.
	- Elements stored are integers. Each element is characterized by it's set.
	- When the user instantiates a new set of size N, the set contains N elements,
	    numbered from 0 to N-1, which corresponds to their indexes in the array of 
	    elements. Each element in this array is characterized by its set number, and
	    each element begins as being in their own set (so element 5 starts on set 5).
	- What matters here is that all elements start in different sets, and the user
	    can apply the union() procedure on 2 elements so that they are contained in the
	    same set. 2 elements A and B are contained in the same set if, and only if,
	    _find(A) == _find(B). This is how the user should check if 2 elements are in the
	    same set.
 
ElfIntUFSet *elfIntUFSet_new(int size);
	- Returns a new set with 'size' elements.
	- Initially, each element constitutes its own set.

void elfIntUFSet_destroy(ElfIntUFSet **set_p);
	- Deallocates memory allocated for a set.

int elfIntUFSet_size(ElfIntUFSet *set);
	- Returns the number of elements in the set.

int elfIntUFSet_find(ElfIntUFSet *set, int elem);
	- Returns the identifier of the set that contains element 'elem'.
	- Complexity O(log(n)), but usually executed in O(1).

void elfIntUFSet_union(ElfIntUFSet *set, int e1, int e2);
	- Performs the union operation on 2 sets, each of which contain either 'e1' or 'e2'.
	- In case the elements belong to the same set, do nothing.
	- Complexity O(1).

*/

#endif
