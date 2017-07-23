#ifndef _ELF_FILE_H
#define _ELF_FILE_H

#include <stdbool.h>

int elfFile_fsize(FILE *fp);
char elfFile_fpeek(FILE *fp);
bool elfFile_feof(FILE *fp);

bool elfFile_exists(const char *filename);
void elfFile_printByteInfo(const char *filename);
void elfFile_printContent(const char *filename);
char *elfFile_getContent(const char *filename, int *size_p);
void elfFile_setContent(const char *filename, const char *contents, int size);
void elfFile_appendContent(const char *filename, const char *contents, int size);



/* DOCUMENTATION

elf_file.[ch]
	Library with routines to manipulate files.
	All functions that receive a (FILE *) as argument are supposed to preserve the file's
	  current position offset. The exception are the functions where the user explicitly
	  request that a read/write operation is executed, such as readLine().


int elfFile_fsize(FILE *fp);
	Returns the size, in bytes, of the file.

char elfFile_fpeek(FILE *fp);
	Returns the next byte in 'fp', without advancing its current offset.

bool elfFile_feof(FILE *fp);
	Returns 'true' if the file has no more bytes to be read.



bool elfFile_exists(const char *filename);
	Returns 'true' if file exists.

void elfFile_printByteInfo(const char *filename);
	Prints a pair (character, ascii number) for each byte in the file.
	Function for debugging purposes.

void elfFile_printContent(const char *filename);
	Prints the whole file into stdout.

char *elfFile_getContent(const char *filename, int *size_p);
	Returns the whole content of the file.
	If 'size_p' is not NULL, it receives the size of the returned array file.

void elfFile_setContent(const char *filename, const char *contents, int size);
	Creates/truncates file with name 'filename', and write 'size' bytes from 'contents' into it.

void elfFile_appendContent(const char *filename, const char *contents, int size);
	Appends 'size' bytes of 'contents' into the file with name 'filename'. Creates the file if needed.

*/


#endif
