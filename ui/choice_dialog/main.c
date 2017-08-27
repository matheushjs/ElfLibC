#include <stdio.h>

#include <elf_choice_dialog.h>

void test1(){
	ElfChoiceDialog *elf = elfChoiceDialog_new();

	elfChoiceDialog_destroy(&elf);
}

int main(int argc, char *argv[]){
	test1();

	return 0;
}
