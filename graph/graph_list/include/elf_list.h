#ifndef _ELF_LIST_H
#define _ELF_LIST_H

#include <stdbool.h>

#define ELF_INT_TO_POINTER(i) ((void *) (long int) (i))
#define ELF_UINT_TO_POINTER(i) ((void *) (unsigned long int) (i))
#define ELF_POINTER_TO_INT(p) ((int) (long int) (p))
#define ELF_POINTER_TO_UINT(p) ((unsigned int) (unsigned long int) (p))
//Credits: https://developer.gnome.org/glib/stable/glib-Type-Conversion-Macros.html

#define ELF_POINTER_TO_INT_GREATER elfList_pointerToIntGreaterFunc
#define ELF_POINTER_TO_UINT_GREATER elfList_pointerToUIntGreaterFunc

typedef struct _ElfList ElfList;

ElfList *elfList_new(bool (*greaterThan)(void*,void*));
ElfList *elfList_newWithEqual(bool (*greaterThan)(void*,void*), bool (*equal)(void*,void*));

/* Functions accepted by any list */
void elfList_insert(ElfList *list_p, void *data);
void elfList_destroy(ElfList **list_p);
void elfList_destroyF(ElfList **list_p, void (*func)(void*));
void *elfList_removeIndex(ElfList *list, int index);
int elfList_size(const ElfList *list);
void *elfList_get(const ElfList *list, int index);
void elfList_traverse(ElfList *list, void(*func)(void*));

/* Functions accepted only by lists with an equality function */
bool elfList_insertUnique(ElfList *list_p, void *data);
int elfList_indexOf(ElfList *list_p, void *data);
bool elfList_contains(ElfList *list, void *data);
int elfList_count(ElfList *list_p, void *data);
int elfList_removeValueF(ElfList *list_p, void *data, void(*free_f)(void*));
int elfList_removeValue(ElfList *list, void *data);

/* Functions on iterators */
typedef struct _ElfListIt ElfListIt;
struct _ElfListIt {
	void *key;
	ElfListIt *next;
};
ElfListIt *elfList_getIterator(ElfList *list);

/* Convenience Functions */
bool elfList_pointerToIntGreaterFunc(void *a, void *b);
bool elfList_pointerToUIntGreaterFunc(void *a, void *b);


/* Documentation

ELF_INT_TO_POINTER(i)
ELF_UINT_TO_POINTER(i)
ELF_POINTER_TO_INT(p)
ELF_POINTER_TO_UINT(p)
	- Macros for type casting from signed/unsigned integers to pointers.
	- Useful since this queue data structure can only store void pointers.


ELF_POINTER_TO_INT_GREATER
ELF_POINTER_TO_UINT_GREATER
	- Convenience macros for using as the greaterThan function when instantiating a list.

ElfList
	- Ordered list of void*
	- Ascending order
	- Accepts duplicates
	- In case of duplicates, older pointers comes first

ElfList *elfList_new(bool (*greaterThan)(void*,void*));
	- Returns a new list, using the function 'greaterThan' as ordering function.
	- greaterThan SHOULD NOT BE REFLEXIVE. This means a > b means b > a is FALSE.
	- By not having 'equal', some operations are not supported.

ElfList *elfList_newWithEqual(bool (*greaterThan)(void*,void*), bool (*equal)(void*,void*));
	- Returns a new list, using the function 'greaterThan' as ordering function,
	  and 'equal' as equality function.
	- greaterThan SHOULD NOT BE REFLEXIVE. This means a > b means b > a is FALSE.

void elfList_insert(ElfList *list_p, void *data);
	- Inserts 'data' into the list.

void elfList_destroy(ElfList **list_p);
	- Destroys a list, doing nothing to the pointers stored in it.

void elfList_destroyF(ElfList **list_p, void (*func)(void*));
	- Destroys a list, using function 'func' to free the stored pointers.

void *elfList_removeIndex(ElfList *list, int index);
	- Removes and returns the item at index 'index'.
	- If 'index' is out of range, nothing is done and NULL is returned.

int elfList_size(const ElfList *list);
	- Returns the number of elements in the list.

void *elfList_get(const ElfList *list, int index);
	- Returns the element at index 'index'.
	- If 'index' is out of range, returns NULL.

void elfList_traverse(ElfList *list, void(*func)(void*));
	- Traverses all nodes in the list, applying 'func' in each stored pointer.

bool elfList_insertUnique(ElfList *list_p, void *data);
	- Inserts 'data' into the list if it's not a duplicate.
	- If list doesn't have an 'equal' function, exits the program.
	- Returns true is 'data' is unique, false if it's a duplicate.

int elfList_indexOf(ElfList *list_p, void *data);
	- If list contains 'data', return its index. Return -1 otherwise.

bool elfList_contains(ElfList *list, void *data);
	- Returns true if the list contains 'data'.

int elfList_count(ElfList *list_p, void *data);
	- Returns the number of elements 'data' within the list.

int elfList_removeValueF(ElfList *list_p, void *data, void(*free_f)(void*));
	- Removes all elements with same value as 'data'.
	- Applies function func() in each pointer removed.
	- Returns the number of elements removed.

int elfList_removeValue(ElfList *list, void *data);
	- Removes all elements with same value as 'data'
	- Nothing is done to pointers stored.
	- Returns the number of elements removed.

ElfListIt
	- Iterator for the list structure.

ElfListIt *elfList_getIterator(ElfList *list);
	- Returns an iterator to the first element of list.
	- Iterators should not be freed.
	- Nodes pointed by iterators shouldn't be modified.
	- The key in the Node shouldn't be modified.
	- Iterators become dangling if the origin list is modified.
	- Iterators become dangling if origin list is destroyed.


bool elfList_pointerToIntGreaterFunc(void *a, void *b);
bool elfList_pointerToUIntGreaterFunc(void *a, void *b);
	- Convenience functions for using as the greaterThan function when instantiating a list.
	- Can also use the macros like ELF_POINTER_TO_INT_GREATER
*/

#endif
