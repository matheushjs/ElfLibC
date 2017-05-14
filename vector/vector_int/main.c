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
	printf("Count of 1: %d\n", elfVector_count_sorted(vec, 1));
	printf("Index of 172172172: %d\n", elfVector_search_sorted(vec, 172172172));
	printf("Count of 172172172: %d\n", elfVector_count_sorted(vec, 172172172));

	printf("Sorted: ");
	elfVector_qsort_descend(vec);
	elfVector_print(vec);
	printf("\n");
	printf("Index of 1: %d\n", elfVector_search_sorted(vec, 1));
	printf("Count of 1: %d\n", elfVector_count_sorted(vec, 1));
	printf("Index of 172172172: %d\n", elfVector_search_sorted(vec, 172172172));
	printf("Count of 172172172: %d\n", elfVector_count_sorted(vec, 172172172));

	elfVector_insert(vec, 0, 91);
	elfVector_insert(vec, elfVector_size(vec)-1, 92);
	elfVector_insert(vec, 2, 93);
	elfVector_insert(vec, 101, 94);
	elfVector_print(vec);
	printf("\n");
	printf("Removed: %d %d %d %d\n",
			elfVector_remove(vec, 0),
			elfVector_remove(vec, elfVector_size(vec)-2),
			elfVector_remove(vec, 2),
			elfVector_remove(vec, 101));
	printf("Removing last element: %d\n", elfVector_remove(vec, elfVector_size(vec)-1));

	printf("Popping: ");
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
