#ifndef _ELF_VECTOR_H
#define _ELF_VECTOR_H

typedef struct _ElfVector ElfVector;

ElfVector *elfVector_new();
ElfVector *elfVector_new_withValue(int size, int value);
void elfVector_destroy(ElfVector **vec_p);

/* DOCUMENTATION
 
typedef ElfVector;
	- Type that holds the vector structure.
 
ElfVector *elfVector_new();
	- Returns a new vector, containing no elements.

ElfVector *elfVector_new_withValue(int size, int value);
	- Returns a new vector, containing 'size' elemente with value 'value'.
	- If 'value' is 0, elements are initialized using 'calloc'.

void elfVector_destroy(ElfVector **vec_p);
	- Frees memory allocated for the vector.

 */

#endif
