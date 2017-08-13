#include <stdlib.h>

// Structure for building a string by appending characters or other strings.
typedef struct _ElfStringBuf {
	char *str;
	int size;
	int capacity;
} ElfStringBuf;

// Documented in header file.
ElfStringBuf *elfStringBuf_new(){
	ElfStringBuf *elf;

	elf = malloc(sizeof(ElfStringBuf));
	elf->str = malloc(sizeof(char) * 8);
	elf->size = 0;
	elf->capacity = 8;

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

// Documented in header file.
void elfStringBuf_appendChar(ElfStringBuf *elf, char c){
	elf->str[elf->size] = c;
	elf->size += 1;
	if(elf->size == elf->capacity){
		elf->capacity <<= 1;
		elf->str = realloc(elf->str, sizeof(char) * elf->capacity);
	}
}

// Documented in header file.
void elfStringBuf_appendString(ElfStringBuf *elf, const char *str){
	while( *str != '\0' ){
		elfStringBuf_appendChar(elf, *str);
		str++;
	}
}

// Documented in header file.
char *elfStringBuf_getString(ElfStringBuf *elf, int *size){
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


