#ifndef _ELF_LIST_H
#define _ELF_LIST_H

#include <limits.h>
#include <stdbool.h>

#define ELF_INVALID_INT INT_MIN

typedef struct _ElfIntList ElfIntList;

ElfIntList *elfIntList_new();
void elfIntList_insert(ElfIntList *list_p, int data);
bool elfIntList_insertUnique(ElfIntList *list, int key);
int elfIntList_indexOf(ElfIntList *list, int key);
bool elfIntList_contains(ElfIntList *list, int key);
int elfIntList_count(ElfIntList *list, int key);
void elfIntList_destroy(ElfIntList **list_p);
int elfIntList_remove(ElfIntList *list, int index);
int elfIntList_size(const ElfIntList *list);
int elfIntList_get(const ElfIntList *list, int index);
void elfIntList_traverse(ElfIntList *list, void(*func)(int));

typedef struct _ElfIntListIt ElfIntListIt;
struct _ElfIntListIt {
	int key;
	ElfIntListIt *next;
};
ElfIntListIt *elfIntList_getIterator(ElfIntList *list);

/* Documentation

ELF_INVALID_INT
	- Invalid integer used to sinnalize errors.

ElfIntList
	- Ordered list of integers
	- Ascending order
	- Accepts duplicates

ElfIntList *elfIntList_new();
	- Creates a new ordered list.

void elfIntList_insert(ElfIntList *list_p, int data);
	- Inserts 'key' into the list.
	- It is forbidden to insert ELF_INVALID_INT (INT_MIN).

bool elfIntList_insertUnique(ElfIntList *list, int key){
	- Inserts 'key' into the list if it's not a duplicate.
	- If it is a duplicate, return false (fail). Else return true.
	- It is forbidden to insert ELF_INVALID_INT.

int elfIntList_indexOf(ElfIntList *list, int key);
	- If 'key' exists within list, return the index of the first occurence.
	- Return -1 otherwise.

bool elfIntList_contains(ElfIntList *list, int key);
	- Returns 'true' if 'key' exists within list.

int elfIntList_count(ElfIntList *list, int key);
	- Returns the amount of times the value 'key' appears in the list.

void elfIntList_destroy(ElfIntList **list_p);
	- Destroys a list

int elfIntList_remove(ElfIntList *list, int index);
	- Removes and returns the item at index 'index'.
	- If 'index' is out of range, nothing is done and ELF_INVALID_INT is returned.

int elfIntList_size(const ElfIntList *list);
	- Returns the number of elements in the list.

int elfIntList_get(const ElfIntList *list, int index);
	- Returns the element at index 'index'.
	- If 'index' is out of range, nothing is done and ELF_INVALID_INT is returned.

void elfIntList_traverse(ElfIntList *list, void(*func)(void*));
	- Traverses all nodes in the list, applying 'func' in each stored integer.


ElfIntListIt
	- Iterator for the list structure.
	- Avoid using.

ElfIntListIt *elfIntList_getIterator(ElfIntList *list);
	- Returns an iterator to the first element of list.
	- Iterators should not be freed.
	- Nodes pointed by iterators shouldn't be modified.
	- The key in the Node shouldn't be modified.
	- Iterators become dangling if the origin list is modified.
	- Iterators become dangling if origin list is destroyed.

*/

#endif
