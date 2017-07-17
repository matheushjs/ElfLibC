#ifndef _ELF_STRING_H
#define _ELF_STRING_H

char *elfString_strip(const char *str);
char *elfString_rstrip(const char *str);
char *elfString_lstrip(const char *str);

char *elfString_dup(const char *str);

char *elfString_format(const char *format, ...);

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



*/

#endif
