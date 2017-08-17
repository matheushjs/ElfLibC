#include <stdlib.h>

#include <elf_utf_buf.h>
#include <elf_string_buf.h>
#include <elf_encodings.h>

// Structure for building a string by appending characters or other strings.
typedef struct _ElfUtfBuf {
	ElfStringBuf *buf;
	int len;   // Length in number of UTF characters.
} ElfUtfBuf;

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

	// Get length of the given char
	charLen = elfEncodings_charLength_utf8(*c);

	// Get length of current string
	bufLen = elfStringBuf_getLength(elf->buf);

	// Insert character to the end of the buffer
	elfStringBuf_insertBytes(elf->buf, bufLen, c, charLen);
	elf->len += 1;
}

// Documented in header file.
const
char *elfUtfBuf_getString(ElfUtfBuf *elf){
	return elfStringBuf_getString(elf->buf);
}

// Documented in header file.
int elfUtfBuf_getLength(const ElfUtfBuf *elf){
	return elf->len;
}

// Documented in header file.
int elfUtfBuf_getSize(const ElfUtfBuf *elf){
	return elfStringBuf_getLength(elf->buf);
}



//TODO

void elfUtfBuf_appendString(ElfUtfBuf *elf, const char *str){

}

char *elfUtfBuf_makeString(ElfUtfBuf *elf, int *len){

}

void elfUtfBuf_insertChar(ElfUtfBuf *elf, int pos, const char *c){

}

void elfUtfBuf_insertString(ElfUtfBuf *elf, int pos, const char *string, int len){

}

void elfUtfBuf_removeString(ElfUtfBuf *elf, int pos, int nChars){

}

const
char *elfUtfBuf_getChar(const ElfUtfBuf *elf, int pos){

}

void elfUtfBuf_setChar(ElfUtfBuf *elf, int pos, const char *c){

}
