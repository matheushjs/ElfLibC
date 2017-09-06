#include <stdio.h>
#include <stdlib.h>

#include <elf_choice_dialog.h>

// Prints a NULL_terminated array of strings
static
void print_strings(const char **strings){
	int i;
	for(i = 0; strings[i] != NULL; i++)
		printf("{%s}\n", strings[i]);
	printf("\n");
}

// Frees a NULL_terminated array of strings
static
void free_strings(char **strings){
	int i;
	for(i = 0; strings[i] != NULL; i++)
		free(strings[i]);
	free(strings);
}

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

	elfChoiceDialog_changeChoice(elf, 1, "Choice 1");
	elfChoiceDialog_changeChoice(elf, 1, "");
	elfChoiceDialog_changeChoice(elf, 3, "Choiceee 3");
	elfChoiceDialog_printInternal(elf);

	elfChoiceDialog_removeChoice(elf, 3);
	elfChoiceDialog_removeChoice(elf, 1);
	elfChoiceDialog_printInternal(elf);

	elfChoiceDialog_setChoiceZero(elf, "");
	elfChoiceDialog_setChoiceZero(elf, "ZEROOOOO");
	elfChoiceDialog_setChoiceZero(elf, "");
	elfChoiceDialog_setChoiceZero(elf, "ZEROOOOO!!!!!");
	elfChoiceDialog_printInternal(elf);

	elfChoiceDialog_setWidth(elf, 700);
	elfChoiceDialog_setWidth(elf, 100);
	elfChoiceDialog_setWidth(elf, 100);
	elfChoiceDialog_printInternal(elf);

	elfChoiceDialog_destroy(&elf);
}

void test2(){
	char *s;
	char **res;
	int w;

	printf("\n\n===== String Splitting =====\n");

	s = "string for testing the string splitting algorithm";
	w = 70;
	printf("Testing on string: {%s}\n", s);
	res = test_split(s, w);
	print_strings((const char **) res);
	free_strings(res);

	s = "string for testing the string splitting algorithm";
	w = 30;
	printf("Testing on string: {%s}\n", s);
	res = test_split(s, w);
	print_strings((const char **) res);
	free_strings(res);

	s = "string for testing the string splitting algorithm";
	w = 10;
	printf("Testing on string: {%s}\n", s);
	res = test_split(s, w);
	print_strings((const char **) res);
	free_strings(res);

	s = "string for testing the string splitting algorithm";
	w = 5;
	printf("Testing on string: {%s}\n", s);
	res = test_split(s, w);
	print_strings((const char **) res);
	free_strings(res);

	s = "pneumoultramicroscopicossilicovulcanoconiose";
	w = 5;
	printf("Testing on string: {%s}\n", s);
	res = test_split(s, w);
	print_strings((const char **) res);
	free_strings(res);

	s = "";
	w = 5;
	printf("Testing on string: {%s}\n", s);
	res = test_split(s, w);
	print_strings((const char **) res);
	free_strings(res);

	s = "a   b   c   d   e   f  g";
	w = 5;
	printf("Testing on string: {%s}\n", s);
	res = test_split(s, w);
	print_strings((const char **) res);
	free_strings(res);
}

void test3(){
	const char *str;
	ElfChoiceDialog *elf = elfChoiceDialog_new();

	printf("\n\n===== Canvas Building =====\n");

	elfChoiceDialog_setHeader(elf, "Hello World!");
	elfChoiceDialog_setText(elf, "Hello World!!!");
	elfChoiceDialog_addChoice(elf, "Choice Number 1");
	elfChoiceDialog_addChoice(elf, "Choice Number 2");
	elfChoiceDialog_setChoiceZero(elf, "Choice Zero!!");

	str = elfChoiceDialog_getInterface(elf);
	printf("%p\n%s\n", str, str);

	str = elfChoiceDialog_getInterface(elf);
	printf("%p\n%s\n", str, str);

	elfChoiceDialog_destroy(&elf);
}

int main(int argc, char *argv[]){
	test1();
	test2();
	test3();

	return 0;
}
