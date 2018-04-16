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
#include <stdlib.h>
#include <stdio.h>
#include <elf_queue.h>

void print(void *i){
	printf("%d ", ELF_POINTER_TO_INT(i));
}

int main(int argc, char *argv[]){
	ElfQueue *q;

	q = elfQueue_new();

	printf("[0|%d]\n", elfQueue_size(q));
	printf("[0|%lu]\n", (unsigned long) elfQueue_front(q));
	printf("[0|%lu]\n", (unsigned long) elfQueue_pop(q));
	
	int i;
	for(i = 0; i < 100; i++)
		elfQueue_push(q, ELF_INT_TO_POINTER(i));

	printf("[100|%d]\n", elfQueue_size(q));
	printf("[99|%d]\n", ELF_POINTER_TO_INT(elfQueue_back(q)));
	printf("[0|%d]\n", ELF_POINTER_TO_INT(elfQueue_pop(q)));
	printf("[1|%d]\n", ELF_POINTER_TO_INT(elfQueue_pop(q)));
	printf("[98|%d]\n", elfQueue_size(q));

	elfQueue_invert(q);
	printf("[99|%d]\n", ELF_POINTER_TO_INT(elfQueue_pop(q)));
	printf("[98|%d]\n", ELF_POINTER_TO_INT(elfQueue_pop(q)));
	printf("[96|%d]\n", elfQueue_size(q));

	for(i = 0; i < 100; i++)
		elfQueue_pop(q);

	printf("[0|%d]\n", elfQueue_size(q));
	
	for(i = 0; i < 10; i++)
		elfQueue_push(q, ELF_INT_TO_POINTER(i));
	printf("[0 1 2 3 4 5 6 7 8 9 |");
	elfQueue_traverse(q, print);
	printf("]\n");
	elfQueue_destroy(&q);

	return 0;
}
