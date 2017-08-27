#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <elf_canvas.h>
#include <elf_string.h> // Manipulation of non-utf8 strings

#define DEFAULT_WIDTH 70

typedef struct _ElfChoiceDialog {
	char *header;    // Header string. Should be optional
	char *text;      // Text string. Should be mandatory

	char **choices;  // Choices to give the user
	int choiceCount; // Number of choices (Maximum of 99 choices)
	
	char *choiceZero;   // User can specify the choice 0 (usually "leave" or "cancel")

	ElfCanvas *canvas;
	int width; // width, in characters, of the canvas. Should be optional (but with a nice default value).
	bool canvasInvallid;  // Checks if canvas exist and if it's valid.
	                      // It becomes invallid if user changed the dialog since last printing.
	char *builtInterface; // Stores the last build interface, in case user requests it multiple times.
} ElfChoiceDialog;

// Documented in header file.
ElfChoiceDialog *elfChoiceDialog_new(){
	ElfChoiceDialog *elf;
	elf = malloc(sizeof(ElfChoiceDialog));

	elf->header = NULL;
	elf->text = NULL;
	
	elf->choices = NULL;
	elf->choiceCount = 0;

	elf->choiceZero = NULL;

	// Canvas is only created upon a call to any function that returns
	//   the string that represents the interface or that prints it.
	elf->canvas = NULL;
	elf->width = DEFAULT_WIDTH;

	elf->canvasInvallid = true;

	return elf;
}

// Documented in header file.
void elfChoiceDialog_destroy(ElfChoiceDialog **elf_p){
	ElfChoiceDialog *elf = *elf_p;
	if(elf){
		free(elf->header);
		free(elf->text);

		int i;
		for(i = 0; i < elf->choiceCount; i++)
			free(elf->choices[i]);
		free(elf->choices);

		free(elf->choiceZero);

		elfCanvas_destroy(&elf->canvas);

		free(elf);
		*elf_p = NULL;
	}
}

/*
 * TODO
 */

void elfChoiceDialog_setHeader(ElfChoiceDialog *elf, const char *text){

}

void elfChoiceDialog_setText(ElfChoiceDialog *elf, const char *text){

}

int elfChoiceDialog_addChoice(ElfChoiceDialog *elf, const char *text){

}

void elfChoiceDialog_removeChoice(ElfChoiceDialog *elf, int choiceNum){

}

void elfChoiceDialog_changeChoice(ElfChoiceDialog *elf, int choiceNum, const char *text){

}

void elfChoiceDialog_setChoiceZero(ElfChoiceDialog *elf, const char *text){
	// Can accept null
}

void elfChoiceDialog_setWidth(ElfChoiceDialog *elf, int width){

}

const
char *elfChoiceDialog_getInterface(ElfChoiceDialog *elf){

}

void elfChoiceDialog_fprint(ElfChoiceDialog *elf, FILE *fp){

}

void elfChoiceDialog_print(ElfChoiceDialog *elf){
	elfChoiceDialog_fprint(elf, stdin);
}

int elfChoiceDialog_prompt(ElfChoiceDialog *elf){
	// Print interface to stdin and return the number typed.
}
