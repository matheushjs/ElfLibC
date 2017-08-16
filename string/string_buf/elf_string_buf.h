#ifndef _ELF_STRING_BUF_H
#define _ELF_STRING_BUF_H

typedef struct _ElfStringBuf ElfStringBuf;

ElfStringBuf *elfStringBuf_new();
void elfStringBuf_destroy(ElfStringBuf **elf_p);
void elfStringBuf_appendChar(ElfStringBuf *elf, char c);
void elfStringBuf_appendString(ElfStringBuf *elf, const char *str);
char *elfStringBuf_makeString(ElfStringBuf *elf, int *len);

/* DOCUMENTATION

typedef struct _ElfStringBuf ElfStringBuf;
	Structure for building a string by appending characters or other strings.
	A character is any value of byte len.

ElfStringBuf *elfStringBuf_new();
	Returns a new string buffer

void elfStringBuf_destroy(ElfStringBuf **elf_p);
	Frees all memory allocated for the given string buffer

void elfStringBuf_appendChar(ElfStringBuf *elf, char c);
	Appends character 'c' to the given string buffer.

void elfStringBuf_appendString(ElfStringBuf *elf, const char *str);
	Appends string 'str' to the string buffer.

char *elfStringBuf_makeString(ElfStringBuf *elf, int *len);
	Returns the string within the given ElfStringBuf, and resets its state to having a string of len 0.
	If 'len' is not NULL, it receives the lenth of the returned string, disregarding the '\0'.
*/

#endif
