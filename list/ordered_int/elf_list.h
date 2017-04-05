#ifndef _ELF_LIST_H
#define _ELF_LIST_H

#include <limits.h>

#define ELF_INVALID_INT INT_MIN

typedef struct _ElfList ElfList;

ElfList *elf_list_new();
void elf_list_insert(ElfList *list_p, int data);
void elf_list_destroy(ElfList **list_p);
int elf_list_remove(ElfList *list, int index);
int elf_list_size(const ElfList *list);
int elf_list_get(const ElfList *list, int index);
void elf_list_traverse(ElfList *list, void(*func)(int));

typedef struct _ElfListIt ElfListIt;
struct _ElfListIt {
	int key;
	ElfListIt *next;
};

ElfListIt *elf_list_get_iterator(ElfList *list);

/* Documentation

ELF_INVALID_INT
	- Invalid integer used to sinnalize errors.

ElfList
	- Ordered list of integers
	- Ascending order
	- Accepts duplicates

ElfListIt
	- List iterator
	- Avoid using.

ElfList *elf_list_new();
	- Creates a new ordered list.

void elf_list_insert(ElfList *list_p, int data);
	- Inserts 'key' into the list.
	- It is forbidden to insert ELF_INVALID_INT (INT_MIN).

void elf_list_destroy(ElfList **list_p);
	- Destroys a list

int elf_list_remove(ElfList *list, int index);
	- Removes and returns the item at index 'index'.
	- If 'index' is out of range, nothing is done and ELF_INVALID_INT is returned.

int elf_list_size(const ElfList *list);
	- Returns the number of elements in the list.

int elf_list_get(const ElfList *list, int index);
	- Returns the element at index 'index'.
	- If 'index' is out of range, nothing is done and ELF_INVALID_INT is returned.

void elf_list_traverse(ElfList *list, void(*func)(void*));
	- Traverses all nodes in the list, applying 'func' in each stored integer.


ElfListIt
	- Iterator for the list structure.

ElfListIt *elf_list_get_iterator(ElfList *list);
	- Returns an iterator to the first element of list.
	- Iterators should not be freed.
	- Iterators become dangling if the origin list is modified.
	- Iterators become dangling if origin list is destroyed.

*/

#endif
