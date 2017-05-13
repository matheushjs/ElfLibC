#include <stdio.h>

#include <elf_vector.h>

int main(int argc, char *argv[]){
	ElfVector *vec;

	vec = elfVector_new_withValue(10, 1);
	elfVector_print(vec);
	elfVector_destroy(&vec);

	return 0;
}
