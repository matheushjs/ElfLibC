#ifndef _STACK_H
#define _STACK_H

#define ELF_INT_TO_POINTER(i) ((void *) (long int) (i))
#define ELF_UINT_TO_POINTER(i) ((void *) (unsigned long int) (i))
#define ELF_POINTER_TO_INT(p) ((int) (long int) (p))
#define ELF_POINTER_TO_UINT(p) ((unsigned int) (unsigned long int) (p))
//Credits: https://developer.gnome.org/glib/stable/glib-Type-Conversion-Macros.html

typedef struct _ElfStack ElfStack;

ElfStack *elfStack_new();
void elfStack_destroy(ElfStack **s);
void elfStack_destroy_f(ElfStack **s_pointer, void(*func)(void*));
void elfStack_push(ElfStack *s, void *data);
void *elfStack_pop(ElfStack *s);
void *elfStack_top(const ElfStack *s);
int elfStack_size(const ElfStack *s);
void elfStack_traverse(const ElfStack *s, void(*func)(void*));
void elfStack_traverse_inv(const ElfStack *s, void(*func)(void*));

/* DOCUMENTATION

ELF_INT_TO_POINTER(i)
ELF_UINT_TO_POINTER(i)
ELF_POINTER_TO_INT(p)
ELF_POINTER_TO_UINT(p)
	- Macros for type casting from signed/unsigned integers to pointers.
	- Useful since this queue data structure can only store void pointers.

ElfStack *elfStack_new();
	- Returns a new stack.

void elfStack_destroy(ElfStack **s);
	- Destroys the stack, doing nothing to the stored pointers.

void elfStack_destroy_f(ElfStack **s_pointer, void(*func)(void*));
	- Destroys the stack, applying 'func' to each stored pointer.

void elfStack_push(ElfStack *s, void *data);
	- Inserts 'data' on the top of the stack.

void *elfStack_pop(ElfStack *s);
	- Removes and returns the element at the top of the stack.
	- Returns NULL if the stack is empty.
	- CAUTION: If using POINTER/INT macros, you should check the size
		of the stack before using this function.

void *elfStack_top(const ElfStack *s);
	- Returns the element at the top of the stack, without removing it.
	- Returns NULL if the stack is empty.
	- CAUTION: If using POINTER/INT macros, you should check the size
		of the stack before using this function.

int elfStack_size(const ElfStack *s);
	- Returns the number of elements within the stack.

void elfStack_traverse(const ElfStack *s, void(*func)(void*));
	- Traverse the stack, from top to bottom, applying 'func' to each element.

void elfStack_traverse_inv(const ElfStack *s, void(*func)(void*));
	- Traverse the stack, from bottom to top, applying 'func' to each element.

*/

#endif
