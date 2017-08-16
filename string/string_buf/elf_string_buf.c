#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)
#define ELF_MAX(X,Y) ((X)>(Y)?X:Y)

#define INITIAL_CAPACITY 8

// Structure for building a string by appending characters or other strings.
typedef struct _ElfStringBuf {
	char *str;
	int len;  // Number of ascii characters in the string. (excluding the \0)
	int capacity;
} ElfStringBuf;

/* Static functions declarations */
static inline void change_length(ElfStringBuf *elf, int len);
/* */

// Documented in header file.
ElfStringBuf *elfStringBuf_new(){
	ElfStringBuf *elf;

	elf = malloc(sizeof(ElfStringBuf));
	elf->str = malloc(sizeof(char) * INITIAL_CAPACITY);
	elf->len = 0;
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

// Changes 'buf' so that it can hold 'len' characters.
// First checks the capacity, then grows/shrinks if needed.
// This should be the only function reallocating ElfStringBuf's array.
// One byte for appending a '\0' is always available.
static inline
void change_length(ElfStringBuf *elf, int len){
	elf->len = len;
	
	if(len < INITIAL_CAPACITY)
		return;

	// For making an extra byte available.
	len += 1;

	if(elf->capacity < len){
		// Grow
		while(elf->capacity < len)
			elf->capacity <<= 1;
		elf->str = realloc(elf->str, sizeof(char) * elf->capacity);

	} else if( len < (0.33 * elf->capacity) ){
		// Shrink
		while(elf->capacity > len)
			elf->capacity >>= 1;
		elf->capacity <<= 1;
		
		// Don't shrink too much
		if(elf->capacity < INITIAL_CAPACITY)
			elf->capacity = INITIAL_CAPACITY;

		elf->str = realloc(elf->str, sizeof(char) * elf->capacity);
	}
}

// Documented in header file.
void elfStringBuf_appendChar(ElfStringBuf *elf, char c){
	change_length(elf, elf->len + 1);
	elf->str[elf->len-1] = c;
}

// Documented in header file.
void elfStringBuf_appendString(ElfStringBuf *elf, const char *str){
	int i, len = strlen(str);
	change_length(elf, elf->len + len);

	for(i = 1; i <= len; i++){
		elf->str[elf->len - i] = str[len - i];
	}
}

// Documented in header file.
char *elfStringBuf_makeString(ElfStringBuf *elf, int *len){
	char *result;
	
	// There is always space available for the '\0'.
	result = elf->str;
	result[elf->len] = '\0';

	// Shrink to fit.
	result = realloc(result, sizeof(char) * (elf->len + 1));

	if(len)
		*len = elf->len;

	// Reset the structure.
	elf->str = malloc(sizeof(char) * INITIAL_CAPACITY);
	elf->len = 0;
	elf->capacity = 8;

	return result;
}

// Documented in header file.
int elfStringBuf_getLength(const ElfStringBuf *elf){
	return elf->len;
}

// Documented in header file.
const
char *elfStringBuf_getString(ElfStringBuf *elf){
	elf->str[elf->len] = '\0';
	return (const char *) elf->str;
}

// Documented in header file.
void elfStringBuf_insertChar(ElfStringBuf *elf, int pos, char c){
	if(pos < 0 || pos > elf->len) ELF_DIE("Invallid position");
	
	change_length(elf, elf->len + 1);
	memmove(&elf->str[pos+1], &elf->str[pos], sizeof(char) * (elf->len - pos));
	elf->str[pos] = c;
}

// Documented in header file.
void elfStringBuf_insertBytes(ElfStringBuf *elf, int pos, const void *bytes, int len){
	if(pos < 0 || pos > elf->len) ELF_DIE("Invallid position");
	if(len == 0) return;

	change_length(elf, elf->len + len);
	memmove(&elf->str[pos+len], &elf->str[pos], sizeof(char) * (elf->len - pos));
	memcpy(&elf->str[pos], bytes, sizeof(char) * len);
}

// Documented in header file.
void elfStringBuf_removeBytes(ElfStringBuf *elf, int pos, int len){
	if(pos < 0 || pos > elf->len) ELF_DIE("Invallid position");
	if(len == 0) return;

	int beg, end;
	for(beg = pos, end = pos + len; end < elf->len; beg++, end++)
		elf->str[beg] = elf->str[end];
	change_length(elf, ELF_MAX(0, elf->len - len));
}
