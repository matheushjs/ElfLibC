#include <stdio.h>
#include <stdlib.h>

#include <elf_utf_buf.h>
#include <elf_string_buf.h>
#include <elf_encodings.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)

// Structure for building a string by appending characters or other strings.
typedef struct _ElfUtfBuf {
	ElfStringBuf *buf;
	int len;   // Length in number of UTF characters.
} ElfUtfBuf;

/* Static functions declarations */
static inline int utf_char_index(const char *str, int pos);
/* */

// Given a UTF8 string, finds the index at which the pos-th character begins.
// If 'pos' exceeds the string size, the position of '\0' is returned.
static inline
int utf_char_index(const char *str, int pos){
	int index, len;

	index = 0;
	while(*str != '\0' && pos > 0){
		len = elfEncodings_charLength_utf8(*str);
		pos -= 1;
		str += len;
		index += len;
	}

	return index;
}

// Documented in header file.
ElfUtfBuf *elfUtfBuf_new(){
	ElfUtfBuf *elf = malloc(sizeof(ElfUtfBuf));
	elf->buf = elfStringBuf_new();
	elf->len = 0;
	return elf;
}

// Documented in header file.
void elfUtfBuf_destroy(ElfUtfBuf **elf_p){
	ElfUtfBuf *elf = *elf_p;
	if(elf){
		elfStringBuf_destroy(&elf->buf);
		free(elf);
		*elf_p = NULL;
	}
}

// Documented in header file.
void elfUtfBuf_appendChar(ElfUtfBuf *elf, const char *c){
	int charLen, bufLen;

	// In 'c' points to the end of a string.
	if(*c == '\0')
		return;

	// Get length of the given char
	charLen = elfEncodings_charLength_utf8(*c);

	// Get length of current string
	bufLen = elfStringBuf_getLength(elf->buf);

	// Insert character to the end of the buffer
	elfStringBuf_insertBytes(elf->buf, bufLen, c, charLen);
	elf->len += 1;
}

// Documented in header file.
void elfUtfBuf_appendString(ElfUtfBuf *elf, const char *str){
	int len;

	while(*str != '\0'){
		len = elfEncodings_charLength_utf8(*str);
		elfUtfBuf_appendChar(elf, str);

		// Advance whole character
		str += len;
	}
}

// Documented in header file.
const
char *elfUtfBuf_getString(ElfUtfBuf *elf){
	return elfStringBuf_getString(elf->buf);
}

// Documented in header file.
char *elfUtfBuf_makeString(ElfUtfBuf *elf, int *len){
	char *result = elfStringBuf_makeString(elf->buf, NULL);
	if(len) *len = elf->len;
	elf->len = 0;
	return result;
}

// Documented in header file.
int elfUtfBuf_getLength(const ElfUtfBuf *elf){
	return elf->len;
}

// Documented in header file.
int elfUtfBuf_getSize(const ElfUtfBuf *elf){
	return elfStringBuf_getLength(elf->buf);
}

// Documented in header file.
void elfUtfBuf_insertChar(ElfUtfBuf *elf, int pos, const char *c){
	if(pos < 0 || pos > elf->len) ELF_DIE("Invallid position");
	
	int idx = utf_char_index(elfStringBuf_getString(elf->buf), pos);
	int len = elfEncodings_charLength_utf8(*c);
	elfStringBuf_insertBytes(elf->buf, idx, c, len);
	elf->len += 1;
}


//TODO

void elfUtfBuf_insertString(ElfUtfBuf *elf, int pos, const char *string, int len){

}

void elfUtfBuf_removeString(ElfUtfBuf *elf, int pos, int nChars){

}

const
char *elfUtfBuf_getChar(const ElfUtfBuf *elf, int pos){

}

void elfUtfBuf_setChar(ElfUtfBuf *elf, int pos, const char *c){

}
