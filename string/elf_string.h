#ifndef _ELF_STRING_H
#define _ELF_STRING_H

#include <stdarg.h>

char *elfString_strip(const char *str);
char *elfString_rstrip(const char *str);
char *elfString_lstrip(const char *str);

char *elfString_dup(const char *str);

char *elfString_format(const char *format, ...)
	__attribute__((format(printf, 1, 2)));

char **elfString_split(const char *str, const char *delimiter);
char **elfString_split_bag(const char *str, const char *delimiterBag);
char **elfString_splitLines(const char *str);

char *elfString_join(const char *delimiter, ...);

/* DOCUMENTATION

char *elfString_strip(const char *str);
	Removes trailing and leading blank/space characters from 'str'.
	Returns a newly allocated string.

char *elfString_rstrip(const char *str);
	Removes trailing blank/space characters from 'str'.
	Returns a newly allocated string.

char *elfString_lstrip(const char *str);
	Removes leading blank/space characters from 'str'.
	Returns a newly allocated string.


char *elfString_dup(const char *str);
	Returns a copy of 'str'.


char *elfString_format(const char *format, ...);
	Returns a string that is formatted using a printf-like 'format' argument.


char **elfString_split(const char *str, const char *delimiter);
	Splits the given string 'str' into tokens that are delimited by the whole string given
	  in the parameter 'delimiter'.
	Returns a NULL-terminated array of strings.

char **elfString_split_bag(const char *str, const char *delimiterBag);
	Same as _split(), but what is considered as delimiter is each individual character within the
	  parameter 'delimiterBag'.
	Returns a NULL-terminated array of strings.

char **elfString_splitLines(const char *str);
	Same as _split_bag(), but the bag of delimiters is "\r\n".
	Returns a NULL-terminated array of strings.


char *elfString_join(const char *delimiter, ...);
	The variadic parameters are all expected to be (char *).
	The last variadic parameter MUST BE NULL, or the program will crash.
	This functions joins all the strings given as variadic parameters, and places 'delimiter'
	  between each of these strings.
	'delimiter' is not inserted in the beginning or in the end of the resulting string.
	Returns the resulting string.

*/

#endif
