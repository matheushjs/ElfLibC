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

	// In case given char is empty.
	if(*c == '\0')
		return;

	int idx = utf_char_index(elfStringBuf_getString(elf->buf), pos);
	int len = elfEncodings_charLength_utf8(*c);
	elfStringBuf_insertBytes(elf->buf, idx, c, len);
	elf->len += 1;
}

// Documented in header file.
void elfUtfBuf_insertString(ElfUtfBuf *elf, int pos, const char *string){
	if(pos < 0 || pos > elf->len) ELF_DIE("Invallid position");

	int charCount, size, charSize, idx;
	const char *aux;

	// Calculate number of bytes on the string to add
	size = 0;
	charCount = 0;
	aux = string;
	while(*aux != '\0'){
		charSize = elfEncodings_charLength_utf8(*aux);
		aux += charSize;
		size += charSize;
		charCount += 1;
	}

	// Increment our length
	elf->len += charCount;

	// Find index in our buffer, where we should insert the given string
	idx = utf_char_index(elfStringBuf_getString(elf->buf), pos);

	// Insert
	elfStringBuf_insertBytes(elf->buf, idx, string, size);
}

// Documented in header file.
void elfUtfBuf_removeString(ElfUtfBuf *elf, int pos, int nChars){
	if(pos < 0 || pos >= elf->len) return;
	if(nChars == 0) return;

	const char *buf;
	int idx, len, charCount;

	// Find position of 'pos'-th character
	buf = elfStringBuf_getString(elf->buf);
	idx = utf_char_index(buf, pos);

	// Count length of bytes of nChars characters from that character
	len = 0;
	charCount = 0;
	buf = &buf[idx];
	while(*buf != '\0' && charCount < nChars){
		int charSize = elfEncodings_charLength_utf8(*buf);
		len += charSize;
		buf += charSize;
		charCount += 1;
	}

	// Remove these bytes through the string buffer.
	elfStringBuf_removeBytes(elf->buf, idx, len);

	// Change utf buffer length
	elf->len -= charCount;
}

// Documented in header file.
const
char *elfUtfBuf_getChar(const ElfUtfBuf *elf, int pos){
	if(pos < 0 || pos >= elf->len) ELF_DIE("Invallid position");
	
	const char *buf;
	int idx;

	// Find position of pos-th character
	buf = elfStringBuf_getString(elf->buf);
	idx = utf_char_index(buf, pos);
	
	return &buf[idx];
}

// Documented in header file.
void elfUtfBuf_setChar(ElfUtfBuf *elf, int pos, const char *c){
	if(pos < 0 || pos >= elf->len) ELF_DIE("Invallid position");

	const char *buf;
	int i, idx, rem_size, add_size;

	// Find position of pos-th character
	buf = elfStringBuf_getString(elf->buf);
	idx = utf_char_index(buf, pos);

	// Check size of each characters
	rem_size = elfEncodings_charLength_utf8(buf[idx]);
	add_size = elfEncodings_charLength_utf8(*c);

	// If size of new character is equal, substitute byte-by-byte
	if(rem_size == add_size){
		for(i = 0; i < rem_size; i++)
			elfStringBuf_setChar(elf->buf, idx+i, c[i]);

	// Else, remove bytes and then insert bytes
	} else {
		elfStringBuf_removeBytes(elf->buf, idx, rem_size);
		elfStringBuf_insertBytes(elf->buf, idx, c, add_size);
	}
}
