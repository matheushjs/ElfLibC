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
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

/* In this file we'll be trying to handle some conversion between ascii, latin1 and UTF8.
 * Particularly, the conversion of the latin1 characters between 128-255.
 */

// All the utf characters being converted have length 2.
// utfchar[0] is the first byte.
typedef unsigned char utfchar[2];

// All hashes defined on the bottom.
static unsigned char lower_latinCaseHash[128];
static unsigned char upper_latinCaseHash[128];
static utfchar       lower_utfCaseHash[320];
static utfchar       upper_utfCaseHash[320];
static unsigned char u2l_encodingHash[320];
static unsigned char l2a_encodingHash[128];

// Hashes any of the latin1 characters higher than 127 into a
//   number from 0 to 127
static inline
int latin1_hashing(unsigned char c){
	return c - 128;
}

// Hashes any UTF8 character that is also a latin1 character above 127,
//   into a number from 0 to 319
// If the given UTF character is not a latin1 character, but has length 2,
//   the hashTable will yield an invalid conversion ( aka {0,0} )
static inline
int utf8_hashing(utfchar c){
	unsigned int num = 0;
	num |= c[1];
	num |= ((int) c[0]) << 8;
	return num - 49792;
}

// Returns the length of the UTF8 character.
// 'firstChar' should be the first byte of the UTF8 character.
int elfEncodings_charLength_utf8(unsigned char firstChar){
    if(firstChar < 0x80 ) return 1;
    else if (!(firstChar & 0x20)) return 2;
    else if (!(firstChar & 0x10)) return 3;
    else if (!(firstChar & 0x08)) return 4;
    else if (!(firstChar & 0x04)) return 5;
	else return 6;
}

// Documented in header file.
int elfEncodings_lower_utf8(char *c){
	int hash, len = elfEncodings_charLength_utf8(*c);
	utfchar u;

	if(len == 1){
		*c = tolower(*c);
	} else if (len == 2){
		u[0] = c[0];
		u[1] = c[1];
		hash = utf8_hashing(u);
		u[0] = lower_utfCaseHash[hash][0];
		u[1] = lower_utfCaseHash[hash][1];
		if(u[0] != 0) {
			c[0] = u[0];
			c[1] = u[1];
		}
	}

	return len;
}

// Documented in header file.
int elfEncodings_upper_utf8(char *c){
	int hash, len = elfEncodings_charLength_utf8(*c);
	utfchar u;

	if(len == 1){
		*c = toupper(*c);
	} else if (len == 2){
		u[0] = c[0];
		u[1] = c[1];
		hash = utf8_hashing(u);
		u[0] = upper_utfCaseHash[hash][0];
		u[1] = upper_utfCaseHash[hash][1];
		if(u[0] != 0) {
			c[0] = u[0];
			c[1] = u[1];
		}
	}

	return len;
}

// Documented in header file.
char elfEncodings_lower_latin1(unsigned char c){
	if(c < 0x80) return tolower(c);
	else return lower_latinCaseHash[latin1_hashing(c)];
}

// Documented in header file.
char elfEncodings_upper_latin1(unsigned char c){
	if(c < 0x80) return toupper(c);
	else return upper_latinCaseHash[latin1_hashing(c)];
}

//Adapted from
//http://stackoverflow.com/questions/4059775/convert-iso-8859-1-strings-to-utf-8-in-c-c
//Answer from user 'Lord Raiden'
//
// Documented in header file.
int elfEncodings_toUtf8_latin1(unsigned char c, char *out1, char *out2){
	if(c < 0x80){
		*out1 = c, *out2 = 0;
		return 1;
	} else {
		*out1 = 0xc0 | c >> 6;     //first  utf8 character
		*out2 = 0x80 | (c & 0x3f); //second utf8 character
		return 2;
	}
}

// Documented in header file.
char elfEncodings_toLatin1_utf8(const char *c, int *length){
	int hash, len;
	char result;
	utfchar u;

	len = elfEncodings_charLength_utf8(*c);
	if(len == 1){
		result = *c;
	} else if(len == 2){
		u[0] = c[0];
		u[1] = c[1];
		hash = utf8_hashing(u);
		result = u2l_encodingHash[hash];
	} else {
		result = 0;
	}
	
	if(length)
		*length = len;

	return result;
}

// Documented in header file.
char elfEncodings_toAscii_latin1(unsigned char c){
	int hash;
	if(c < 0x80){
		return c;
	} else {
		hash = latin1_hashing(c);
		return l2a_encodingHash[hash];
	}
}

// Documented in header file.
char *elfEncodings_advanceChar_utf8(char *str){
	return str + elfEncodings_charLength_utf8(*str);
}

// Documented in header file.
const
char *elfEncodings_advanceChar_utf8_c(const char *str){
	return str + elfEncodings_charLength_utf8(*str);
}

// Documented in header file.
int elfEncodings_charCmp_utf8(const char *a, const char *b){
	int asize, bsize;
	asize = elfEncodings_charLength_utf8(*a);
	bsize = elfEncodings_charLength_utf8(*b);

	if(asize < bsize)
		return -1;
	else if(asize > bsize)
		return 1;
	else
		return strncmp(a, b, asize);
}

// Documented in header file.
void elfEncodings_fputc_utf8(const char *c, FILE *fp){
	fwrite(c, sizeof(char), elfEncodings_charLength_utf8(*c), fp);
}

// Documented in header file.
void elfEncodings_putc_utf8(const char *c){
	elfEncodings_fputc_utf8(c, stdout);
}

static unsigned char lower_latinCaseHash[128] = {
128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,
146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,
164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,
182,183,184,185,186,187,188,189,190,191,224,225,226,227,228,229,230,231,
232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,215,248,249,
250,251,252,253,254,223,224,225,226,227,228,229,230,231,232,233,234,235,
236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,
254,255
};

static unsigned char upper_latinCaseHash[128] = {
128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,
146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,
164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,
182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,
200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,
218,219,220,221,222, 83,192,193,194,195,196,197,198,199,200,201,202,203,
204,205,206,207,208,209,210,211,212,213,214,247,216,217,218,219,220,221,
222,255
};

static utfchar lower_utfCaseHash[320] = {
{194,128},{194,129},{194,130},{194,131},{194,132},{194,133},{194,134},
{194,135},{194,136},{194,137},{194,138},{194,139},{194,140},{194,141},
{194,142},{194,143},{194,144},{194,145},{194,146},{194,147},{194,148},
{194,149},{194,150},{194,151},{194,152},{194,153},{194,154},{194,155},
{194,156},{194,157},{194,158},{194,159},{194,160},{194,161},{194,162},
{194,163},{194,164},{194,165},{194,166},{194,167},{194,168},{194,169},
{194,170},{194,171},{194,172},{194,173},{194,174},{194,175},{194,176},
{194,177},{194,178},{194,179},{194,180},{194,181},{194,182},{194,183},
{194,184},{194,185},{194,186},{194,187},{194,188},{194,189},{194,190},
{194,191},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{195,160},{195,161},{195,162},{195,163},{195,164},{195,165},
{195,166},{195,167},{195,168},{195,169},{195,170},{195,171},{195,172},
{195,173},{195,174},{195,175},{195,176},{195,177},{195,178},{195,179},
{195,180},{195,181},{195,182},{195,151},{195,184},{195,185},{195,186},
{195,187},{195,188},{195,189},{195,190},{195,159},{195,160},{195,161},
{195,162},{195,163},{195,164},{195,165},{195,166},{195,167},{195,168},
{195,169},{195,170},{195,171},{195,172},{195,173},{195,174},{195,175},
{195,176},{195,177},{195,178},{195,179},{195,180},{195,181},{195,182},
{195,183},{195,184},{195,185},{195,186},{195,187},{195,188},{195,189},
{195,190},{195,191}
};

static utfchar upper_utfCaseHash[320] = {
{194,128},{194,129},{194,130},{194,131},{194,132},{194,133},{194,134},
{194,135},{194,136},{194,137},{194,138},{194,139},{194,140},{194,141},
{194,142},{194,143},{194,144},{194,145},{194,146},{194,147},{194,148},
{194,149},{194,150},{194,151},{194,152},{194,153},{194,154},{194,155},
{194,156},{194,157},{194,158},{194,159},{194,160},{194,161},{194,162},
{194,163},{194,164},{194,165},{194,166},{194,167},{194,168},{194,169},
{194,170},{194,171},{194,172},{194,173},{194,174},{194,175},{194,176},
{194,177},{194,178},{194,179},{194,180},{206,156},{194,182},{194,183},
{194,184},{194,185},{194,186},{194,187},{194,188},{194,189},{194,190},
{194,191},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{195,128},{195,129},{195,130},{195,131},{195,132},{195,133},
{195,134},{195,135},{195,136},{195,137},{195,138},{195,139},{195,140},
{195,141},{195,142},{195,143},{195,144},{195,145},{195,146},{195,147},
{195,148},{195,149},{195,150},{195,151},{195,152},{195,153},{195,154},
{195,155},{195,156},{195,157},{195,158},{195,159},{195,128},{195,129},
{195,130},{195,131},{195,132},{195,133},{195,134},{195,135},{195,136},
{195,137},{195,138},{195,139},{195,140},{195,141},{195,142},{195,143},
{195,144},{195,145},{195,146},{195,147},{195,148},{195,149},{195,150},
{195,183},{195,152},{195,153},{195,154},{195,155},{195,156},{195,157},
{195,158},{197,184}
};

static unsigned char u2l_encodingHash[320] = {
128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,
146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,
164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,
182,183,184,185,186,187,188,189,190,191,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,192,193,
194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,
230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,
248,249,250,251,252,253,254,255
};

static unsigned char l2a_encodingHash[128] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0, 0,0,0,'C',0,0,0,0,'R',0,0,0,'2','3','\'','u',0,'.',',', '1','o',0,0,0,0,
0,'A','A','A','A','A','A',0,'C','E','E','E','E', 'I','I','I','I','D','N',
'O','O','O','O','O','x','0','U','U','U','U','Y',0, 'B','a','a','a','a','a',
'a',0,'c','e','e','e','e','i','i','i','i',0,'n', 'o','o','o','o','o',0,0,
'u','u','u','u','y',0,'y'
};
