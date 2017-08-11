#ifndef _ELF_CANVAS_H
#define _ELF_CANVAS_H

typedef struct _ElfCanvas ElfCanvas;

ElfCanvas *elfCanvas_new_wh(int width, int height);
void elfCanvas_destroy(ElfCanvas **elf_p);

int elfCanvas_getWidth(const ElfCanvas *elf);
int elfCanvas_getHeight(const ElfCanvas *elf);

/* DOCUMENTATION

typedef ElfCanvas;
	Structure that holds a matrix of characters which can be printed to a file at any point.

ElfCanvas *elfCanvas_new_wh(int width, int height);
	Returns a new ElfCanvas.

void elfCanvas_destroy(ElfCanvas **elf_p);
	Frees all memory allocated for the given ElfCanvas.

int elfCanvas_getWidth(const ElfCanvas *elf);
int elfCanvas_getHeight(const ElfCanvas *elf);
	Return the current measures of the canvas.

*/

#endif
