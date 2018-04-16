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
#ifndef _ELF_ENCODINGS_H
#define _ELF_ENCODINGS_H

int elfEncodings_lower_utf8(char *c);
int elfEncodings_upper_utf8(char *c);

char elfEncodings_lower_latin1(unsigned char c);
char elfEncodings_upper_latin1(unsigned char c);

int elfEncodings_toUtf8_latin1(unsigned char c, char *out1, char *out2);
char elfEncodings_toLatin1_utf8(const char *c, int *length);
char elfEncodings_toAscii_latin1(unsigned char c);

int elfEncodings_charLength_utf8(unsigned char firstChar);

char *elfEncodings_advanceChar_utf8(char *str);
const char *elfEncodings_advanceChar_utf8_c(const char *str);

int elfEncodings_charCmp_utf8(const char *a, const char *b);

void elfEncodings_fputc_utf8(const char *c, FILE *fp);
void elfEncodings_putc_utf8(const char *c);

/* DOCUMENTATION

Library for manipulating individual UTF or latin1 characters.
For manipulating multiple characters, refer to elfString library or anything alike.
All pointers to characters will be of type (char*).
Character arguments will be either (char) or (unsigned char), depending on which is more useful
  for the computation within the function receiving such char. Characters passed by value are
  automatically converted between signed/unsigned, so there shouldn't be any problems.
All returned characters will be of type (char).

int elfEncodings_lower_utf8(char *c);
	Converts an utf8 character to lowercase.
	'c' must be pointing to the first character of the utf character.

int elfEncodings_upper_utf8(char *c);
	Converts an utf8 character to uppercase.
	'c' must be pointing to the first character of the utf character.

char elfEncodings_lower_latin1(unsigned char c);
	Returns the lowercase of the latin1 character 'c'.

char elfEncodings_upper_latin1(unsigned char c);
	Returns the uppercase of the latin1 character 'c'.

int elfEncodings_toUtf8_latin1(unsigned char c, char *out1, char *out2);
	Given a latin1 character 'c', returns its utf8 representation.
	If the given latin1 maps to a utf8 character with size 1, 'out2' receives 0.
	Returns:
		- Return value is the size of the resulting utf8 character. Can be 1 or 2.
		- 'out1' and 'out2' receive the utf bytes.
		- If 1 is the returned value, then 'out2' is 0.

char elfEncodings_toLatin1_utf8(const char *c, int *length);
	Given a pointer 'c' to the first byte of a utf8 character,
	  returns its latin1 representation.
	If 'len' is not NULL, it receives the size of the utf8 character converted.
	If the utf8 character does not map to any latin1, then 0 is returned.

char elfEncodings_toAscii_latin1(char c);
	Given a character 'c' in latin1, returns its ascii correspondent.
	Latin1 characters that arent ascii are converted to the most similar ascii character. If there
	  are no similar ascii, then 0 is returned.

int elfEncodings_charLength_utf8(unsigned char firstChar);
	Given a first byte of a utf8 character, returns the character's length.

char *elfEncodings_advanceChar_utf8(char *str);
const char *elfEncodings_advanceChar_utf8_c(const char *str);
	Returns a pointer to the next character of the UTF8 string.

int elfEncodings_charCmp_utf8(const char *a, const char *b);
	Compares characters 'a' and 'b', returning -1, 0, 1 in a similar way to strcmp().

void elfEncodings_fputc_utf8(const char *c, FILE *fp);
	Writes a single UTF character 'c' to the file 'fp'.

void elfEncodings_putc_utf8(const char *c);
	Same as _fputc_utf8 but 'fp' is set as stdout.
*/

#endif
