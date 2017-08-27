#ifndef _ELF_CHOICE_DIALOG_H
#define _ELF_CHOICE_DIALOG_H

typedef struct _ElfChoiceDialog ElfChoiceDialog;

ElfChoiceDialog *elfChoiceDialog_new();
void elfChoiceDialog_destroy(ElfChoiceDialog **elf_p);

/* DOCUMENTATION

	This library provides the structure ElfChoiceDialog, that you can use to build a simple interface with
	  a certain amount of numbered choices, and the user has to choose one.
	When the object is created, the interface is blank, with just a frame; the user of the structure can
	  sequentially add choices, header, explanation text and so on.
	The library tries to provide reasonable default values, which can be changed by the user, if desired.
	The interface should look like this:

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

typedef ElfChoiceDialog;
	Structure that holds the user interface.

ElfChoiceDialog *elfChoiceDialog_new();
	Returns a newly allocated ElfChoiceDialog.

void elfChoiceDialog_destroy(ElfChoiceDialog **elf_p);
	Frees all memory allocated for the given ElfChoiceDialog.

*/

#endif
