#ifndef _ELF_UTF_BUF_H
#define _ELF_UTF_BUF_H

typedef struct _ElfUtfBuf ElfUtfBuf;

ElfUtfBuf *elfUtfBuf_new();
void elfUtfBuf_destroy(ElfUtfBuf **elf_p);

void elfUtfBuf_appendChar(ElfUtfBuf *elf, const char *c);
void elfUtfBuf_appendString(ElfUtfBuf *elf, const char *str);

int elfUtfBuf_getLength(const ElfUtfBuf *elf);
int elfUtfBuf_getSize(const ElfUtfBuf *elf);

const char *elfUtfBuf_getString(ElfUtfBuf *elf);
char *elfUtfBuf_makeString(ElfUtfBuf *elf, int *len);

void elfUtfBuf_insertChar(ElfUtfBuf *elf, int pos, const char *c);


/* DOCUMENTATION

typedef ElfUtfBuf;
	Structure for holding a UTF-8 String buffer.
	This library provides functions for manipulating the characters within a string that holds UTF-8 characters.


ElfUtfBuf *elfUtfBuf_new();
	Returns a newly created Utf String buffer.

void elfUtfBuf_destroy(ElfUtfBuf **elf_p);
	Frees all memory allocated to the given utf string buffer.

void elfUtfBuf_appendChar(ElfUtfBuf *elf, const char *c);
	Appends the given utf character pointer to by 'c' to the buffer.

void elfUtfBuf_appendString(ElfUtfBuf *elf, const char *str);
	Appends the given utf string pointed to by 'str' to the buffer.
	The utf string must be null terminated, as every C string.

int elfUtfBuf_getLength(const ElfUtfBuf *elf);
	Returns the number of UTF8 characters in the buffer.

int elfUtfBuf_getSize(const ElfUtfBuf *elf);
	Returns the number of bytes occupied by all the characters in the buffer.
	'\0' is not included.


const char *elfUtfBuf_getString(ElfUtfBuf *elf);
	Returns a pointer to the null-terminated string within the buffer.
	The string isn't supposed to be modified.
	The string may become INVALLID after any call to any other function in this library.

char *elfUtfBuf_makeString(ElfUtfBuf *elf, int *len);
	Returns the string within the given ElfUtfBuf, and resets its state to having a string of len 0.
	If 'len' is not NULL, it receives the length of the returned string, which is the number of
	  utf characters, disregarding the trailing '\0'.

void elfUtfBuf_insertChar(ElfUtfBuf *elf, int pos, const char *c);
	Inserts character 'c' in the buffer so that it occupies the position 'pos'.
	'pos' may be the last position, but may not be beyond that.

*/

#endif
