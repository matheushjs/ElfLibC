#ifndef _ELF_CSV_H
#define _ELF_CSV_H

#include <stdbool.h>

/* GENERICS */
void elfCsv_freeStrings(char **strings);


/* CSV READER */
typedef struct _ElfCsvR ElfCsvR;

ElfCsvR *elfCsvR_new(const char *filename);
ElfCsvR *elfCsvR_new_withSep(const char *filename, char sep);
void elfCsvR_destroy(ElfCsvR **elf_p);
bool elfCsvR_hasNext(ElfCsvR *elf);
char **elfCsvR_nextLine(ElfCsvR *elf);
const char **elfCsvR_nextLine_managed(ElfCsvR *elf);


/* CSV WRITER */




/* DOCUMENTATION

elf_csv.[ch]
	Library for manipulating CSV files.
	This library attempts to follow the RFC4180 standard, which is available
	  at: https://tools.ietf.org/html/rfc4180
	The CSV files may be in encodings latin1, UTF8 or ascii.
	The CSV may end with a newline, unless it's empty.


void elfCsv_freeStrings(char **strings);
	Frees memory of a NULL-terminated array of strings.


typedef ElfCsvR;
	Structure that represents a CSV Reader.


ElfCsvR *elfCsvR_new(const char *filename);
	Creates a new CSV Reader based on file with name 'filename'.

ElfCsvR *elfCsvR_new_withSep(const char *filename, char sep);
	Creates a new CSV Reader based on file with name 'filename' and with
	  field delimiter 'sep'.

void elfCsvR_destroy(ElfCsvR **elf_p);
	Frees memory allocated for the given CSV Reader

bool elfCsvR_hasNext(ElfCsvR *elf);
	Returns 'true' if there are more lines in the CSV file that haven't been read yet.

char **elfCsvR_nextLine(ElfCsvR *elf);
	Returns the next line in the CSV file.
	The return value is a NULL-terminated array of strings.

const char **elfCsvR_nextLine_managed(ElfCsvR *elf);
	Same as above, but the strings returned will be freed by the internals of this library.
	You may cast the const-qualifier out if you're absolutely sure all the returned pointers will be
	  kept in a state that can be freed.
	You are free to intermingle calls between the _managed overload or the regular _nextLine functions.
	  Only the pointers returned by the _managed overload will be freed, and only on next call to the
	  _managed() function.


*/

#endif
