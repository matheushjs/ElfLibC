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
