#ifndef _ELF_SET_H
#define _ELF_SET_H

typedef struct _ElfSet ElfSet;

ElfSet *elfSet_new(int size);
void elfSet_destroy(ElfSet **set_p);
int elfSet_size(ElfSet *set);
int elfSet_find(ElfSet *set, int elem);
void elfSet_union(ElfSet *set, int e1, int e2);

/* DOCUMENTATION
 
typedef ElfSet
	- Structure that represents a set that uses Union-Find algorithms.
	- Elements stored are integers. Each element is characterized by it's set.
	- If the set holds N elements, they are mandatorily numbered from 0 to N-1.
 
ElfSet *elfSet_new(int size);
	- Returns a new set with 'size' elements.
	- Initially, each element constitutes its own set.

void elfSet_destroy(ElfSet **set_p);
	- Deallocates memory allocated for a set.

int elfSet_size(ElfSet *set);
	- Returns the number of elements in the set.

int elfSet_find(ElfSet *set, int elem);
	- Returns the identifier of the set that contains element 'elem'.
	- Complexity O(log(n)), but usually executed in O(1).

void elfSet_union(ElfSet *set, int e1, int e2);
	- Performs the union operation on 2 sets, each of which contain either 'e1' or 'e2'.
	- In case the elements belong to the same set, do nothing.
	- Complexity O(1).

*/

#endif
