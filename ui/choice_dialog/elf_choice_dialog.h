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
#ifndef _ELF_CHOICE_DIALOG_H
#define _ELF_CHOICE_DIALOG_H

typedef struct _ElfChoiceDialog ElfChoiceDialog;

ElfChoiceDialog *elfChoiceDialog_new();
void elfChoiceDialog_destroy(ElfChoiceDialog **elf_p);

void elfChoiceDialog_setHeader(ElfChoiceDialog *elf, const char *text);
void elfChoiceDialog_setText(ElfChoiceDialog *elf, const char *text);

int elfChoiceDialog_addChoice(ElfChoiceDialog *elf, const char *text);
void elfChoiceDialog_removeChoice(ElfChoiceDialog *elf, int choiceNum);
void elfChoiceDialog_changeChoice(ElfChoiceDialog *elf, int choiceNum, const char *text);
void elfChoiceDialog_setChoiceZero(ElfChoiceDialog *elf, const char *text);

void elfChoiceDialog_setWidth(ElfChoiceDialog *elf, int width);

const char *elfChoiceDialog_getInterface(ElfChoiceDialog *elf);

void elfChoiceDialog_printInternal(const ElfChoiceDialog *elf);

// TODO: Remove this
char **test_split(const char *string, int width);


/* DOCUMENTATION

	This library provides the structure ElfChoiceDialog, that you can use to build a simple interface with
	  a certain amount of numbered choices, and the user has to choose one.

	When the object is created, the interface is blank, with just a frame; the user of the structure can
	  sequentially add choices, header, explanation text and so on.

	The library tries to provide reasonable default values, which can be changed by the user, if desired.

	The library works with UTF8 characters, but the user is expected to use only characters of same width.
	Japanese characters, for example, may be wider in the terminal than regular latin1 characters, and using
	  both of them in the same Dialog will mess up the interface.
	Also, graphical characters such as newline or tabs are also forbidden. The Dialog will split big strings
	  into multiple lines whenever needed, so don't use these graphical characters.

	The dialog was designed to hold at most 99 choices. Trying to exceed this number will make the library
	  terminate your program with an error.

	The interface should look like this:

            .------------------------------------------------------.
            |                       Header                         |
            |------------------------------------------------------|
            |           Text                                       |
            |                                                      |
            |           [1] -                                      |
            |           [2] -                                      |
            |           [3] -                                      |
            |           [4] -                                      |
            |           [5] -                                      |
            |           [6] -                                      |
            |           [7] -                                      |
            |          [11] -                                      |  - 11 is just for illustration of multi-digit
            |------------------------------------------------------|
            |           [0] - Exit                                 |
            '------------------------------------------------------'
|-spacing--| |padding||------------- width --------------||padding|


typedef ElfChoiceDialog;
	Structure that holds the user interface.

ElfChoiceDialog *elfChoiceDialog_new();
	Returns a newly allocated ElfChoiceDialog.

void elfChoiceDialog_destroy(ElfChoiceDialog **elf_p);
	Frees all memory allocated for the given ElfChoiceDialog.

void elfChoiceDialog_setHeader(ElfChoiceDialog *elf, const char *text);
	Sets the header text of the interface.
	'text' may be NULL to unset.

void elfChoiceDialog_setText(ElfChoiceDialog *elf, const char *text);
	Sets the text of the interface.
	'text' may be NULL to unset.

int elfChoiceDialog_addChoice(ElfChoiceDialog *elf, const char *text);
	Adds choice 'text' to the interface.
	Will exit the program if more than 99 choices are added.
	Returns the number of the choice added.

void elfChoiceDialog_removeChoice(ElfChoiceDialog *elf, int choiceNum);
	Removes choice with index 'choiceNum' from the Choice Dialog.
	'choiceNum' must be a number higher than or equal to 1.
	Complexity for removing a choice is O(n) on the number of choices.

void elfChoiceDialog_changeChoice(ElfChoiceDialog *elf, int choiceNum, const char *text);
	Changes choice of index 'choiceNum' to be 'text'.
	'choicenum' must be a number higher than or equal to 1.

void elfChoiceDialog_printInternal(const ElfChoiceDialog *elf);
	Prints the internal state of the ElfChoiceDialog to stdout.
	For debugging only.

void elfChoiceDialog_setChoiceZero(ElfChoiceDialog *elf, const char *text);
	Sets the choice number 0 of the given ChoiceDialog.
	'text' can be NULL to unset.

void elfChoiceDialog_setWidth(ElfChoiceDialog *elf, int width);
	Sets the width of the ChoiceDialog.
	The width is the number of characters between the Dialog's side frames, as illustrated
	  on the top of this documentation.
	Must be a number higher than 9. 8 letters are reserved for writing the choices, and 1 letter is
	  reserved for the leading whitespace before writing the choice itself.

const char *elfChoiceDialog_getInterface(ElfChoiceDialog *elf);
	Returns a string that represents the whole Dialog text.
	Printing the returned string to the terminal should present the dialog to the user.
	The dialog ends with a newline character.
	The dialog is re-built only when needed, which is when the dialog is invallidated by a change made
	  by any (most of them) function call.
*/

#endif
