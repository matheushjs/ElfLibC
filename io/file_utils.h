#ifndef _FILE_UTILS_H
#define _FILE_UTILS_H

#include <stdio.h>

typedef enum {FALSE, TRUE} bool;

int count_row(char *);
int count_num_col(char *);
int file_size(FILE *fp);
bool file_exist(char *filename);
bool file_EOF(FILE *fp);

#endif
