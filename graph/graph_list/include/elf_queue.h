#ifndef _ELF_QUEUE_H
#define _ELF_QUEUE_H

#define ELF_INT_TO_POINTER(i) ((void *) (long int) (i))
#define ELF_UINT_TO_POINTER(i) ((void *) (unsigned long int) (i))
#define ELF_POINTER_TO_INT(p) ((int) (long int) (p))
#define ELF_POINTER_TO_UINT(p) ((unsigned int) (unsigned long int) (p))
//Credits: https://developer.gnome.org/glib/stable/glib-Type-Conversion-Macros.html

typedef struct _ElfQueue ElfQueue;

ElfQueue *elfQueue_new();
void elfQueue_push(ElfQueue *q, void *data);
void *elfQueue_pop(ElfQueue *q);
void elfQueue_destroy(ElfQueue **q_pointer);
void elfQueue_destroy_f(ElfQueue **q_pointer, void(*func)(void*));
int elfQueue_size(const ElfQueue *q);
void *elfQueue_front(const ElfQueue *q);
void *elfQueue_back(const ElfQueue *q);
void elfQueue_invert(ElfQueue *q);
void elfQueue_traverse(const ElfQueue *q, void(*func)(void *));

/* Documentation

ELF_INT_TO_POINTER(i)
ELF_UINT_TO_POINTER(i)
ELF_POINTER_TO_INT(p)
ELF_POINTER_TO_UINT(p)
	- Macros for type casting from signed/unsigned integers to pointers.
	- Useful since this queue data structure can only store void pointers.

ElfQueue *elfQueue_alloc();
	- Returns a new queue.

void elfQueue_push(ElfQueue *q, void *data);
	- Inserts 'data' at the back of the queue.

void *elfQueue_pop(ElfQueue *q);
	- Removes and returns the front element.
	- Returns NULL if queue is empty.
	- CAUTION: If using POINTER/INT macros, you should check the size
		of the queue before using this function.

void elfQueue_destroy(ElfQueue **q_pointer);
	- Destroys the queue. Nothing is done to the stored pointers.

void elfQueue_destroy_f(ElfQueue **q_pointer, void(*func)(void*));
	- Destroy the queue, freeing each key using function 'func'.

int elfQueue_size(const ElfQueue *q);
	- Returns the number of elements in the queue.

void *elfQueue_front(const ElfQueue *q);
	- Returns the front element, without removing it.
	- Returns NULL if queue is empty.
	- CAUTION: If using POINTER/INT macros, you should check the size
		of the queue before using this function.

void *elfQueue_back(const ElfQueue *q);
	- Returns the back element, without removing it.
	- Returns NULL if queue is empty.
	- CAUTION: If using POINTER/INT macros, you should check the size
		of the queue before using this function.

void elfQueue_invert(ElfQueue *q);
	- Inverts the order of the queue.

void elfQueue_traverse(const ElfQueue *q, void(*func)(void *));
	Traverse the queue, from front to back, applying function 'func' in each element.

*/

#endif