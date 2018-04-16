/*
 * MIT License
 * 
 * Copyright (c) 2018 Matheus H. J. Saldanha <mhjsaldanha@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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

char elfStringBuf_getChar(const ElfStringBuf *elf, int pos);
void elfStringBuf_setChar(ElfStringBuf *elf, int pos, char c);


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
	If 'len' is not NULL, it receives the length of the returned string, disregarding the '\0'.


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
	'pos' may be the last position, but may not be beyond that.

void elfStringBuf_removeBytes(ElfStringBuf *elf, int pos, int len);
	Removes 'len' bytes from the string buffer, starting at position 'pos'.
	The characters to the right of the removed position are "shifted-left".


char elfStringBuf_getChar(const ElfStringBuf *elf, int pos);
	Returns character at position 'pos'.

void elfStringBuf_setChar(ElfStringBuf *elf, int pos, char c);
	Sets character at position 'pos' to value 'c'.
	'pos' must be a position within the string.

*/

#endif
