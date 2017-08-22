#ifndef _ELF_ENCODINGS_H
#define _ELF_ENCODINGS_H

int elfEncodings_lower_utf8(unsigned char *c);
int elfEncodings_upper_utf8(unsigned char *c);

unsigned char elfEncodings_lower_latin1(unsigned char c);
unsigned char elfEncodings_upper_latin1(unsigned char c);

int elfEncodings_toUtf8_latin1(unsigned char c, unsigned char *out1, unsigned char *out2);
unsigned char elfEncodings_toLatin1_utf8(const unsigned char *c, int *length);
unsigned char elfEncodings_toAscii_latin1(unsigned char c);

int elfEncodings_charLength_utf8(unsigned char firstChar);


/* DOCUMENTATION

int elfEncodings_lower_utf8(unsigned char *c);
	Converts an utf8 character to lowercase.
	'c' must be pointing to the first character of the utf character.

int elfEncodings_upper_utf8(unsigned char *c);
	Converts an utf8 character to uppercase.
	'c' must be pointing to the first character of the utf character.

unsigned char elfEncodings_lower_latin1(unsigned char c);
	Returns the lowercase of the latin1 character 'c'.

unsigned char elfEncodings_upper_latin1(unsigned char c);
	Returns the uppercase of the latin1 character 'c'.

int elfEncodings_toUtf8_latin1(unsigned char c, unsigned char *out1, unsigned char *out2);
	Given a latin1 character 'c', returns its utf8 representation.
	If the given latin1 maps to a utf8 character with size 1, 'out2' receives 0.
	Returns:
		- Return value is the size of the resulting utf8 character. Can be 1 or 2.
		- 'out1' and 'out2' receive the utf bytes.
		- If 1 is the returned value, then 'out2' is 0.

unsigned char elfEncodings_toLatin1_utf8(const unsigned char *c, int *length);
	Given a pointer 'c' to the first byte of a utf8 character,
	  returns its latin1 representation.
	If 'len' is not NULL, it receives the size of the utf8 character converted.
	If the utf8 character does not map to any latin1, then 0 is returned.

unsigned char elfEncodings_toAscii_latin1(unsigned char c);
	Given a character 'c' in latin1, returns its ascii correspondent.
	Latin1 characters that arent ascii are converted to the most similar ascii character. If there
	  are no similar ascii, then 0 is returned.

int elfEncodings_charLength_utf8(unsigned char firstChar);
	Given a first byte of a utf8 character, returns the character's length.

*/

#endif
