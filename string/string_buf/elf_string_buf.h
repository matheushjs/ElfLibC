#ifndef _ELF_STRING_BUF_H
#define _ELF_STRING_BUF_H

typedef struct _ElfStringBuf ElfStringBuf;

ElfStringBuf *elfStringBuf_new();
void elfStringBuf_destroy(ElfStringBuf **elf_p);
void elfStringBuf_appendChar(ElfStringBuf *elf, char c);
void elfStringBuf_appendString(ElfStringBuf *elf, const char *str);
char *elfStringBuf_makeString(ElfStringBuf *elf, int *len);

int elfStringBuf_getLength(const ElfStringBuf *elf);
const char *elfStringBuf_getString(ElfStringBuf *elf);

void elfStringBuf_insertChar(ElfStringBuf *elf, int pos, char c);
void elfStringBuf_insertBytes(ElfStringBuf *elf, int pos, const void *bytes, int len);
void elfStringBuf_removeBytes(ElfStringBuf *elf, int pos, int len);

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


int elfStringBuf_getLength(const ElfStringBuf *elf);
	Returns the number of characters in the string in the buffer.

const char *elfStringBuf_getString(ElfStringBuf *elf);
	Returns a pointer to the null-terminated string within the buffer.
	The string isn't supposed to be modified.
	The string may become INVALLID after any call to any other function in this library.


void elfStringBuf_insertChar(ElfStringBuf *elf, int pos, char c);
	Inserts character 'c' in the buffer so that it occupies the position 'pos'.
	'pos' may be the last position, but may not be beyond that.

void elfStringBuf_insertBytes(ElfStringBuf *elf, int pos, const void *bytes, int len);
	Inserts 'len' bytes from the memory location 'bytes' into the buffer, so that it begins
	  at position 'pos'.

void elfStringBuf_removeBytes(ElfStringBuf *elf, int pos, int len);
	Removes 'len' bytes from the string buffer, starting at position 'pos'.
	The characters to the right of the removed position are "shifted-left".

*/

#endif
