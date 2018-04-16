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
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Documented in header file.
int elfFile_fsize(FILE *fp){
	int tell, size;
	
	tell = ftell(fp);
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, tell, SEEK_SET);

	return size;
}

// Documented in header file.
char elfFile_fpeek(FILE *fp){
	char c = fgetc(fp);
	if(c != EOF) fseek(fp, -1, SEEK_CUR);
	return c;
}

// Documented in header file.
bool elfFile_feof(FILE *fp){
	char c = elfFile_fpeek(fp);
	return c == EOF ? true : false;
}

// Documented in header file.
void elfFile_fprintByteInfo(FILE *fp){
	int tell = ftell(fp);
	char c;
	fseek(fp, 0, SEEK_SET);
	while(!elfFile_feof(fp)){
		c = fgetc(fp);
		printf("%c - %3d\n", c, c);
	}
	fseek(fp, tell, SEEK_SET);
}

// Documented in header file.
void elfFile_fprintContent(FILE *fp){
	int tell = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	while(!elfFile_feof(fp))
		fputc(fgetc(fp), stdout);
	fseek(fp, tell, SEEK_SET);
}

// Documented in header file.
char *elfFile_fgetContent(FILE *fp, int *size_p){
	int tell = ftell(fp);
	int size = elfFile_fsize(fp);
	char *result = malloc(sizeof(char) * size);
	fread(result, size, sizeof(char), fp);
	fseek(fp, tell, SEEK_SET);
	if(size_p) *size_p = size;
	return result;
}

// Documented in header file.
void elfFile_fappendContent(FILE *fp, const char *contents, int size){
	fseek(fp, 0, SEEK_END);
	fwrite(contents, size, sizeof(char), fp);
}

// Documented in header file.
int elfFile_size(const char *filename){
	FILE *fp = fopen(filename, "r");
	int size = elfFile_fsize(fp);
	fclose(fp);
	return size;
}

// Documented in header file.
bool elfFile_exists(const char *filename){
	FILE *fp = fopen(filename, "r");
	if(!fp) return false;
	fclose(fp);
	return true;
}

// Documented in header file.
void elfFile_printByteInfo(const char *filename){
	FILE *fp = fopen(filename, "r");
	if(!fp) return;
	elfFile_fprintByteInfo(fp);
	fclose(fp);
}

// Documented in header file.
void elfFile_printContent(const char *filename){
	FILE *fp = fopen(filename, "r");
	if(!fp) return;
	elfFile_fprintContent(fp);
	fclose(fp);
}

// Documented in header file.
char *elfFile_getContent(const char *filename, int *size_p){
	FILE *fp = fopen(filename, "r");
	char *result;

	if(!fp){
		if(size_p) *size_p = 0;
		return calloc(sizeof(char), 1);
	}
	
	result = elfFile_fgetContent(fp, size_p);
	fclose(fp);
	return result;
}

// Documented in header file.
void elfFile_setContent(const char *filename, const char *contents, int size){
	FILE *fp = fopen(filename, "w");
	fwrite(contents, sizeof(char), size, fp);
	fclose(fp);
}

// Documented in header file.
void elfFile_appendContent(const char *filename, const char *contents, int size){
	FILE *fp = fopen(filename, "a");
	elfFile_fappendContent(fp, contents, size);
	fclose(fp);
}

// Documented in header file.
char *elfFile_freadLine(FILE *fp){
	char *result;
	int retval;

	retval = fscanf(fp, "%m[^\n]", &result);
	fscanf(fp, "%*c"); // Clear newline

	if(retval != 1)
		return calloc(sizeof(char), 1);
	else
		return result;
}

// Documented in header file.
int elfFile_fcountLines(FILE *fp){
	int count, tell = ftell(fp);
	char c;

	if(elfFile_fsize(fp) == 0)
		return 0;

	fseek(fp, 0, SEEK_SET);
	count = 1;
	while( (c = fgetc(fp)) != EOF )
		if(c == '\n')
			count++;

	fseek(fp, tell, SEEK_SET);
	return count;
}

// Documented in header file.
int elfFile_countLines(const char *filename){
	int count;
	FILE *fp;

	fp = fopen(filename, "r");
	if(!fp) return 0;

	count = elfFile_fcountLines(fp);
	fclose(fp);

	return count;
}
