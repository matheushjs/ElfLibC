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
#ifndef _ELF_FILE_H
#define _ELF_FILE_H

#include <stdbool.h>

/* FILE POINTER ARGUMENT ONLY */
char elfFile_fpeek(FILE *fp);
bool elfFile_feof(FILE *fp);
char *elfFile_freadLine(FILE *fp);

/* FILE NAME ARGUMENT ONLY */
bool elfFile_exists(const char *filename);
void elfFile_setContent(const char *filename, const char *contents, int size);

/* BOTH ARGUMENTS */
int  elfFile_fsize(FILE *fp);
int  elfFile_size(const char *filename);

void elfFile_fprintByteInfo(FILE *fp);
void elfFile_printByteInfo(const char *filename);

void elfFile_fprintContent(FILE *fp);
void elfFile_printContent(const char *filename);

char *elfFile_fgetContent(FILE *fp, int *size_p);
char *elfFile_getContent(const char *filename, int *size_p);

void elfFile_fappendContent(FILE *fp, const char *contents, int size);
void elfFile_appendContent(const char *filename, const char *contents, int size);

int elfFile_fcountLines(FILE *fp);
int elfFile_countLines(const char *filename);



/* DOCUMENTATION

elf_file.[ch]
	Library with routines to manipulate files.
	Functions that manipulate files by their FILE* have names prepended with 'f'. Those that
	  manipulate files by their names have regular names.
	All functions that receive a (FILE *) as argument are supposed to preserve the file's
	  current position offset. The exception are the functions where the user explicitly
	  request that a read/write/append operation is executed, such as readLine().


char elfFile_fpeek(FILE *fp);
	Returns the next byte in 'fp', without advancing its current offset.

bool elfFile_feof(FILE *fp);
	Returns 'true' if the file has no more bytes to be read.

char *elfFile_freadLine(FILE *fp);
	Returns the next line in file 'fp'.

bool elfFile_exists(const char *filename);
	Returns 'true' if file exists.

void elfFile_setContent(const char *filename, const char *contents, int size);
	Creates/truncates file with name 'filename', and write 'size' bytes from 'contents' into it.



int elfFile_fsize(FILE *fp);
int elfFile_size(const char *filename);
	Returns the size, in bytes, of the file.

void elfFile_fprintByteInfo(FILE *fp);
void elfFile_printByteInfo(const char *filename);
	Prints a pair (character, ascii number) for each byte in the file.
	Function for debugging purposes.

void elfFile_fprintContent(FILE *fp);
void elfFile_printContent(const char *filename);
	Prints the whole file into stdout.

char *elfFile_fgetContent(FILE *fp, int *size_p);
char *elfFile_getContent(const char *filename, int *size_p);
	Returns the whole content of the file.
	If 'size_p' is not NULL, it receives the size of the returned array file.
	Guaranteed to return a newly alloc'ed string, even if the file doesn't exist (string of size 0).

void elfFile_fappendContent(FILE *fp, const char *contents, int size);
void elfFile_appendContent(const char *filename, const char *contents, int size);
	Appends 'size' bytes of 'contents' into the file with name 'filename'. Creates the file if needed.

int elfFile_fcountLines(FILE *fp);
int elfFile_countLines(const char *filename);
	Returns the number of lines in the file.

*/


#endif
