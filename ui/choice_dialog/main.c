#include <stdio.h>

#include <elf_choice_dialog.h>

void test1(){
	ElfChoiceDialog *elf = elfChoiceDialog_new();

	elfChoiceDialog_printInternal(elf);

	elfChoiceDialog_setHeader(elf, "");
	//elfChoiceDialog_setHeader(elf, NULL);
	elfChoiceDialog_setHeader(elf, "My Beautiful Header");
	elfChoiceDialog_printInternal(elf);

	elfChoiceDialog_setText(elf, "");
	//elfChoiceDialog_setText(elf, NULL);
	elfChoiceDialog_setText(elf, "My Beautiful Prompt Text");
	elfChoiceDialog_printInternal(elf);

	elfChoiceDialog_addChoice(elf, NULL);
	elfChoiceDialog_addChoice(elf, "");
	elfChoiceDialog_addChoice(elf, "Choice 2");
	elfChoiceDialog_addChoice(elf, "Choice 3");
	elfChoiceDialog_printInternal(elf);

	elfChoiceDialog_destroy(&elf);
}

int main(int argc, char *argv[]){
	test1();

	return 0;
}
