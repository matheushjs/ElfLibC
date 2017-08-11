#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct _ElfCanvas {
	char **matrix;

	int w;  // Width
	int h;  // Height

	int wCapacity; // Size alloc'ed for each row, in number of characters.

	bool fixHeight; // Canvas has fixed height value.
	bool fixWidth;  // Canvas has fixed width value.
} ElfCanvas;

/* TODO:
 *
 * NOO! The Canvas should not be auto-resizable!!!
 * It'd make it easier to use? YES
 * It'd make it harder to implement? YESSSSSSSS TONS!!
 * Would it be really useful? Nope, not really. This is made for terminal applications, so
 *   the canvas user will probably know how big the canvas has to be.
 *
 * So don't do it.
 */



/* Static function declarations */
static inline char *new_row(int size);
/* */

// Returns a new char vector filled with blank spaces.
static inline
char *new_row(int size){
	static int val = 32 + (32<<8) + (32<<16) + (32<<24); //32 is whitespace ascii
	char *result = malloc(size * sizeof(char));
	memset(result, val, size * sizeof(char));
	return result;
}

// Documented in header file.
ElfCanvas *elfCanvas_new_wh(int width, int height){
	ElfCanvas *elf;

	elf = malloc(sizeof(ElfCanvas));
	
	elf->fixWidth  = width  >= 0 ? true : false;
	elf->fixHeight = height >= 0 ? true : false;

	elf->w = width  >= 0 ? width  : 0;
	elf->h = height >= 0 ? height : 0;

	int capacity = 8;
	while(capacity < elf->w) capacity <<= 1;
	elf->wCapacity = capacity;

	if(elf->h > 0)
		elf->matrix = malloc(sizeof(char *) * elf->h);
	else
		elf->matrix = NULL;

	int i;
	for(i = 0; i < elf->h; i++)
		elf->matrix[i] = new_row(elf->wCapacity);

	return elf;
}

// Documented in header file.
ElfCanvas *elfCanvas_new_w(int width){
	return elfCanvas_new_wh(width, -1);
}

// Documented in header file.
ElfCanvas *elfCanvas_new_h(int height){
	return elfCanvas_new_wh(-1, height);
}

// Documented in header file.
ElfCanvas *elfCanvas_new(){
	return elfCanvas_new_wh(-1, -1);
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
