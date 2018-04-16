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
#include <elf_stack.h>

void print(void *i){
	printf("%d ", ELF_POINTER_TO_INT(i));
}

int main(int argc, char *argv[]){
	ElfStack *s = elfStack_new();

	int i;
	for(i = 0; i < 100000; i++)
		elfStack_push(s, ELF_INT_TO_POINTER(i));

	printf("[99999|%d]\n", ELF_POINTER_TO_INT(elfStack_pop(s)));
	printf("[99998|%d]\n", ELF_POINTER_TO_INT(elfStack_top(s)));
	printf("[99998|%d]\n", ELF_POINTER_TO_INT(elfStack_pop(s)));
	printf("[99998|%d]\n", elfStack_size(s));

	for(i = 0; i < 100000; i++)
		elfStack_pop(s);

	printf("[0|%d]\n", elfStack_size(s));
	printf("[0|%d]\n", ELF_POINTER_TO_INT(elfStack_pop(s)));
	printf("[0|%d]\n", ELF_POINTER_TO_INT(elfStack_top(s)));
	printf("[0|%d]\n", ELF_POINTER_TO_INT(elfStack_pop(s)));

	for(i = 0; i < 5; i++)
		elfStack_push(s, ELF_INT_TO_POINTER(i));

	printf("[4 3 2 1 0 |");
	elfStack_traverse(s, print);
	printf("]\n");

	printf("[0 1 2 3 4 |");
	elfStack_traverse_inv(s, print);
	printf("]\n");

	elfStack_destroy(&s);

	return 0;
}
