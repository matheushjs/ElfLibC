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
bool elfFile_exists(const char *filename){
	FILE *fp = fopen(filename, "r");
	if(!fp) return false;
	fclose(fp);
	return true;
}

// Documented in header file.
void elfFile_printByteInfo(const char *filename){
	FILE *fp = fopen(filename, "r");
	char c;

	if(!fp) return;

	while(!elfFile_feof(fp)){
		c = fgetc(fp);
		printf("%c - %3d\n", c, c);
	}
	fclose(fp);
}

// Documented in header file.
void elfFile_printContent(const char *filename){
	FILE *fp = fopen(filename, "r");

	if(!fp) return;

	while(!elfFile_feof(fp))
		fputc(fgetc(fp), stdout);
	fclose(fp);
}

// Documented in header file.
char *elfFile_getContent(const char *filename, int *size_p){
	FILE *fp = fopen(filename, "r");
	int size;
	char *result;

	if(!fp){
		result = malloc(1);
		*result = '\0';
		if(size_p) *size_p = 0;
		return result;
	}

	size = elfFile_fsize(fp);
	result = malloc(sizeof(char) * size);
	fread(result, size, sizeof(char), fp);
	fclose(fp);

	if(size_p) *size_p = size;
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
	fwrite(contents, sizeof(char), size, fp);
	fclose(fp);
}
