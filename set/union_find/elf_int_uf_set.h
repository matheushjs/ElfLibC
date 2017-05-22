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
	- Structure that represents a set that uses Union-Find algorithms.
	- Elements stored are integers. Each element is characterized by it's set.
	- If the set holds N elements, they are mandatorily numbered from 0 to N-1.
 
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
