#include <stdlib.h>
#include <stdio.h>
#include <elf_queue.h>

void print(void *i){
	printf("%d ", ELF_POINTER_TO_INT(i));
}

int main(int argc, char *argv[]){
	ElfQueue *q;

	q = elfQueue_new();

	printf("[0|%d]\n", elfQueue_size(q));
	printf("[0|%lu]\n", (unsigned long) elfQueue_front(q));
	printf("[0|%lu]\n", (unsigned long) elfQueue_pop(q));
	
	int i;
	for(i = 0; i < 100; i++)
		elfQueue_push(q, ELF_INT_TO_POINTER(i));

	printf("[100|%d]\n", elfQueue_size(q));
	printf("[99|%d]\n", ELF_POINTER_TO_INT(elfQueue_back(q)));
	printf("[0|%d]\n", ELF_POINTER_TO_INT(elfQueue_pop(q)));
	printf("[1|%d]\n", ELF_POINTER_TO_INT(elfQueue_pop(q)));
	printf("[98|%d]\n", elfQueue_size(q));

	elfQueue_invert(q);
	printf("[99|%d]\n", ELF_POINTER_TO_INT(elfQueue_pop(q)));
	printf("[98|%d]\n", ELF_POINTER_TO_INT(elfQueue_pop(q)));
	printf("[96|%d]\n", elfQueue_size(q));

	for(i = 0; i < 100; i++)
		elfQueue_pop(q);

	printf("[0|%d]\n", elfQueue_size(q));
	
	for(i = 0; i < 10; i++)
		elfQueue_push(q, ELF_INT_TO_POINTER(i));
	printf("[0 1 2 3 4 5 6 7 8 9 |");
	elfQueue_traverse(q, print);
	printf("]\n");
	elfQueue_destroy(&q);

	return 0;
}
