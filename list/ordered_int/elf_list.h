#ifndef _ELF_LIST_H
#define _ELF_LIST_H

#include <limits.h>
#include <stdbool.h>

#define ELF_INVALID_INT INT_MIN

typedef struct _ElfList ElfList;

ElfList *elf_list_new();
void elf_list_insert(ElfList *list_p, int data);
bool elf_list_insertUnique(ElfList *list, int key);
int elf_list_indexOf(ElfList *list, int key);
bool elf_list_contains(ElfList *list, int key);
int elf_list_count(ElfList *list, int key);
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
ElfListIt *elf_list_getIterator(ElfList *list);

/* Documentation

ELF_INVALID_INT
	- Invalid integer used to sinnalize errors.

ElfList
	- Ordered list of integers
	- Ascending order
	- Accepts duplicates

ElfList *elf_list_new();
	- Creates a new ordered list.

void elf_list_insert(ElfList *list_p, int data);
	- Inserts 'key' into the list.
	- It is forbidden to insert ELF_INVALID_INT (INT_MIN).

bool elf_list_insertUnique(ElfList *list, int key){
	- Inserts 'key' into the list if it's not a duplicate.
	- If it is a duplicate, return false (fail). Else return true.
	- It is forbidden to insert ELF_INVALID_INT.

int elf_list_indexOf(ElfList *list, int key);
	- If 'key' exists within list, return the index of the first occurence.
	- Return -1 otherwise.

bool elf_list_contains(ElfList *list, int key);
	- Returns 'true' if 'key' exists within list.

int elf_list_count(ElfList *list, int key);
	- Returns the amount of times the value 'key' appears in the list.

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
	- Avoid using.

ElfListIt *elf_list_getIterator(ElfList *list);
	- Returns an iterator to the first element of list.
	- Iterators should not be freed.
	- Nodes pointed by iterators shouldn't be modified.
	- The key in the Node shouldn't be modified.
	- Iterators become dangling if the origin list is modified.
	- Iterators become dangling if origin list is destroyed.

*/

#endif
