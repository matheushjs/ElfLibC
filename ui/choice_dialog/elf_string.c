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
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <elf_encodings.h>
#include <elf_string_buf.h>
#include <elf_string.h>

/* Static functions defined later */
static bool char_equals_any(char c, const char *accept);
static bool char_equals_any_utf8(const char *c, const char *accept);
/* */


// Documented in header file.
char *elfString_lstrip(const char *str){
	int len, res_len, lead;
	char *result;

	len = strlen(str);

	// Count leading spaces
	for(lead = 0; lead < len; lead++){
		if( !isspace(str[lead]) )
			break;
	}

	res_len = len - lead;
	result = malloc(sizeof(char) * (res_len + 1));
	memcpy(result, str+lead, res_len * sizeof(char));
	result[res_len] = '\0';
	
	return result;
}

// Documented in header file.
char *elfString_rstrip(const char *str){
	int len, res_len, trail;
	char *result;

	len = strlen(str);

	// Count trailing spaces
	for(trail = 0; trail < len; trail++){
		if( !isspace(str[len-trail-1]) )
			break;
	}

	res_len = len - trail;
	result = malloc(sizeof(char) * (res_len + 1));
	memcpy(result, str, res_len * sizeof(char));
	result[res_len] = '\0';

	return result;
}

// Documented in header file.
char *elfString_strip(const char *str){
	char *aux, *res;
	aux = elfString_rstrip(str);
	res = elfString_lstrip(aux);
	free(aux);
	return res;
}

// Documented in header file.
char *elfString_dup(const char *str){
	int len;
	char *result;

	len = strlen(str);
	result = malloc(sizeof(char) * (len + 1));
	memcpy(result, str, len + 1);

	return result;
}

// Documented in header file.
char *elfString_format(const char *format, ...){
	va_list vl, vc;
	char *result, dummy;
	int retval;

	va_start(vl, format);

	// Get ideal size
	va_copy(vc, vl);
	retval = vsnprintf(&dummy, 0, format, vc);
	va_end(vc);

	// Execute formatting
	result = malloc(sizeof(char) * (retval + 1));
	vsnprintf(result, retval + 1, format, vl);

	va_end(vl);

	return result;
}

// Documented in header file.
char **elfString_split(const char *str, const char *delimiter){
	char **result, c;
	const char *iter;
	int delSize, // size of the delimiter
	    resSize, // size of array 'result'
	    curSize; // curSize, size of token being processed
	ElfStringBuf *newStr;

	iter = str;
	delSize = strlen(delimiter);
	
	// Protection against delimiter = "". Which would cause an eternal loop.
	if(delSize == 0){
		result = malloc(sizeof(char *) * 2);
		result[0] = elfString_dup(str);
		result[1] = NULL;
		return result;
	}

	resSize = 0;
	result = NULL;

	// Initialize stringBuf
	newStr = elfStringBuf_new();

	while(true){
		//Skip delimiter
		while( strncmp(iter, delimiter, delSize) == 0){
			iter += delSize;
		}

		//Read token
		while( strncmp(iter, delimiter, delSize) != 0){
			c = *iter;
			if(c == '\0') break;
			//Add character to the string
			elfStringBuf_appendChar(newStr, c);
			iter += 1;
		}

		// Allocate one more string
		resSize += 1;
		result = realloc(result, sizeof(char *) * resSize);

		// Grab string on Buffer
		result[resSize - 1] = elfStringBuf_makeString(newStr, &curSize);

		// If string is empty, we reached the end of 'str'.
		// Notice that if it's empty, _makeString() returns an empty string which should be freed.
		if(curSize == 0){
			free(result[resSize - 1]);
			result[resSize - 1] = NULL; // NULL-termination
			break;
		}
	}

	elfStringBuf_destroy(&newStr);

	return result;
}

// Verifies if 'c' is any character within 'accept'.
// If 'accept' is empty, 'false' is returned.
static
bool char_equals_any(char c, const char *accept){
	while(*accept != '\0'){
		if(c == *accept)
			return true;
		accept++;
	}
	return false;
}

// Documented in header file.
char **elfString_split_bag(const char *str, const char *delimiterBag){
	char **result, c;
	const char *iter;
	int resSize, curSize;
	ElfStringBuf *newStr;

	iter = str;
	
	// Set up for using realloc
	resSize = 0;
	result = NULL;

	// Initialize stringBuf
	newStr = elfStringBuf_new();

	while(true){
		//Skip delimiters
		while( char_equals_any(*iter, delimiterBag) ){
			iter += 1;
		}

		//Read token
		while( !char_equals_any(*iter, delimiterBag) ){
			c = *iter;
			if(c == '\0') break;
			//Add character to the string
			elfStringBuf_appendChar(newStr, c);
			iter += 1;
		}

		// Allocate one more string
		resSize += 1;
		result = realloc(result, sizeof(char *) * resSize);

		// Grab string on Buffer
		result[resSize - 1] = elfStringBuf_makeString(newStr, &curSize);

		// If string is empty, we reached the end of 'str'.
		// Notice that if it's empty, _makeString() returns an empty string which should be freed.
		if(curSize == 0){
			free(result[resSize - 1]);
			result[resSize - 1] = NULL; // NULL-termination
			break;
		}
	}

	elfStringBuf_destroy(&newStr);

	return result;
}

// Documented in header file.
char **elfString_splitLines(const char *str){
	return elfString_split_bag(str, "\r\n");
}

// Documented in header file.
char *elfString_join(const char *delimiter, ...){
	va_list vl;
	ElfStringBuf *buf;
	const char *str;
	char *result;
	int count;

	buf = elfStringBuf_new();

	va_start(vl, delimiter);
	
	count = 0;
	while(true){
		str = va_arg(vl, const char*);
		if(str == NULL) break;

		// Add delimiter only when needed
		if(count != 0){
			elfStringBuf_appendString(buf, delimiter);
		}

		count++;
		elfStringBuf_appendString(buf, str);
	}
	
	va_end(vl);

	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}

// Documented in header file.
char *elfString_center_fill(const char *str, int width, char fill){
	int len, diff, left, right;
	ElfStringBuf *buf;
	char *result;

	len = strlen(str);
	diff = width - len;

	// If 'str' is longer than 'width', return a duplicate.
	if(diff <= 0)
		return elfString_dup(str);

	buf = elfStringBuf_new();

	left = diff/2;
	right = diff - left;

	// Add leading fill
	for(; left > 0; left--)
		elfStringBuf_appendChar(buf, fill);

	// Add string
	elfStringBuf_appendString(buf, str);

	// Add trailing fill
	for(; right > 0; right--)
		elfStringBuf_appendChar(buf, fill);

	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}

// Documented in header file.
char *elfString_center(const char *str, int width){
	return elfString_center_fill(str, width, ' ');
}

// Documented in header file.
int elfString_count(const char *str, const char *key){
	int len, count;

	len = strlen(key);
	if(len == 0) // Protection against infinite loop
		return 0;

	count = 0;
	while(*str != '\0'){
		if( strncmp(str, key, len) == 0){
			count++;
			str += len;
		} else {
			str += 1;
		}
	}

	return count;
}

// Documented in header file.
int elfString_find(const char *str, const char *key){
	int len, index;

	len = strlen(key);
	if(len == 0) // Protection against a bug
		return -1;

	index = 0;
	while(*str != '\0'){
		if( strncmp(str, key, len) == 0){
			return index;
		} else {
			str += 1;
			index++;
		}
	}

	return -1;
}

// Documented in header file.
char *elfString_replace(const char *str, const char *old, const char *neww){
	ElfStringBuf *buf;
	int len;
	char *result;

	len = strlen(old);
	if(len == 0) // Protection against bugs
		return elfString_dup(str);

	buf = elfStringBuf_new();
	while(*str != '\0'){
		if( strncmp(str, old, len) == 0){
			elfStringBuf_appendString(buf, neww);
			str += len;
		} else {
			elfStringBuf_appendChar(buf, *str);
			str += 1;
		}
	}
	
	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}

// Documented in header file.
bool elfString_endsWith(const char *str, const char *key){
	int slen, klen;
	slen = strlen(str);
	klen = strlen(key);
	if(klen > slen) return false;
	else return strncmp(str + (slen - klen), key, klen) == 0 ? true : false;
}

// Documented in header file.
bool elfString_startsWith(const char *str, const char *key){
	int len = strlen(key);
	return strncmp(str, key, len) == 0 ? true : false;
}

// Documented in header file.
char *elfString_slice(const char *str, int left, int right){
	int len, aux, beg, end;

	len = strlen(str);
	
	// Decide beginning character
	// Should be the first sliced character
	if(left >= 0){
		beg = left;
	} else {
		beg = len + left;
	}

	// Decide end character
	// Should be the first character after the slice 
	if(right >= 0){
		end = right;
	} else {
		end = len + right;
	}

	// Swap if inverted
	if(beg > end){
		aux = end;
		end = beg;
		beg = aux;
	}

	// Replace if out-of-bounds
	if(beg < 0)
		beg = 0;

	// Replace if out-of-bounds
	if(end >= len)
		end = len;

	int i;
	ElfStringBuf *buf;
	char *result;

	buf = elfStringBuf_new();
	for(i = beg; i < end; i++)
		elfStringBuf_appendChar(buf, str[i]);
	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}

// Documented in header file.
char *elfString_invert(const char *str){
	int i, len;
	ElfStringBuf *buf;

	len = strlen(str);
	buf = elfStringBuf_new();
	for(i = len - 1; i >= 0; i--)
		elfStringBuf_appendChar(buf, str[i]);
	
	char *result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);
	return result;
}

// Documented in header file.
char *elfString_lower(const char *str){
	ElfStringBuf *buf;
	char *result;

	buf = elfStringBuf_new();
	while( *str != '\0' ){
		elfStringBuf_appendChar(buf, tolower(*str));
		str++;
	}

	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}

// Documented in header file.
char *elfString_upper(const char *str){
	ElfStringBuf *buf;
	char *result;

	buf = elfStringBuf_new();
	while( *str != '\0' ){
		elfStringBuf_appendChar(buf, toupper(*str));
		str++;
	}

	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}

// Documented in header file.
char *elfString_capitalize(const char *str){
	int len;
	char *result;
	
	len = strlen(str);
	result = malloc(sizeof(char) * (len + 1));
	memcpy(result, str, len+1);
	result[0] = toupper(result[0]);

	return result;
}

// Documented in header file.
char *elfString_swapCase(const char *str){
	ElfStringBuf *buf;
	char *result;

	buf = elfStringBuf_new();
	while( *str != '\0' ){
		if(islower(*str))
			elfStringBuf_appendChar(buf, toupper(*str));
		else
			elfStringBuf_appendChar(buf, tolower(*str));
		str++;
	}

	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}

// Documented in header file.
char *elfString_title(const char *str){
	ElfStringBuf *buf;
	char *result;

	buf = elfStringBuf_new();

	while(true){
		// Skip whitespace
		while(isspace(*str)){
			elfStringBuf_appendChar(buf, *str);
			str++;
		}

		// Check end of string
		if(*str == '\0')
			break;

		// Capitalize character
		elfStringBuf_appendChar(buf, toupper(*str));
		str++;

		// Skip non-whitespace
		while(!isspace(*str) && *str != '\0'){
			elfStringBuf_appendChar(buf, *str);
			str++;
		}
	}

	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}


/*
 * ENCODING-RELATED FUNCTIONS
 */

// Documented in header file.
char *elfString_toUtf8_fromLatin1(const char *str){
	ElfStringBuf *buf;
	char *result;
	char c0, c1;

	buf = elfStringBuf_new();

	while(*str != '\0'){
		elfEncodings_toUtf8_latin1(*str, &c0, &c1);
		elfStringBuf_appendChar(buf, c0);
		if(c1 != 0){
			elfStringBuf_appendChar(buf, c1);
		}
		str++;
	}

	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}

// Documented in header file.
char *elfString_toLatin1_fromUtf8(const char *str){
	ElfStringBuf *buf;
	char c, *result;
	int len;

	buf = elfStringBuf_new();

	while(*str != '\0'){
		c = elfEncodings_toLatin1_utf8(str, &len);
		if(c != 0)
			elfStringBuf_appendChar(buf, c);
		else
			elfStringBuf_appendChar(buf, '*');
		str += len;
	}

	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}

// Documented in header file.
char *elfString_toAscii_fromLatin1(const char *str){
	char *result, *aux;
	result = aux = elfString_dup(str);
	while(*aux != '\0'){
		*aux = elfEncodings_toAscii_latin1(*aux);
		if(*aux == 0)
			*aux = '*';
		aux++;
	}
	return result;
}

// Documented in header file.
char *elfString_toAscii_fromUtf8(const char *str){
	char *result, *aux;
	aux = elfString_toLatin1_fromUtf8(str);
	result = elfString_toAscii_fromLatin1(aux);
	free(aux);
	return result;
}

// Documented in header file.
int elfString_len_latin1(const char *str){
	return strlen(str);
}

// Documented in header file.
int elfString_len_utf8(const char *str){
	int len, aux;
	
	len = 0;
	while(*str != '\0'){
		aux = elfEncodings_charLength_utf8(*str);
		str += aux;
		len += 1;
	}

	return len;
}

// Documented in header file.
char *elfString_lower_latin1(const char *str){
	ElfStringBuf *buf;
	char *result;

	buf = elfStringBuf_new();
	while(*str != '\0'){
		elfStringBuf_appendChar(buf, elfEncodings_lower_latin1(*str));
		str++;
	}

	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);
	return result;
}

// Documented in header file.
char *elfString_upper_latin1(const char *str){
	ElfStringBuf *buf;
	char *result;

	buf = elfStringBuf_new();
	while(*str != '\0'){
		elfStringBuf_appendChar(buf, elfEncodings_upper_latin1(*str));
		str++;
	}

	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);
	return result;
}

// Documented in header file.
char *elfString_lower_utf8(const char *str){
	char *result, *aux;
	int len;

	result = aux = elfString_dup(str);
	while(*aux != '\0'){
		len = elfEncodings_lower_utf8(aux);
		aux += len;
	}

	return result;
}

// Documented in header file.
char *elfString_upper_utf8(const char *str){
	char *result, *aux;
	int len;

	result = aux = elfString_dup(str);
	while(*aux != '\0'){
		len = elfEncodings_upper_utf8(aux);
		aux += len;
	}

	return result;
}

// Documented in header file.
char *elfString_capitalize_latin1(const char *str){
	char *result;
	result = elfString_dup(str);
	result[0] = elfEncodings_upper_latin1(result[0]);
	return result;
}

// Documented in header file.
char *elfString_capitalize_utf8(const char *str){
	char *result;
	result = elfString_dup(str);
	elfEncodings_upper_utf8(result);
	return result;
}

// Documented in header file.
char *elfString_title_latin1(const char *str){
	ElfStringBuf *buf;
	char *result;

	buf = elfStringBuf_new();

	while(true){
		// Skip whitespace
		while(isspace(*str)){
			elfStringBuf_appendChar(buf, *str);
			str++;
		}

		// Check end of string
		if(*str == '\0')
			break;

		// Capitalize character
		elfStringBuf_appendChar(buf, elfEncodings_upper_latin1(*str));
		str++;

		// Skip non-whitespace
		while(!isspace(*str) && *str != '\0'){
			elfStringBuf_appendChar(buf, *str);
			str++;
		}
	}

	result = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}

// Documented in header file.
char *elfString_title_utf8(const char *str){
	char *result, *aux;
	int len;

	result = aux = elfString_dup(str);

	while(true){
		// Skip whitespace
		while(isspace(*aux))
			aux++;

		// Check end of string
		if(*aux == '\0')
			break;

		// Capitalize character
		len = elfEncodings_upper_utf8(aux);
		aux += len;

		// Skip non-whitespace
		while(!isspace(*aux) && *aux != '\0')
			aux++;
	}

	return result;
}

// Documented in header file.
int elfString_strncmp_utf8(const char *a, const char *b, int size){
	int count, cmp;

	count = 0; // Number of characters that were compared and were equal
	while(true){
		if(count == size)
			return 0;
		if( *a == '\0' && *b == '\0')
			return 0;
		if(*a == '\0')
			return -1;
		if(*b == '\0')
			return 1;

		cmp = elfEncodings_charCmp_utf8(a, b);
		if(cmp != 0)
			return cmp;

		count++;
		a = elfEncodings_advanceChar_utf8_c(a);
		b = elfEncodings_advanceChar_utf8_c(b);
	}

	return 0;
}

// Documented in header file.
int elfString_strcmp_utf8(const char *a, const char *b){
	return elfString_strncmp_utf8(a, b, -1);
}

// Documented in header file.
char **elfString_split_utf8(const char *str, const char *delimiter){
	char **result;
	const char *iter;
	int delLen,  // number of characters in the delimiter
	    delSize, // Number of bytes occupied by the delimiter
	    resSize, // size of array 'result'
	    curSize; // curSize, size of token being processed
	ElfStringBuf *newStr;

	iter = str;
	delLen  = elfString_len_utf8(delimiter);
	delSize = strlen(delimiter);

	// Protection against delimiter = "". Which would cause an eternal loop.
	if(delLen == 0){
		result = malloc(sizeof(char *) * 2);
		result[0] = elfString_dup(str);
		result[1] = NULL;
		return result;
	}

	// Initialize for usage with realloc
	resSize = 0;
	result = NULL;

	// Initialize stringBuf
	newStr = elfStringBuf_new();

	while(true){
		// Skip delimiter
		while( elfString_strncmp_utf8(iter, delimiter, delLen) == 0){
			iter += delSize;
		}

		// Read token
		while( elfString_strncmp_utf8(iter, delimiter, delLen) != 0){
			if(*iter == '\0') break;
			
			// Add character to string buffer
			int i, charSize = elfEncodings_charLength_utf8(*iter);
			for(i = 0; i < charSize; i++)
				elfStringBuf_appendChar(newStr, iter[i]);

			iter += charSize;
		}

		// Allocate one more string
		resSize += 1;
		result = realloc(result, sizeof(char *) * resSize);

		// Grab string on buffer
		result[resSize - 1] = elfStringBuf_makeString(newStr, &curSize);

		// If string is empty, we reached the end of 'str
		// Notice that if it's empty, _makeString() returns an empty string that should be freed
		if(curSize == 0){
			free(result[resSize - 1]);
			result[resSize - 1] = NULL; // We return a NULL-terminated array of strings
			break;
		}
	}

	elfStringBuf_destroy(&newStr);
	return result;
}

// Verifies if 'c' is any UTF character within 'accept'.
// If 'accept' is empty, 'false' is returned.
// If 'c' is empty, 'false' is returned.
static
bool char_equals_any_utf8(const char *c, const char *accept){
	while(*accept != '\0'){
		if(elfEncodings_charCmp_utf8(c, accept) == 0)
			return true;
		accept = elfEncodings_advanceChar_utf8_c(accept);
	}
	return false;
}

// Documented in header file.
char **elfString_split_bag_utf8(const char *str, const char *delimiterBag){
	char **result;
	const char *iter;
	int resSize,
	    curSize;
	ElfStringBuf *newStr;

	iter = str;

	// Set up for using realloc
	resSize = 0;
	result = NULL;

	// Initialize stringBuf
	newStr = elfStringBuf_new();

	while(true){

		// Skip delimiters
		while( char_equals_any_utf8(iter, delimiterBag) ){
			iter = elfEncodings_advanceChar_utf8_c(iter);
		}

		// Read token
		while( !char_equals_any_utf8(iter, delimiterBag) ){
			if(*iter == '\0') break;

			// Add character to the string
			int i, charSize = elfEncodings_charLength_utf8(*iter);
			for(i = 0; i < charSize; i++)
				elfStringBuf_appendChar(newStr, iter[i]);

			// Advance iterator
			iter += charSize;
		}

		// Allocate one more string
		resSize += 1;
		result = realloc(result, sizeof(char *) * resSize);

		// Grab string on Buffer
		result[resSize - 1] = elfStringBuf_makeString(newStr, &curSize);

		// If string is empty, we reached the end of 'str'.
		// Notice that if it's empty, _makeString() returns an empty string which should be freed.
		if(curSize == 0){
			free(result[resSize - 1]);
			result[resSize - 1] = NULL; // NULL-termination
			break;
		}
	}
	
	elfStringBuf_destroy(&newStr);

	return result;
}
