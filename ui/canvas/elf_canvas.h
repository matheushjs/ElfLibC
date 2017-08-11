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

void elfCanvas_drawChar(const ElfCanvas *elf, int w, int h, char c);
void elfCanvas_drawText(ElfCanvas *elf, int w, int h, const char *str);
void elfCanvas_drawText_v(ElfCanvas *elf, int w, int h, const char *str);

void elfCanvas_fillRow_span(ElfCanvas *elf, int w1, int w2, int h, char c);
void elfCanvas_fillRow(ElfCanvas *elf, int h, char c);

void elfCanvas_fillCol_span(ElfCanvas *elf, int h1, int h2, int w, char c);
void elfCanvas_fillCol(ElfCanvas *elf, int w, char c);

/* DOCUMENTATION

typedef ElfCanvas;
	Structure that holds a matrix of characters which can be printed to a file at any point.

ElfCanvas *elfCanvas_new(int width, int height);
	Returns a new ElfCanvas.

void elfCanvas_destroy(ElfCanvas **elf_p);
	Frees all memory allocated for the given ElfCanvas.

int elfCanvas_getWidth(const ElfCanvas *elf);
int elfCanvas_getHeight(const ElfCanvas *elf);
	Return the current measures of the canvas.

void elfCanvas_fprint(const ElfCanvas *elf, FILE *fp);
	Prints the canvas to file 'fp', separating each row with a newline character.

void elfCanvas_print(const ElfCanvas *elf);
	Prints the canvas to standard output.

void elfCanvas_drawChar(const ElfCanvas *elf, int w, int h, char c);
	Adds a character to the canvas at position (w, h).

void elfCanvas_drawText(ElfCanvas *elf, int w, int h, const char *str);
	Draws the string 'str' into the canvas horizontally, beginning at position (w,h).
	If the string doesn't fit into the canvas, the trailing characters will be ignored.

void elfCanvas_drawText_v(ElfCanvas *elf, int w, int h, const char *str);
	Draws the string 'str 'into the canvas vertically, beginning at position (w,h).
	If the string doesn't fit into the canvas, the trailing characters will be ignored.

void elfCanvas_fillRow_span(ElfCanvas *elf, int w1, int w2, int h, char c);
	Fills the row 'h' with character 'c', beginning from column w1 to w2.

void elfCanvas_fillRow(ElfCanvas *elf, int h, char c);
	Fills the whole line 'h' with character 'c'.

void elfCanvas_fillCol_span(ElfCanvas *elf, int h1, int h2, int w, char c);
	Fills the column 'w' with character 'c', starting from row h1 to h2.

void elfCanvas_fillCol(ElfCanvas *elf, int w, char c);
	Fills the whole column 'w' with character 'c'.
*/

#endif
