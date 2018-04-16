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
#include <string.h>

#include <elf_int_vector.h>

void test(ElfIntVector *vec){
	int i, max, min;

	for(i = 0; i < 10; i++)
		elfIntVector_pushBack(vec, i);
	printf("Vector has %d elements.\n", elfIntVector_size(vec));
	elfIntVector_put(vec, 0, 727272);
	elfIntVector_put(vec, 727272, 727272);
	printf("First element is %d.\n", elfIntVector_get(vec, 0));
	printf("Last element is %d.\n", elfIntVector_get(vec, elfIntVector_size(vec)-1));
	elfIntVector_maxmin(vec, &max, &min);
	printf("maxmin are %d, %d\n", max, min);
	printf("Index of 1: %d\n", elfIntVector_search(vec, 1));
	printf("Count of 1: %d\n", elfIntVector_count(vec, 1));

	elfIntVector_print(vec);
	printf("\n");

	ElfIntVector *indexes = elfIntVector_qsort_ascendWithIndexes(vec);
	printf("Sorted: ");
	elfIntVector_print(vec);
	printf("\n");
	printf("Indexes: ");
	elfIntVector_print(indexes);
	printf("\n");
	elfIntVector_destroy(&indexes);
	printf("Index of 1: %d\n", elfIntVector_search_sorted(vec, 1));
	printf("Count of 1: %d\n", elfIntVector_count_sorted(vec, 1));
	printf("Index of 172172172: %d\n", elfIntVector_search_sorted(vec, 172172172));
	printf("Count of 172172172: %d\n", elfIntVector_count_sorted(vec, 172172172));

	printf("Sorted: ");
	elfIntVector_qsort_descend(vec);
	elfIntVector_print(vec);
	printf("\n");
	printf("Index of 1: %d\n", elfIntVector_search_sorted(vec, 1));
	printf("Count of 1: %d\n", elfIntVector_count_sorted(vec, 1));
	printf("Index of 172172172: %d\n", elfIntVector_search_sorted(vec, 172172172));
	printf("Count of 172172172: %d\n", elfIntVector_count_sorted(vec, 172172172));

	elfIntVector_insert(vec, 0, 91);
	elfIntVector_insert(vec, elfIntVector_size(vec)-1, 92);
	elfIntVector_insert(vec, 2, 93);
	elfIntVector_insert(vec, 101, 94);
	elfIntVector_print(vec);
	printf("\n");
	printf("Removed: %d %d %d %d\n",
			elfIntVector_remove(vec, 0),
			elfIntVector_remove(vec, elfIntVector_size(vec)-2),
			elfIntVector_remove(vec, 2),
			elfIntVector_remove(vec, 101));
	printf("Removing last element: %d\n", elfIntVector_remove(vec, elfIntVector_size(vec)-1));

	printf("Popping: ");
	while(elfIntVector_size(vec) != 0)
		printf("%d ", elfIntVector_popBack(vec));
	printf("\n");

	printf("Vector has %d elements.\n", elfIntVector_size(vec));
	printf("\n\n");
}

int main(int argc, char *argv[]){
	ElfIntVector *vec, *vec2;

	vec = elfIntVector_new_withValue(10, 1);
	vec2 = elfIntVector_new_fromOther(vec);
	test(vec2);
	elfIntVector_destroy(&vec);
	elfIntVector_destroy(&vec2);

	vec = elfIntVector_new();
	test(vec);
	elfIntVector_destroy(&vec);

	vec = elfIntVector_new_random(10, -10, 10);
	test(vec);
	elfIntVector_destroy(&vec);

	int array[] = {10, 1, 9, 2, 8, 3, 7, 4, 6, 5};
	int *mem = NULL;
	
	mem = malloc(sizeof(array));
	memcpy(mem, array, sizeof(array));

	vec = elfIntVector_new_fromArray(&mem, 10);
	test(vec);
	elfIntVector_destroy(&vec);

	return 0;
}
