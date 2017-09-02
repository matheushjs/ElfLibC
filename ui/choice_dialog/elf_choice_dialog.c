#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <elf_canvas.h>
#include <elf_string_buf.h>
#include <elf_string.h> // Manipulation of non-utf8 strings

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)

#define DEFAULT_WIDTH 70
#define DEFAULT_SPACING 5
#define DEFAULT_PADDING 2

typedef struct _ElfChoiceDialog {
	char *header;    // Header string. Should be optional
	char *text;      // Text string. Should be mandatory

	char **choices;  // Choices to give the user
	int choiceCount; // Number of choices (Maximum of 99 choices)
	
	char *choiceZero;   // User can specify the choice 0 (usually "leave" or "cancel")

	char *interface; // Stores the last built interface, in case user requests it multiple times.
	bool isValid;    // Checks if canvas exist and if it's valid.
	                   // It becomes invallid if user changed the dialog since last printing.
	
	int width;       // width, in characters, of the interface. Optional with default.
	int spacing;     // Distance to terminal's left side. Optional with default.
	int sidePadding; // Distance between the content and the side borders. Optional with default.
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
	elf->isValid = false;
	
	elf->width = DEFAULT_WIDTH;
	elf->spacing = DEFAULT_SPACING;
	elf->sidePadding = DEFAULT_PADDING;

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
	elf->isValid = false;
}

// Documented in header file.
void elfChoiceDialog_setText(ElfChoiceDialog *elf, const char *text){
	if(elf->text) free(elf->text);
	elf->text = elfString_dup(text);
	elf->isValid = false;
}

// Documented in header file.
int elfChoiceDialog_addChoice(ElfChoiceDialog *elf, const char *text){
	// Can't be null
	if(!text) return -1;

	// Expand choice array
	elf->choiceCount += 1;
	elf->choices = (char **) realloc(elf->choices, sizeof(char *) * elf->choiceCount);
	elf->choices[elf->choiceCount - 1] = elfString_dup(text);

	elf->isValid = false;
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
	elf->isValid = false;
}

// Documented in header file.
void elfChoiceDialog_changeChoice(ElfChoiceDialog *elf, int choiceNum, const char *text){
	if(choiceNum <= 0 || choiceNum > elf->choiceCount)
		ELF_DIE("Invallid choice number.");

	if(text == NULL)
		ELF_DIE("Received NULL pointer.");

	free(elf->choices[choiceNum - 1]);
	elf->choices[choiceNum - 1] = elfString_dup(text);
	elf->isValid = false;
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

	printf("Width  : %d\n", elf->width);
	printf("Spacing: %d\n", elf->spacing);
	printf("Padding: %d\n", elf->sidePadding);
}

// Documented in header file.
void elfChoiceDialog_setChoiceZero(ElfChoiceDialog *elf, const char *text){
	free(elf->choiceZero);
	elf->choiceZero = elfString_dup(text);
	elf->isValid = false;
}

// Documented in header file.
void elfChoiceDialog_setWidth(ElfChoiceDialog *elf, int width){
	if(width <= 0)
		ELF_DIE("Invallid choice number.");
	elf->width = width;
	elf->isValid = false;
}

/*
 * TODO
 */

static
char **fit_string_to_width(char *string, int width){
	// Must return a NULL-terminated array of strings, each containing
	//   a fragment of 'string' that doesn't exceed length 'width'.
	
	/* How it will work
	 *
	 * 1) We will have a 'while' loop that builds just single line
	 * 2) Everything will work based on the concept of token
	 * 
	 * Steps:
	 *
	 * 1) Create new line
	 * 2) Get next token
	 * 3) If token fits in the line, add it
	 * 4) If doesn't fit in the line:
	 *  	4.1) If token is bigger than width, add as much of it in the current line
	 *  	     and edit the current token to have only the characters that still
	 *  	     need to be added
	 *  	4.2) If the token is smaller than 'width', finish current line and pass
	 *  	     current token fully to next iteration. Go to step 1.
	 */

	ElfStringBuf *line;
	char **tokens, *token, **iter;
	char **lines;
	int lineCount, lineLen, tokenLen;
	
	// Array of tokens removed from 'string'
	tokens = elfString_split_bag_utf8(string, " ");
	
	// Line being constructed
	line = elfStringBuf_new();
	lineLen = 0;   // Characters in line being built

	// Prepare for using realloc.
	lines = NULL;
	lineCount = 0; // Number of lines

	iter = tokens;
	while(*iter != NULL){
/*
		lineLen = 0;

		// Add first token to the current line
		tokenLen = elfString_len_utf8(*iter);
		if(tokenLen <= width){
			elfStringBuf_appendString(line, *iter);
			lineLen += tokenLen;
			iter += 1;
		}
		
		// Add as many tokens as possible to the current line
		// Note that if first token isn't inserted, this one isn't either
		while(*iter != NULL){
			tokenLen = elfString_len_utf8(*iter);
			if(tokenLen + 1 <= width){ // +1 because of whitespace
				// if token + whitespace fits in the line, add them.
				elfStringBuf_appendChar(line, ' ');
				elfStringBuf_appendString(line, *iter);
				lineLen += tokenLen + 1;
				iter += 1;
			} else break;
		}

		// Analyze last not-inserted token
		if(*iter == NULL){
			break;
		} else {
			tokenLen = elfString_len_utf8(*iter);
			
			if(tokenLen > width){
				// If token doesn't fit in a single line
				int usedChars = 0;
				
				// Check if current line can hold whitespace + at least 1 character
				if(width - lineLen >= 2){
					elfStringBuf_appendChar(line, ' ');
					lineLen += 1;

					while(lineLen != width){
						elfStringBuf_appendChar(line, (*iter)[usedChars]);
						lineLen += 1;
						usedChars += 1;
					}
				}

				// Add current line to lines
				lineCount += 1;
				lines = realloc(lines, sizeof(char *) * lineCount);
				lines[lineCount - 1] = elfStringBuf_makeString(line, NULL); // This resets the StringBuf
				lineLen = 0;

				// Fill following lines
				while( usedChars != tokenLen){

					// Check if needs new line
					if(lineLen == width){
						lineCount += 1;
						lines = realloc(lines, sizeof(char *) * lineCount);
						lines[lineCount - 1] = elfStringBuf_makeString(line, NULL); // This resets the StringBuf
						lineLen = 0;
					}

					elfStringBuf_appendChar(line, (*iter)[usedChars]);
					usedChars += 1;
					lineLen += 1;
				}

			} else {
				// If token does fit in a single line, create another line
				lineCount += 1;
				lines = realloc(lines, sizeof(char *) * lineCount);
				lines[lineCount - 1] = elfStringBuf_makeString(line, NULL); // This resets the StringBuf
				lineLen = 0;

			}
		}
*/

	}

	elfStringBuf_destroy(&line);
	return NULL;
}

const
char *elfChoiceDialog_getInterface(ElfChoiceDialog *elf){
	return NULL;
}

void elfChoiceDialog_fprint(ElfChoiceDialog *elf, FILE *fp){
	fprintf(fp, "%s", elfChoiceDialog_getInterface(elf));
}

void elfChoiceDialog_print(ElfChoiceDialog *elf){
	elfChoiceDialog_fprint(elf, stdin);
}

int elfChoiceDialog_prompt(ElfChoiceDialog *elf, const char *promptStr){
	// Print interface to stdin and return the number typed.
	return 0;
}
