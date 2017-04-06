#ifndef _ELF_LIST_H
#define _ELF_LIST_H

#include <stdbool.h>

#define ELF_INT_TO_POINTER(i) ((void *) (long int) (i))
#define ELF_UINT_TO_POINTER(i) ((void *) (unsigned long int) (i))
#define ELF_POINTER_TO_INT(p) ((int) (long int) (p))
#define ELF_POINTER_TO_UINT(p) ((unsigned int) (unsigned long int) (p))
//Credits: https://developer.gnome.org/glib/stable/glib-Type-Conversion-Macros.html

typedef struct _ElfList ElfList;

ElfList *elf_list_new(bool (*greaterThan)(void*,void*));
ElfList *elf_list_newWithEqual(bool (*greaterThan)(void*,void*), bool (*equal)(void*,void*));

/* Functions accepted by any list */
void elf_list_insert(ElfList *list_p, void *data);
void elf_list_destroy(ElfList **list_p);
void elf_list_destroyF(ElfList **list_p, void (*func)(void*));
void *elf_list_remove(ElfList *list, int index);
int elf_list_size(const ElfList *list);
void *elf_list_get(const ElfList *list, int index);
void elf_list_traverse(ElfList *list, void(*func)(void*));

/* Functions accepted only by lists with an equality function */
bool elf_list_insertUnique(ElfList *list_p, void *data);
int elf_list_indexOf(ElfList *list_p, void *data);
bool elf_list_contains(ElfList *list, void *data);
int elf_list_count(ElfList *list_p, void *data);


/* Documentation

ELF_INT_TO_POINTER(i)
ELF_UINT_TO_POINTER(i)
ELF_POINTER_TO_INT(p)
ELF_POINTER_TO_UINT(p)
	- Macros for type casting from signed/unsigned integers to pointers.
	- Useful since this queue data structure can only store void pointers.

ElfList
	- Ordered list of void*
	- Ascending order
	- Accepts duplicates
	- In case of duplicates, older pointers comes first

ElfList *elf_list_new(bool (*greaterThan)(void*,void*));
	- Returns a new list, using the function 'greaterThan' as ordering function.
	- greaterThan SHOULD NOT BE REFLEXIVE. This means a > b means b > a is FALSE.
	- By not having 'equal', some operations are not supported.

ElfList *elf_list_newWithEqual(bool (*greaterThan)(void*,void*), bool (*equal)(void*,void*));
	- Returns a new list, using the function 'greaterThan' as ordering function,
	  and 'equal' as equality function.
	- greaterThan SHOULD NOT BE REFLEXIVE. This means a > b means b > a is FALSE.

void elf_list_insert(ElfList *list_p, void *data);
	- Inserts 'data' into the list.

void elf_list_destroy(ElfList **list_p);
	- Destroys a list, doing nothing to the pointers stored in it.

void elf_list_destroyF(ElfList **list_p, void (*func)(void*));
	- Destroys a list, using function 'func' to free the stored pointers.

void *elf_list_remove(ElfList *list, int index);
	- Removes and returns the item at index 'index'.
	- If 'index' is out of range, nothing is done and NULL is returned.

int elf_list_size(const ElfList *list);
	- Returns the number of elements in the list.

void *elf_list_get(const ElfList *list, int index);
	- Returns the element at index 'index'.
	- If 'index' is out of range, returns NULL.

void elf_list_traverse(ElfList *list, void(*func)(void*));
	- Traverses all nodes in the list, applying 'func' in each stored pointer.

bool elf_list_insertUnique(ElfList *list_p, void *data);
	- Inserts 'data' into the list if it's not a duplicate.
	- If list doesn't have an 'equal' function, exits the program.
	- Returns true is 'data' is unique, false if it's a duplicate.

int elf_list_indexOf(ElfList *list_p, void *data);
	- If list contains 'data', return its index. Return -1 otherwise.

bool elf_list_contains(ElfList *list, void *data);
	- Returns true if the list contains 'data'.

int elf_list_count(ElfList *list_p, void *data);
	- Returns the number of elements 'data' within the list.

*/

#endif
