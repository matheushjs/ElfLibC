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
