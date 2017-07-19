#ifndef _ELF_STRING_H
#define _ELF_STRING_H

#include <stdarg.h>
#include <stdbool.h>
#include <limits.h>

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

char *elfString_center_fill(const char *str, int width, char fill);
char *elfString_center(const char *str, int width);

int elfString_count(const char *str, const char *key);
int elfString_find(const char *str, const char *key);
char *elfString_replace(const char *str, const char *old, const char *neww);

bool elfString_endsWith(const char *str, const char *key);
bool elfString_startsWith(const char *str, const char *key);

char *elfString_slice(const char *str, int left, int right);
char *elfString_invert(const char *str);

char *elfString_lower(const char *str);
char *elfString_upper(const char *str);
char *elfString_capitalize(const char *str);
char *elfString_swapCase(const char *str);
char *elfString_title(const char *str);

/* ENCODING-RELATED FUNCTIONS */
char *elfString_toUtf8_fromLatin1(const char *str);
char *elfString_toLatin1_fromUtf8(const char *str);

int elfString_len_latin1(const char *str);
int elfString_len_utf8(const char *str);

char *elfString_lower_latin1(const char *str);
char *elfString_upper_latin1(const char *str);
char *elfString_lower_utf8(const char *str);
char *elfString_upper_utf8(const char *str);

char *elfString_capitalize_latin1(const char *str);
char *elfString_capitalize_utf8(const char *str);

char *elfString_title_latin1(const char *str);
char *elfString_title_utf8(const char *str);


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


char *elfString_center_fill(const char *str, int width, char fill);
	Returns a string whose content is the string 'str' centered within a space of 'width' characters,
	  and the filling characters are chosen to be the given 'fill'.

char *elfString_center(const char *str, int width);
	Wrapper for the _center_fill() function, with 'fill' parameter being a space ' '.

int elfString_count(const char *str, const char *key);
	Returns the number of non-overlapping occurences of 'key' within 'str'.

int elfString_find(const char *str, const char *key);
	Finds 'key' within 'str'. Returns the index in 'str' where the first occurence of 'key' happens.
	If not found, returns a negative number.

char *elfString_replace(const char *str, const char *old, const char *neww);
	Returns a string where all occurences of 'old' in 'str' are replaced by 'neww'.


bool elfString_endsWith(const char *str, const char *key);
	Returns true if 'str' ends with string 'key'.

bool elfString_startsWith(const char *str, const char *key);
	Returns true if 'str' starts with 'key'.


char *elfString_slice(const char *str, int left, int right);
	Returns a new string that is a slice of 'str'.
	'left' represents the index of the first character to be sliced.
	'right' represents the index of the first character AFTER THE SLICE.
	Negative numbers in 'left' or 'right' will represent indexes of characters when they are
	  counted backwards from the end of 'str'. So if the string has 5 characters, and 'left'
	  is -1, then left is equivalent to being 4, the index of the 5th character.
	If 'left' or 'right' are out of bounds, they are replaced by the nearest valid value.
	If 'left' is higher than 'right', they are swapped.
	One may use INT_MAX as 'right' to slice to the end of the string.

	Examples:
		_slice("hello",  0,  1) equals "h"
		_slice("hello",  1,  3) equals "el"
		_slice("hello",  0, -1) equals "hell"
		_slice("hello", -5, -1) equals "hell"
		_slice("hello", 0, INT_MAX) equals "hello"

char *elfString_invert(const char *str);
	Returns a string that is 'str' inverted.


char *elfString_lower(const char *str);
	Returns a string that is 'str' in lowercase.

char *elfString_upper(const char *str);
	Returns a string that is 'str' in uppercase.

char *elfString_capitalize(const char *str);
	Returns a string that is 'str' with first character in uppercase.

char *elfString_swapCase(const char *str);
	Returns a string that is 'str' with all lowercase characters converted to uppercase,
	  and all uppercase converted to lowercase.

char *elfString_title(const char *str);
	Returns a string that is 'str' with all tokens capitalized.
	Tokens are any substring that are composed of non-white/blankspace characters.


char *elfString_toUtf8_fromLatin1(const char *str);
	Given a string 'str' in encoding latin1, returns its utf8 equivalent.

char *elfString_toLatin1_fromUtf8(const char *str);
	Given a string 'str' in encoding UTF8, returns its latin1 equivalent.
	If any utf8 character cannot be mapped to latin1, it's replaced by '*'.

int elfString_len_latin1(const char *str);
	Returns the length of a latin1 string.

int elfString_len_utf8(const char *str);
	Returns the length of a utf8 string.

char *elfString_lower_latin1(const char *str);
char *elfString_upper_latin1(const char *str);
	Given a latin1 string, returns its lowercase/uppercase equivalent, respectively.

char *elfString_lower_utf8(const char *str);
char *elfString_upper_utf8(const char *str);
	Given a UTF8 string, returns its lowercase/uppercase equivalent, respectively.

char *elfString_capitalize_latin1(const char *str);
	Given a latin1 string, capitalize its first letter and return in a new string.

char *elfString_capitalize_utf8(const char *str);
	Given a UTF8 string, capitalize its first letter and return in a new string.

char *elfString_title_latin1(const char *str);
	Given a latin1 string 'str', returns a string that is 'str' with all tokens capitalized.
	Tokens are any substring that are composed of non-white/blankspace characters.

char *elfString_title_utf8(const char *str);
	Given a UTF8 string 'str', returns a string that is 'str' with all tokens capitalized.
	Tokens are any substring that are composed of non-white/blankspace characters.

*/

#endif
