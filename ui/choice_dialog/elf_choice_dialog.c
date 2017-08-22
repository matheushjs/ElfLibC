#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <elf_canvas.h>
#include <elf_string.h> // Manipulation of non-utf8 strings

typedef struct _ElfChoiceDialog {
	char **options;  // Options to give the user
	int optionCount; // Number of options
	
	char *optionZero;   // User can specify the option 0 (usually "leave" or "cancel"
	bool hasOptionZero; // Stores whether user gave option 0 or not.

	ElfCanvas *canvas;
	bool invallid; // Checks if canvas exist and if it's valid.
	               // It becomes invallid if user changed the dialog since last printing.
} ElfChoiceDialog;
