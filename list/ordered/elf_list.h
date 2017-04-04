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
void elf_list_insert(ElfList *list_p, void *data);
void elf_list_destroy(ElfList **list_p);
void elf_list_destroy_f(ElfList **list_p, void (*func)(void*));
void *elf_list_remove(ElfList *list, int index);
int elf_list_size(const ElfList *list);
void *elf_list_get(const ElfList *list, int index);
void elf_list_traverse(ElfList *list, void(*func)(void*));

/* Documentation

ELF_INT_TO_POINTER(i)
ELF_UINT_TO_POINTER(i)
ELF_POINTER_TO_INT(p)
ELF_POINTER_TO_UINT(p)
	- Macros for type casting from signed/unsigned integers to pointers.
	- Useful since this queue data structure can only store void pointers.

ElfList *elf_list_new(bool (*greaterThan)(void*,void*));
	- Returns a new list, using the function 'greaterThan' as ordering function.
	- If greaterThan is NOT reflexive, then duplicate elements will be
	  ordered by their insertion dates. Older elements appear first.

void elf_list_insert(ElfList *list_p, void *data);
	- Inserts 'data' into the list.

void elf_list_destroy(ElfList **list_p);
	- Destroys a list, doing nothing to the pointers stored in it.

void elf_list_destroy_f(ElfList **list_p, void (*func)(void*));
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

*/

#endif
