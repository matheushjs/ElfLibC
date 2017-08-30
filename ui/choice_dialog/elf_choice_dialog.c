#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <elf_canvas.h>
#include <elf_string.h> // Manipulation of non-utf8 strings

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)

#define DEFAULT_WIDTH 70

typedef struct _ElfChoiceDialog {
	char *header;    // Header string. Should be optional
	char *text;      // Text string. Should be mandatory

	char **choices;  // Choices to give the user
	int choiceCount; // Number of choices (Maximum of 99 choices)
	
	char *choiceZero;   // User can specify the choice 0 (usually "leave" or "cancel")

	char *interface; // Stores the last built interface, in case user requests it multiple times.
	int width;       // width, in characters, of the interface. Should be optional (but with a nice default value).
	bool interfaceInval;  // Checks if canvas exist and if it's valid.
	                      // It becomes invallid if user changed the dialog since last printing.
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

	elf->interface = NULL;
	elf->width = DEFAULT_WIDTH;
	elf->interfaceInval = true;

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
		free(elf->interface);

		free(elf);
		*elf_p = NULL;
	}
}

// Documented in header file.
void elfChoiceDialog_setHeader(ElfChoiceDialog *elf, const char *text){
	if(elf->header) free(elf->header);
	elf->header = elfString_dup(text);
}

// Documented in header file.
void elfChoiceDialog_setText(ElfChoiceDialog *elf, const char *text){
	if(elf->text) free(elf->text);
	elf->text = elfString_dup(text);
}

// Documented in header file.
int elfChoiceDialog_addChoice(ElfChoiceDialog *elf, const char *text){
	// Can't be null
	if(!text) return -1;

	// Expand choice array
	elf->choiceCount += 1;
	elf->choices = (char **) realloc(elf->choices, sizeof(char *) * elf->choiceCount);
	elf->choices[elf->choiceCount - 1] = elfString_dup(text);

	return elf->choiceCount;
}

// Documented in header file.
void elfChoiceDialog_removeChoice(ElfChoiceDialog *elf, int choiceNum){
	int i, n;

	if(choiceNum <= 0 || choiceNum > elf->choiceCount)
		ELF_DIE("Invallid choice number.");


	// We want to erase choice (choiceNum - 1), since choices here start with index 1.
	free(elf->choices[choiceNum - 1]);
	for(i = choiceNum - 1, n = elf->choiceCount - 1; i < n; i++){
		// Shift choices to the left
		elf->choices[i] = elf->choices[i+1];
	}

	// Shrink array
	elf->choiceCount -= 1;
	elf->choices = (char **) realloc(elf->choices, sizeof(char *) * elf->choiceCount);
	
	// failsafe because of realloc() documentation.
	if(elf->choiceCount == 0)
		elf->choices = NULL;
}

// Documented in header file.
void elfChoiceDialog_changeChoice(ElfChoiceDialog *elf, int choiceNum, const char *text){
	if(choiceNum <= 0 || choiceNum > elf->choiceCount)
		ELF_DIE("Invallid choice number.");

	if(text == NULL)
		ELF_DIE("Received NULL pointer.");

	free(elf->choices[choiceNum - 1]);
	elf->choices[choiceNum - 1] = elfString_dup(text);
}

// Documented in header file.
void elfChoiceDialog_printInternal(const ElfChoiceDialog *elf){
	if(elf->header)
		printf("Header: %s\n", elf->header);
	if(elf->text)
		printf("Text: %s\n", elf->text);

	int i, n;
	for(i = 0, n = elf->choiceCount; i < n; i++){
		printf("Choice %d: %s\n", i+1, elf->choices[i]);
	}
	
	if(elf->choiceZero)
		printf("Choice 0: %s\n", elf->choiceZero);

	printf("Width: %d\n", elf->width);
}

/*
 * TODO
 */

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

int elfChoiceDialog_prompt(ElfChoiceDialog *elf, const char *promptStr){
	// Print interface to stdin and return the number typed.
}
