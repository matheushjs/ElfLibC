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

#include <elf_string.h>
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

void addStuff(ElfChoiceDialog *elf){
	elfChoiceDialog_setHeader(elf, "This is my dialog's cute big header that I want properly split in multiple lines");
	elfChoiceDialog_setText(elf, "This is my dialog's cute big text that I also want properly split in multiple lines");
	elfChoiceDialog_setChoiceZero(elf, "This is my dialog's cute choice zero, which, duh, I also want it properly split in multiple lines");

	int i;
	for(i = 0; i < 10; i++){
		char *str = elfString_format("This is my dialog's %d-th choice, which is big because I want to check if it is properly split in multiple lines", i+1);
		elfChoiceDialog_addChoice(elf, str);
		free(str);
	}
}

void test4(){
	ElfChoiceDialog *elf = elfChoiceDialog_new();

	addStuff(elf);

	printf("%s\n", elfChoiceDialog_getInterface(elf));
	
	elfChoiceDialog_setWidth(elf, 20);
	printf("%s\n", elfChoiceDialog_getInterface(elf));

	elfChoiceDialog_setWidth(elf, 30);
	printf("%s\n", elfChoiceDialog_getInterface(elf));

	elfChoiceDialog_setWidth(elf, 100);
	printf("%s\n", elfChoiceDialog_getInterface(elf));

	elfChoiceDialog_destroy(&elf);
}

int main(int argc, char *argv[]){
	test1();
	test2();
	test3();
	test4();

	return 0;
}
