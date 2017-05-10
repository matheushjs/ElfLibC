#ifndef _ELF_LIST_H
#define _ELF_LIST_H

#include <limits.h>
#include <stdbool.h>

#define ELF_INVALID_INT INT_MIN

typedef struct _ElfList ElfList;

ElfList *elfList_new();
void elfList_insert(ElfList *list_p, int data);
bool elfList_insertUnique(ElfList *list, int key);
int elfList_indexOf(ElfList *list, int key);
bool elfList_contains(ElfList *list, int key);
int elfList_count(ElfList *list, int key);
void elfList_destroy(ElfList **list_p);
int elfList_remove(ElfList *list, int index);
int elfList_size(const ElfList *list);
int elfList_get(const ElfList *list, int index);
void elfList_traverse(ElfList *list, void(*func)(int));

typedef struct _ElfListIt ElfListIt;
struct _ElfListIt {
	int key;
	ElfListIt *next;
};
ElfListIt *elfList_getIterator(ElfList *list);

/* Documentation

ELF_INVALID_INT
	- Invalid integer used to sinnalize errors.

ElfList
	- Ordered list of integers
	- Ascending order
	- Accepts duplicates

ElfList *elfList_new();
	- Creates a new ordered list.

void elfList_insert(ElfList *list_p, int data);
	- Inserts 'key' into the list.
	- It is forbidden to insert ELF_INVALID_INT (INT_MIN).

bool elfList_insertUnique(ElfList *list, int key){
	- Inserts 'key' into the list if it's not a duplicate.
	- If it is a duplicate, return false (fail). Else return true.
	- It is forbidden to insert ELF_INVALID_INT.

int elfList_indexOf(ElfList *list, int key);
	- If 'key' exists within list, return the index of the first occurence.
	- Return -1 otherwise.

bool elfList_contains(ElfList *list, int key);
	- Returns 'true' if 'key' exists within list.

int elfList_count(ElfList *list, int key);
	- Returns the amount of times the value 'key' appears in the list.

void elfList_destroy(ElfList **list_p);
	- Destroys a list

int elfList_remove(ElfList *list, int index);
	- Removes and returns the item at index 'index'.
	- If 'index' is out of range, nothing is done and ELF_INVALID_INT is returned.

int elfList_size(const ElfList *list);
	- Returns the number of elements in the list.

int elfList_get(const ElfList *list, int index);
	- Returns the element at index 'index'.
	- If 'index' is out of range, nothing is done and ELF_INVALID_INT is returned.

void elfList_traverse(ElfList *list, void(*func)(void*));
	- Traverses all nodes in the list, applying 'func' in each stored integer.


ElfListIt
	- Iterator for the list structure.
	- Avoid using.

ElfListIt *elfList_getIterator(ElfList *list);
	- Returns an iterator to the first element of list.
	- Iterators should not be freed.
	- Nodes pointed by iterators shouldn't be modified.
	- The key in the Node shouldn't be modified.
	- Iterators become dangling if the origin list is modified.
	- Iterators become dangling if origin list is destroyed.

*/

#endif
