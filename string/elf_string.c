#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

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
