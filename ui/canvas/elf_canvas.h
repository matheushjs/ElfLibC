#ifndef _ELF_CANVAS_H
#define _ELF_CANVAS_H

typedef struct _ElfCanvas ElfCanvas;

ElfCanvas *elfCanvas_new();
ElfCanvas *elfCanvas_new_h(int height);
ElfCanvas *elfCanvas_new_w(int width);
ElfCanvas *elfCanvas_new_wh(int width, int height);

void elfCanvas_destroy(ElfCanvas **elf_p);


/* DOCUMENTATION

typedef ElfCanvas;
	Structure that holds a matrix of characters which can be printed to a file at any point.


ElfCanvas *elfCanvas_new();
	Returns a new ElfCanvas whose height and width will be allocated on-the-fly as the user
	  adds characters to canvas.

ElfCanvas *elfCanvas_new_h(int height);
ElfCanvas *elfCanvas_new_w(int width);
ElfCanvas *elfCanvas_new_wh(int width, int height);
	Returns a new ElfCanvas whose height and/or width is fixed.
	A negative value means that measure is not fixed.


void elfCanvas_destroy(ElfCanvas **elf_p);
	Frees all memory allocated for the given ElfCanvas.

*/

#endif
