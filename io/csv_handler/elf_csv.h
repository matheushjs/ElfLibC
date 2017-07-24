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
typedef struct _ElfCsvW ElfCsvW;

ElfCsvW *elfCsvW_new_withArgs(const char *filename, char sep, bool truncate);
ElfCsvW *elfCsvW_new(const char *filename);
void elfCsvW_destroy(ElfCsvW **elf_p);
void elfCsvW_writeLine(ElfCsvW *elf, const char **strings);



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


typedef ElfCsvW;
	Structure representing a CSV Writer.

ElfCsvW *elfCsvW_new_withArgs(const char *filename, char sep, bool truncate);
	Returns a new CSV Writer, based on file with name 'filename', and considering 'sep' as the
	  internal separator between fields.
	If 'truncate' is true, the file will be truncated if it already exists.
	If 'truncate' is false, we will append to the file.

ElfCsvW *elfCsvW_new(const char *filename);
	Same as above, but with default arguments:
		sep = ','
		truncate = true

void elfCsvW_destroy(ElfCsvW **elf_p);
	Frees all memory allocated for the given CSV Writer.

void elfCsvW_writeLine(ElfCsvW *elf, const char **strings);
	Writes a new line to the CSV file.
	'strings' is expected to be a NULL-terminated array of strings.
	If there are no strings in the string array, nothing is written to the CSV file.
*/

#endif
