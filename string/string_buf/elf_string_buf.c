#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 8

// Structure for building a string by appending characters or other strings.
typedef struct _ElfStringBuf {
	char *str;
	int size;
	int capacity;
} ElfStringBuf;

/* Static functions declarations */
static inline void change_size(ElfStringBuf *elf, int size);
/* */

// Documented in header file.
ElfStringBuf *elfStringBuf_new(){
	ElfStringBuf *elf;

	elf = malloc(sizeof(ElfStringBuf));
	elf->str = malloc(sizeof(char) * INITIAL_CAPACITY);
	elf->size = 0;
	elf->capacity = INITIAL_CAPACITY;

	return elf;
}

// Documented in header file.
void elfStringBuf_destroy(ElfStringBuf **elf_p){
	ElfStringBuf *elf = *elf_p;

	if(elf){
		free(elf->str);
		free(elf);
		*elf_p = NULL;
	}
}

// Changes 'buf' so that it can hold 'size' characters.
// First checks the capacity, then grows/shrinks if needed.
static inline
void change_size(ElfStringBuf *elf, int size){
	if(size < INITIAL_CAPACITY){
		elf->size = size;
		return;
	}

	if(elf->capacity < size){
		// Grow
		while(elf->capacity < size)
			elf->capacity <<= 1;
		elf->str = realloc(elf->str, sizeof(char) * elf->capacity);

	} else if( size < (0.33 * elf->capacity) ){
		// Shrink
		while(elf->capacity > size)
			elf->capacity >>= 1;
		elf->capacity <<= 1;
		
		// Don't shrink too much
		if(elf->capacity < INITIAL_CAPACITY)
			elf->capacity = INITIAL_CAPACITY;

		elf->str = realloc(elf->str, sizeof(char) * elf->capacity);
	}
	
	elf->size = size;
}

// Documented in header file.
void elfStringBuf_appendChar(ElfStringBuf *elf, char c){
	change_size(elf, elf->size + 1);
	elf->str[elf->size-1] = c;
}

// Documented in header file.
void elfStringBuf_appendString(ElfStringBuf *elf, const char *str){
	int i, len = strlen(str);
	change_size(elf, elf->size + len);

	for(i = 1; i <= len; i++){
		elf->str[elf->size - i] = str[len - i];
	}
}

// Documented in header file.
char *elfStringBuf_makeString(ElfStringBuf *elf, int *size){
	char *result;
	
	result = elf->str;
	result[elf->size] = '\0';
	result = realloc(result, sizeof(char) * (elf->size + 1));

	if(size)
		*size = elf->size;

	elf->str = malloc(sizeof(char) * 8);
	elf->size = 0;
	elf->capacity = 8;

	return result;
}




/*
 * TODO
 */

int elfStringBuf_getSize(ElfStringBuf *elf){

}

const
char *elfStringBuf_getString(ElfStringBuf *elf){
	// Place the ending '\0' and return the string
}

void elfStringBuf_insertChar(ElfStringBuf *elf, int pos, char c){
	// Inserts char 'c' so that it's at position 'pos'
}

void elfStringBuf_insertBytes(ElfStringBuf *elf, int pos, void *bytes, int size){
	// Inserts 'size' bytes from memory position 'bytes'
}

void elfStringBuf_removeBytes(ElfStringBuf *elf, int pos, int size){
	// Removes the given range
}
