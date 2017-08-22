#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <elf_canvas.h>
#include <elf_string.h> // Manipulation of non-utf8 strings

typedef struct _ElfChoiceDialog {
	char *header;    // Header string. Should be optional
	char *text;      // Text string. Should be mandatory

	char **options;  // Options to give the user
	int optionCount; // Number of options (Maximum of 100 options)
	
	char *optionZero;   // User can specify the option 0 (usually "leave" or "cancel")

	ElfCanvas *canvas;
	bool invallid; // Checks if canvas exist and if it's valid.
	               // It becomes invallid if user changed the dialog since last printing.
} ElfChoiceDialog;

/*

   .-------------------------.
   |      Header             |
   |-------------------------|
   |    Text:                |
   |                         |
   |  [1] -                  |
   |  [2] -                  |
   |  [3] -                  |
   |  [4] -                  |
   |  [5] -                  |
   |  [6] -                  |
   |  [7] -                  |
   | [11] -                  |  - 11 is just for illustration
   |-------------------------|
   |  [0] - Exit             |
   '-------------------------'

*/
