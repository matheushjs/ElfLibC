#include <stdio.h>
#include <stdbool.h>

bool elfFile_fileExist(const char *filename){
	FILE *fp = fopen(filename, "r");
	if(!fp) return false;
	fseek(fp, -1, SEEK_CUR);
	return true;
}
