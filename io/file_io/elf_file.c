#include <stdio.h>
#include <stdbool.h>

//elfFile_size
//elfFile_eof
//CSV handling

bool elfFile_exists(const char *filename){
	FILE *fp = fopen(filename, "r");
	if(!fp) return false;
	fseek(fp, -1, SEEK_CUR);
	return true;
}
