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
void elfUtfBuf_insertString(ElfUtfBuf *elf, int pos, const char *string);
void elfUtfBuf_removeString(ElfUtfBuf *elf, int pos, int nChars);

const char *elfUtfBuf_getChar(const ElfUtfBuf *elf, int pos);
void elfUtfBuf_setChar(ElfUtfBuf *elf, int pos, const char *c);

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

void elfUtfBuf_insertString(ElfUtfBuf *elf, int pos, const char *string);
	Inserts string 'string' in the buffer so that it occupies the position 'pos'.
	'pos' may be the last position, but may not be beyond that.

void elfUtfBuf_removeString(ElfUtfBuf *elf, int pos, int nChars);
	Removes 'len' UTF8 characters from the utf buffer, starting the removal at the 'pos'-th character.
	The characters to the right of the removed position are "shifted-left".
	Any position outside of the buffer's bounds is accepted too. Simply nothing is done.

const char *elfUtfBuf_getChar(const ElfUtfBuf *elf, int pos);
	Returns the 'pos'-th character.
	Any position outside of the buffer's bounds is considered an error.

void elfUtfBuf_setChar(ElfUtfBuf *elf, int pos, const char *c);
	Sets the pos-th UTF8 character as being the UTF8 character 'c'.
	If size of character 'c' is the same as character being modified, the functions just modifies the
	  original character byte-by-byte.
	If sizes are different, bytes are removed and then inserted, which can be a time-consuming  operation.
	'pos' must be a position within the string.
*/

#endif
