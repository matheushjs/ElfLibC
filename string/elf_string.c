#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <elf_string_buf.h>
#include <elf_string.h>

/* Static functions defined later */
static bool char_equals_any(char c, const char *accept);


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
	int delSize, resSize, curSize;
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
		result[resSize - 1] = elfStringBuf_getString(newStr, &curSize);

		// If string is empty, we reached the end of 'str'.
		// Notice that if it's empty, _getString() returns an empty string which should be freed.
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
	
	resSize = 0;
	result = NULL;

	// Initialize stringBuf
	newStr = elfStringBuf_new();

	while(true){
		//Skip delimiter
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
		result[resSize - 1] = elfStringBuf_getString(newStr, &curSize);

		// If string is empty, we reached the end of 'str'.
		// Notice that if it's empty, _getString() returns an empty string which should be freed.
		if(curSize == 0){
			free(result[resSize - 1]);
			result[resSize - 1] = NULL; // NULL-termination
			break;
		}
	}

	elfStringBuf_destroy(&newStr);

	return result;
}

char **elfString_splitLines(const char *str){
	// NULL-terminated array of strings
	return NULL;
}

char *elfString_join(const char *delimiter, ...){
	va_list vl;
	va_start(vl, delimiter);
	va_end(vl);
	return NULL;
}

char *elfString_center(const char *str){
	return NULL;
}

int elfString_count(const char *str, const char *key){
	// Only non-overlapping instances
	return 0;
}

bool *elfString_endsWith(const char *str){
	return false;
}

bool *elfString_startsWith(const char *str){
	return false;
}

bool *elfString_find(const char *str){
	return false;
}

char *elfString_replace(const char *str, const char *old, const char *new){
	return NULL;
}

// Slice
// Invert


/*
 * FUNCTIONS THAT REQUIRE UTF8 TREATMENT
 *
 * OR MAKE SPECIAL FUNCTIONS FOR UTF8 (I like this)
 */

char *elfString_capitalize(const char *str){
	return NULL;
}

char *elfString_lower(const char *str){
	return NULL;
}

char *elfString_upper(const char *str){
	return NULL;
}

char *elfString_swapCase(const char *str){
	return NULL;
}

char *elfString_title(const char *str){
	// Pascal case everything
	return NULL;
}
