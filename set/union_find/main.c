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
#include <elf_int_uf_set.h>

void printSet(ElfIntUFSet *set){
	int i;
	int size = elfIntUFSet_size(set);

	for(i = 0; i < size; i++){
		printf("%3d ", i);
	}
	printf("\n");
	for(i = 0; i < size; i++){
		printf("%3d ", elfIntUFSet_find(set, i));
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	ElfIntUFSet *set;
	int a, b, size;

	printf("How many elements?\n");
	scanf("%d", &size);

	set = elfIntUFSet_new(size);

	printf("Just keep typing 2 numbers to perform union operations. Type a letter to end.\n");
	while( scanf("%d %d", &a, &b) == 2 ){
		if( ((unsigned char) a) > 19 || ((unsigned char) b) > 19) //Fancy array bounds-checking
			continue;
		elfIntUFSet_union(set, a, b);
		printSet(set);
	}

	elfIntUFSet_destroy(&set);
}
