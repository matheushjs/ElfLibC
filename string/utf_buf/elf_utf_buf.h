#ifndef _ELF_UTF_BUF_H
#define _ELF_UTF_BUF_H

typedef struct _ElfUtfBuf ElfUtfBuf;

ElfUtfBuf *elfUtfBuf_new();
void elfUtfBuf_destroy(ElfUtfBuf **elf_p);

/* DOCUMENTATION

typedef ElfUtfBuf;
	Structure for holding a UTF String buffer.
	This library provides functions for manipulating the characters within a string that holds UTF characters.


ElfUtfBuf *elfUtfBuf_new();
	Returns a newly created Utf String buffer.

void elfUtfBuf_destroy(ElfUtfBuf **elf_p);
	Frees all memory allocated to the given utf string buffer.

*/

#endif
