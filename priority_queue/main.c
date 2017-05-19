#include <stdio.h>
#include <stdlib.h>

#include <elf_pqueue.h>

int main(int argc, char *argv[]){
	ElfPQueue *pq = elfPQueue_new_minFirst();
	
	printf("Keep typing (value, priority). Terminate by typing a letter.\n");

	int size, value, pr;

	while( scanf("%d %d", &value, &pr) == 2){
		elfPQueue_push(pq, value, pr);
	}

	printf("\n");

	while( (size = elfPQueue_size(pq)) != 0 ){
		value = elfPQueue_pop(pq, &pr);
		printf("(%d) Popped: %d %d\n", size, value, pr);
	}

	elfPQueue_destroy(&pq);

	return 0;
}
