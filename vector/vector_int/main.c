#include <stdio.h>

#include <elf_vector.h>

void test(ElfVector *vec){
	int i, max, min;

	for(i = 0; i < 10; i++)
		elfVector_pushBack(vec, i);
	printf("Vector has %d elements.\n", elfVector_size(vec));
	elfVector_put(vec, 0, 727272);
	elfVector_put(vec, 727272, 727272);
	printf("First element is %d.\n", elfVector_get(vec, 0));
	printf("Last element is %d.\n", elfVector_get(vec, elfVector_size(vec)-1));
	elfVector_maxmin(vec, &max, &min);
	printf("maxmin are %d, %d\n", max, min);
	printf("Index of 1: %d\n", elfVector_search(vec, 1));
	printf("Count of 1: %d\n", elfVector_count(vec, 1));

	elfVector_print(vec);
	printf("\n");

	printf("Sorted: ");
	elfVector_qsort_ascend(vec);
	elfVector_print(vec);
	printf("\n");
	printf("Index of 1: %d\n", elfVector_search_sorted(vec, 1));
	printf("Index of 172172172: %d\n", elfVector_search_sorted(vec, 172172172));

	printf("Sorted: ");
	elfVector_qsort_descend(vec);
	elfVector_print(vec);
	printf("\n");
	printf("Index of 1: %d\n", elfVector_search_sorted(vec, 1));
	printf("Index of 172172172: %d\n", elfVector_search_sorted(vec, 172172172));

	while(elfVector_size(vec) != 0)
		printf("%d ", elfVector_popBack(vec));
	printf("\n");

	printf("Vector has %d elements.\n", elfVector_size(vec));
	printf("\n\n");
}

int main(int argc, char *argv[]){
	ElfVector *vec;

	vec = elfVector_new_withValue(10, 1);
	test(vec);
	elfVector_destroy(&vec);

	vec = elfVector_new();
	test(vec);
	elfVector_destroy(&vec);

	vec = elfVector_new_random(10, -10, 10);
	test(vec);
	elfVector_destroy(&vec);

	return 0;
}
