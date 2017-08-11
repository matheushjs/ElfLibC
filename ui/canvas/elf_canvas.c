#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)
#define ELF_MAX(X,Y) ((X)>(Y)?X:Y)
#define ELF_MIN(X,Y) ((X)<(Y)?X:Y)

typedef struct _ElfCanvas {
	char **matrix;
	int w;  // Width
	int h;  // Height
} ElfCanvas;

/* Static function declarations */
static inline char *new_row(int size);
/* */

// Returns a new char vector filled with blank spaces.
static inline
char *new_row(int size){
	char *result = malloc(size * sizeof(char));
	int i;
	for(i = 0; i < size; i++)
		result[i] = ' ';
	return result;
}

// Documented in header file.
ElfCanvas *elfCanvas_new(int width, int height){
	ElfCanvas *elf;

	elf = malloc(sizeof(ElfCanvas));
	
	elf->w = width  > 0 ? width  : 1;
	elf->h = height > 0 ? height : 1;
	elf->matrix = malloc(sizeof(char *) * elf->h);

	int i;
	for(i = 0; i < elf->h; i++)
		elf->matrix[i] = new_row(elf->w);

	return elf;
}

// Documented in header file.
void elfCanvas_destroy(ElfCanvas **elf_p){
	ElfCanvas *elf = *elf_p;
	if(elf){
		int i;
		for(i = 0; i < elf->h; i++)
			free(elf->matrix[i]);
		free(elf->matrix);
		free(elf);
		*elf_p = NULL;
	}
}

// Documented in header file.
int elfCanvas_getWidth(const ElfCanvas *elf){
	return elf->w;
}

// Documented in header file.
int elfCanvas_getHeight(const ElfCanvas *elf){
	return elf->h;
}

// Documented in header file.
void elfCanvas_fprint(const ElfCanvas *elf, FILE *fp){
	int i;
	for(i = 0; i < elf->h; i++){
		fwrite(elf->matrix[i], sizeof(char), elf->w, fp);
		fputc('\n', fp);
	}
}

// Documented in header file.
void elfCanvas_print(const ElfCanvas *elf){
	elfCanvas_fprint(elf, stdout);
}

// Documented in header file.
void elfCanvas_drawChar(const ElfCanvas *elf, int w, int h, char c){
	if(w < 0 || w >= elf->w || h < 0 || h >= elf->h) ELF_DIE("Invalid arguments");
	elf->matrix[h][w] = c;
}

// Documented in header file.
void elfCanvas_drawText(ElfCanvas *elf, int w, int h, const char *str){
	if(w < 0 || w >= elf->w || h < 0 || h >= elf->h) ELF_DIE("Invalid arguments");

	int i, size;
	
	size = strlen(str);
	for(i = 0; i < size && (w+i) < elf->w; i++){
		elf->matrix[h][w+i] = str[i];
	}
}

// Documented in header file.
void elfCanvas_drawText_v(ElfCanvas *elf, int w, int h, const char *str){
	if(w < 0 || w >= elf->w || h < 0 || h >= elf->h) ELF_DIE("Invalid arguments");

	int i, size;
	
	size = strlen(str);
	for(i = 0; i < size && (h+i) < elf->h; i++){
		elf->matrix[h+i][w] = str[i];
	}
}

// Documented in header file.
void elfCanvas_fillRow_span(ElfCanvas *elf, int w1, int w2, int h, char c){
	if(w1 < 0 || w2 < 0
	|| w1 > w2
	|| h < 0 || h >= elf->h) ELF_DIE("Invalid arguments");

	w1 = ELF_MIN(w1, elf->w);
	w2 = ELF_MIN(w2, elf->w-1);

	while(w1 <= w2)
		elf->matrix[h][w1++] = c;
}

// Documented in header file.
void elfCanvas_fillRow(ElfCanvas *elf, int h, char c){
	elfCanvas_fillRow_span(elf, 0, elf->w-1, h, c);
}

// Documented in header file.
void elfCanvas_fillCol_span(ElfCanvas *elf, int h1, int h2, int w, char c){
	if(h1 < 0 || h2 < 0
	|| h1 > h2
	|| w < 0 || w >= elf->w) ELF_DIE("Invalid arguments");

	h1 = ELF_MIN(h1, elf->h);
	h2 = ELF_MIN(h2, elf->h-1);

	while(h1 <= h2)
		elf->matrix[h1++][w] = c;
}

// Documented in header file.
void elfCanvas_fillCol(ElfCanvas *elf, int w, char c){
	elfCanvas_fillCol_span(elf, 0, elf->h-1, w, c);
}

