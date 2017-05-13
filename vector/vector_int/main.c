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

	elfVector_print(vec);
	printf("\n");

	while(elfVector_size(vec) != 0)
		printf("%d ", elfVector_popBack(vec));
	printf("\n");

	printf("Vector has %d elements.\n", elfVector_size(vec));
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
