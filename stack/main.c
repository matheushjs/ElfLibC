#include <stdio.h>
#include <elf_stack.h>

void print(void *i){
	printf("%d ", ELF_POINTER_TO_INT(i));
}

int main(int argc, char *argv[]){
	ElfStack *s = elf_stack_new();

	int i;
	for(i = 0; i < 100000; i++)
		elf_stack_push(s, ELF_INT_TO_POINTER(i));

	printf("[99999|%d]\n", ELF_POINTER_TO_INT(elf_stack_pop(s)));
	printf("[99998|%d]\n", ELF_POINTER_TO_INT(elf_stack_top(s)));
	printf("[99998|%d]\n", ELF_POINTER_TO_INT(elf_stack_pop(s)));
	printf("[99998|%d]\n", elf_stack_size(s));

	for(i = 0; i < 100000; i++)
		elf_stack_pop(s);

	printf("[0|%d]\n", elf_stack_size(s));
	printf("[0|%d]\n", ELF_POINTER_TO_INT(elf_stack_pop(s)));
	printf("[0|%d]\n", ELF_POINTER_TO_INT(elf_stack_top(s)));
	printf("[0|%d]\n", ELF_POINTER_TO_INT(elf_stack_pop(s)));

	for(i = 0; i < 5; i++)
		elf_stack_push(s, ELF_INT_TO_POINTER(i));

	printf("[4 3 2 1 0 |");
	elf_stack_traverse(s, print);
	printf("]\n");

	printf("[0 1 2 3 4 |");
	elf_stack_traverse_inv(s, print);
	printf("]\n");

	elf_stack_destroy(&s);

	return 0;
}
