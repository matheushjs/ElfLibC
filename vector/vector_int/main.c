#include <stdio.h>

#include <elf_vector.h>

void test(ElfVector *vec){
	int i;

	for(i = 0; i < 100; i++)
		elfVector_pushBack(vec, i);

	elfVector_print(vec);
	printf("\n");
}

int main(int argc, char *argv[]){
	ElfVector *vec;

	vec = elfVector_new_withValue(10, 1);
	test(vec);
	elfVector_destroy(&vec);

	vec = elfVector_new();
	test(vec);
	elfVector_destroy(&vec);

	return 0;
}
