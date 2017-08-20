#ifndef _ELF_CANVAS_H
#define _ELF_CANVAS_H

#include <stdio.h>

typedef struct _ElfCanvas ElfCanvas;

ElfCanvas *elfCanvas_new(int width, int height);
void elfCanvas_destroy(ElfCanvas **elf_p);

int elfCanvas_getWidth(const ElfCanvas *elf);
int elfCanvas_getHeight(const ElfCanvas *elf);

void elfCanvas_fprint(const ElfCanvas *elf, FILE *fp);
void elfCanvas_print(const ElfCanvas *elf);

void elfCanvas_drawChar(const ElfCanvas *elf, int w, int h, const char *c);
void elfCanvas_drawText(ElfCanvas *elf, int w, int h, const char *str);
void elfCanvas_drawText_v(ElfCanvas *elf, int w, int h, const char *str);

void elfCanvas_fillRow_span(ElfCanvas *elf, int w1, int w2, int h, const char *c);
void elfCanvas_fillRow(ElfCanvas *elf, int h, const char *c);

void elfCanvas_fillCol_span(ElfCanvas *elf, int h1, int h2, int w, const char *c);
void elfCanvas_fillCol(ElfCanvas *elf, int w, const char *c);

/* DOCUMENTATION

typedef ElfCanvas;
	Structure that holds a matrix of characters which can be printed to a file at any point.
	All the characters are expected to be in encoding UTF8, hence why all character arguments
	  are actually received as (char*).
	All characters are expected to have the same width when displayed in whatever displaying
	  device the canvas will be printed on. This means that accented characters probably mix
	  well with ascii characters, but japanese characters wouldn't because they are normally
	  wider.

ElfCanvas *elfCanvas_new(int width, int height);
	Returns a new ElfCanvas that holds 'height' lines of text and 'width' UTF characters
	  per line.

void elfCanvas_destroy(ElfCanvas **elf_p);
	Frees all memory allocated for the given ElfCanvas.

int elfCanvas_getWidth(const ElfCanvas *elf);
int elfCanvas_getHeight(const ElfCanvas *elf);
	Return the current measures of the canvas.

void elfCanvas_fprint(const ElfCanvas *elf, FILE *fp);
	Prints the canvas to file 'fp', separating each row with a newline character.

void elfCanvas_print(const ElfCanvas *elf);
	Prints the canvas to standard output.

void elfCanvas_drawChar(const ElfCanvas *elf, int w, int h, const char *c);
	Adds given UTF character to the canvas at position (w, h).

void elfCanvas_drawText(ElfCanvas *elf, int w, int h, const char *str);
	Draws the UTF string 'str' into the canvas horizontally, beginning at position (w,h).
	If the string doesn't fit into the canvas, the trailing characters will be ignored.

void elfCanvas_drawText_v(ElfCanvas *elf, int w, int h, const char *str);
	Draws the UTF string 'str 'into the canvas vertically, beginning at position (w,h).
	If the string doesn't fit into the canvas, the trailing characters will be ignored.

void elfCanvas_fillRow_span(ElfCanvas *elf, int w1, int w2, int h, const char *c);
	Fills the row 'h' with UTF character 'c', beginning from column w1 to w2.
	If given interval is out of bounds, only the possible space is filled.

void elfCanvas_fillRow(ElfCanvas *elf, int h, const char *c);
	Fills the whole line 'h' with UTF character 'c'.

void elfCanvas_fillCol_span(ElfCanvas *elf, int h1, int h2, int w, const char *c);
	Fills the column 'w' with UTF character 'c', starting from row h1 to h2.
	If given interval is out of bounds, only the possible space is filled.

void elfCanvas_fillCol(ElfCanvas *elf, int w, const char *c);
	Fills the whole column 'w' with UTF character 'c'.
*/

#endif
