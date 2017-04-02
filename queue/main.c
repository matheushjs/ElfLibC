#include <stdlib.h>
#include <stdio.h>
#include <elf_queue.h>

void print(void *i){
	printf("%d ", ELF_POINTER_TO_INT(i));
}

int main(int argc, char *argv[]){
	ElfQueue *q;

	q = elf_queue_alloc();

	printf("[0|%d]\n", elf_queue_size(q));
	printf("[0|%lu]\n", (unsigned long) elf_queue_front(q));
	printf("[0|%lu]\n", (unsigned long) elf_queue_pop(q));
	
	int i;
	for(i = 0; i < 100; i++)
		elf_queue_push(q, ELF_INT_TO_POINTER(i));

	printf("[100|%d]\n", elf_queue_size(q));
	printf("[99|%d]\n", ELF_POINTER_TO_INT(elf_queue_back(q)));
	printf("[0|%d]\n", ELF_POINTER_TO_INT(elf_queue_pop(q)));
	printf("[1|%d]\n", ELF_POINTER_TO_INT(elf_queue_pop(q)));
	printf("[98|%d]\n", elf_queue_size(q));

	elf_queue_invert(q);
	printf("[99|%d]\n", ELF_POINTER_TO_INT(elf_queue_pop(q)));
	printf("[98|%d]\n", ELF_POINTER_TO_INT(elf_queue_pop(q)));
	printf("[96|%d]\n", elf_queue_size(q));

	for(i = 0; i < 100; i++)
		elf_queue_pop(q);

	printf("[0|%d]\n", elf_queue_size(q));
	
	for(i = 0; i < 10; i++)
		elf_queue_push(q, ELF_INT_TO_POINTER(i));
	printf("[0 1 2 3 4 5 6 7 8 9 |");
	elf_queue_traverse(q, print);
	printf("]\n");
	elf_queue_destroy(&q);

	return 0;
}
