/*
 * MIT License
 * 
 * Copyright (c) 2018 Matheus H. J. Saldanha <mhjsaldanha@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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

/* File-local declarations along this source file*/
typedef struct _TokenInfo TokenInfo;
static void token_init(TokenInfo *tok, char **tokens);
static void token_next(TokenInfo *tok);
static bool token_empty(TokenInfo *tok);
static char **fit_string_to_width(const char *string, int width, int *size_out);
static void free_strings(char **strings);
/* END */

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
	if(elf->choiceCount == 99)
		ELF_DIE("Cannot have more than 99 choices.");

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
	if(width <= 9)
		ELF_DIE("Invallid width. Must be higher than 9.");
	elf->width = width;
	elf->isValid = false;
}

/*
 * TODO
 * 
 * TODO: Separate Header, choiceZero and middleBoard in 3 different canvas
 */

// Structure for handling an array of strings, where each string is a single token (a word).
typedef struct _TokenInfo {
	char *iter;
	int len;

	char **tokens;
	int curToken;
} TokenInfo;

// Initializes the structure for handling the array 'tokens'.
static
void token_init(TokenInfo *tok, char **tokens){
	tok->tokens = tokens;
	tok->curToken = 0;

	tok->iter = tok->tokens[tok->curToken];
	if(tok->iter != NULL)
		tok->len = elfString_len_utf8(tok->iter);
	else
		tok->len = -1;
}

// Advances to the next token in the array
static
void token_next(TokenInfo *tok){
	tok->curToken += 1;
	tok->iter = tok->tokens[tok->curToken];
	if(tok->iter != NULL)
		tok->len = elfString_len_utf8(tok->iter);
	else
		tok->len = -1;
}

// Returns 'true' if there are no more tokens
static
bool token_empty(TokenInfo *tok){
	return (bool) (tok->len == -1);
}

// Splits 'string' nicely into multiple lines, in order to fit within width of 'width'.
// UTF8 characters are taking into consideration.
// If 'size_out' is not NULL, stores in it the number of lines retuned.
// Returns a NULL-terminated array of strings containing the lines that 'string' was split into.
static
char **fit_string_to_width(const char *string, int width, int *size_out){
	// Must return a NULL-terminated array of strings, each containing
	//   a fragment of 'string' that doesn't exceed length 'width'.
	
	TokenInfo token;
	char **lines;
	int lineLen, lineCount;
	ElfStringBuf *line;

	// Get tokens and initialize TokenInfo
	token_init(&token, elfString_split_bag_utf8(string, " "));

	// Prepare for using realloc.
	lines = NULL;
	lineCount = 0; // Number of lines
	
	// Line being constructed
	line = elfStringBuf_new();

	while(!token_empty(&token)){

		// Will break the loop once the NULL token is reached
		lineLen = 0;   // Characters in line being built

		// Add first token to the current line
		if(token.len <= width){
			elfStringBuf_appendString(line, token.iter);
			lineLen += token.len;
			
			// Get next token
			token_next(&token);
		}

		// Add as many tokens as possible to current line
		while(!token_empty(&token)){
			if(lineLen + token.len + 1 <= width){ // +1 because of whitespace
				elfStringBuf_appendChar(line, ' ');
				elfStringBuf_appendString(line, token.iter);
				lineLen += token.len + 1;
				token_next(&token);
			} else break;
		}

		// Analyze last inserted token
		if(token_empty(&token)){
			// Do nothing
		} else if(width - lineLen <= 1){
			// Do nothing, because only a whitespace fits in current line
		} else if(token.len > width) {
			// If token not inserted does not fit in a single line
			// We add as many characters as possible to current line

			// Add whitespace only if line is not empty
			if(lineLen != 0){
				elfStringBuf_appendChar(line, ' ');
				lineLen += 1;
			}

			// Add characters
			while(lineLen != width){
				elfStringBuf_appendChar(line, *token.iter);
				lineLen += 1;
				token.iter++;
				token.len--;
			}
			// Leave the shortened token for the next while-iteration
		} else {
			// If token does fit in a line, we just leave it for the next while iteration
		}

		// Conclude creation of current line
		lineCount += 1;
		lines = realloc(lines, sizeof(char *) * lineCount);
		lines[lineCount - 1] = elfStringBuf_makeString(line, NULL); // This resets the buffer
		
	}

	// Free tokens
	int i;
	for(i = 0; token.tokens[i] != NULL; i++)
		free(token.tokens[i]);
	free(token.tokens);

	// Add terminating NULL line
	lines = realloc(lines, sizeof(char *) * (lineCount + 1));
	lines[lineCount] = NULL;

	if(size_out)
		*size_out = lineCount;

	elfStringBuf_destroy(&line);
	return lines;
}

// TODO: Remove this
char **test_split(const char *string, int width){
	return fit_string_to_width(string, width, NULL);
}

// Frees a NULL-terminated array of strings
static
void free_strings(char **strings){
	int i;
	for(i = 0; strings[i] != NULL; i++)
		free(strings[i]);
	free(strings);
}


/* TODO: Implement this
 *
 * Some clarifications on building the dialog
 * The dialog will be built using 4 ElfCanvas: Header, TextBox, MiddleBox and ZeroBox
 *
 * The header will be built as something like this:
 * 
     |                                                      |
     |                       Header                         |
     |                                                      |
 *
 * The TextBox:
 *
     |           Text                                       |
 *
 * The MiddleBox:
 *
     |           [1] -                                      |
     |           [2] -                                      |
     |           [3] -                                      |
     |           [4] -                                      |
 *
 * The ZeroBox:
 *
     |           [0] - Exit                                 |
 *
 * And then each canvas will be glued one to the other, delimiting them with frames
 */

const
char *elfChoiceDialog_getInterface(ElfChoiceDialog *elf){
	if(elf->isValid)
		return (const char *) elf->interface;

	// Structure that holds an array of strings, which consist of lines that should be
	//   placed vertically.
	typedef struct _VerticalText {
		char **lines;
		int nLines;
	} VerticalText;

	VerticalText header;
	VerticalText text;
	VerticalText *choices;
	VerticalText choiceZero;
	int i, j, n;
	
	// Split header, text and choiceZero
	header.lines     = fit_string_to_width(elf->header,     elf->width, &header.nLines);
	text.lines       = fit_string_to_width(elf->text,       elf->width - 8, &text.nLines);
	choiceZero.lines = fit_string_to_width(elf->choiceZero, elf->width - 8, &choiceZero.nLines);
	
	// Split all the choices
	choices = malloc(sizeof(VerticalText) * elf->choiceCount);
	int choiceWidth = elf->width - 8;
	for(i = 0, n = elf->choiceCount; i < n; i++){
		choices[i].lines = fit_string_to_width(elf->choices[i], choiceWidth, &choices[i].nLines);
	}

	// Calculate canvas height
	int canvasHeight = 1 + header.nLines + 1 + text.nLines + 1;
	for(i = 0, n = elf->choiceCount; i < n; i++)
		canvasHeight += choices[i].nLines;
	canvasHeight += 1 + choiceZero.nLines + 1;

	// Calculate canvas width
	int canvasWidth;
	canvasWidth = elf->spacing + 1 + elf->sidePadding + elf->width + elf->sidePadding + 1;
	
	// Create canvas
	ElfCanvas *canvas = elfCanvas_new(canvasWidth, canvasHeight);

	// Draw top and bottom frames
	elfCanvas_fillRow_span(canvas, elf->spacing, canvasWidth-1, 0, "-");
	elfCanvas_fillRow_span(canvas, elf->spacing, canvasWidth-1, canvasHeight-1, "-");

	// Draw frame below header
	int headerFrameHeight = 1 + header.nLines + 1 - 1; // -1 cuz of 0-indexing
	elfCanvas_fillRow_span(canvas, elf->spacing, canvasWidth-1, headerFrameHeight, "-");

	// Draw frame above choiceZero
	int choiceZeroFrameHeight = headerFrameHeight;
	choiceZeroFrameHeight += text.nLines + 1;
	for(i = 0, n = elf->choiceCount; i < n; i++)
		choiceZeroFrameHeight += choices[i].nLines;
	choiceZeroFrameHeight += 1; // Doesn't need -1 because headerFrameHeight already consider it.
	elfCanvas_fillRow_span(canvas, elf->spacing, canvasWidth-1, choiceZeroFrameHeight, "-");

	// Draw left and right frames
	elfCanvas_fillCol_span(canvas, 0, canvasHeight-1, elf->spacing, "|");
	elfCanvas_fillCol_span(canvas, 0, canvasHeight-1, canvasWidth-1, "|");

	// Draw edge points
	elfCanvas_drawChar(canvas, elf->spacing , 0, ".");
	elfCanvas_drawChar(canvas, canvasWidth-1, 0, ".");
	elfCanvas_drawChar(canvas, elf->spacing , canvasHeight-1, "'");
	elfCanvas_drawChar(canvas, canvasWidth-1, canvasHeight-1, "'");

	// Draw header text
	int h0 = 1; //first row to write
	int w0 = elf->spacing + 1 + elf->sidePadding;
	for(i = 0; i < header.nLines; i++){
		elfCanvas_drawText(canvas, w0, h0 + i, header.lines[i]);
	}
	
	// Draw secondary text
	h0 = 1 + header.nLines + 1; //first row to write
	w0 = w0; // Keep old value
	for(i = 0; i < text.nLines; i++){
		elfCanvas_drawText(canvas, w0, h0 + i, text.lines[i]);
	}

	// Draw options
	h0 = 1 + header.nLines + 1 + text.nLines + 1;
	w0 = w0; // Keep old value
	int w1 = w0 + 8; // Width for each option's text
	int nDigits = 1; // keep track of number of digits
	for(i = 0; i < elf->choiceCount; i++){
		// For each choice

		// Increment digit count if needed
		if( (i+1)%10 == 0 ) nDigits++;

		// Make choice number string
		char choiceStr[10];
		sprintf(choiceStr, "[%d] - ", i+1);

		// Write choice number string
		elfCanvas_drawText(canvas, w0 + 3 - nDigits, h0, choiceStr);

		// Write choice lines
		for(j = 0; j < choices[i].nLines; j++){
			elfCanvas_drawText(canvas, w1, h0 + j, choices[i].lines[j]);
		}

		// Increment height
		h0 += choices[i].nLines;
	}

	// Draw choice zero
	h0 += 1; // Keep old value and jump the frame
	w0 = w0; // keep old
	w1 = w1; // keep old
	elfCanvas_drawText(canvas, w0, h0, "  [0] - ");
	for(i = 0; i < choiceZero.nLines; i++)
		elfCanvas_drawText(canvas, w1, h0 + i, choiceZero.lines[i]);

	char *retval = (char *) elfCanvas_buildString(canvas);
	elfCanvas_destroy(&canvas);

	if(elf->interface)
		free(elf->interface);
	elf->interface = retval;
	elf->isValid = true;
	
	// Free all splitted lines
	free_strings(header.lines);
	free_strings(text.lines);
	free_strings(choiceZero.lines);
	for(i = 0, n = elf->choiceCount; i < n; i++)
		free_strings(choices[i].lines);
	free(choices);

	return (const char *) elf->interface;
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
