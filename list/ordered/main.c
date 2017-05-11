#include <stdlib.h>
#include <stdio.h>

#include <elf_list.h>

bool equal(void *d1, void *d2){
	return ELF_POINTER_TO_INT(d1) == ELF_POINTER_TO_INT(d2) ? true : false;
}

void print(void *d){
	printf("%d ", ELF_POINTER_TO_INT(d));
}

void print_list(ElfList *l){
	elfList_traverse(l, print);
	printf("\n");
}

int main(int argc, char *argv[]){

	ElfList *list = elfList_newWithEqual(ELF_POINTER_TO_INT_GREATER, equal);
	print_list(list);
	
	int i;
	for(i = 0; i < 20; i++)
		elfList_insert(list, ELF_INT_TO_POINTER(i%7));
	print_list(list);
	printf("size: %d\n", elfList_size(list));
	for(i = 0; i < 7; i++)
		printf("Count of %d: %d\n", i, elfList_count(list, ELF_INT_TO_POINTER(i)));

	printf("Removed 2: %d\n", elfList_removeValue(list, ELF_INT_TO_POINTER(2)));
	printf("size: %d\n", elfList_size(list));
	print_list(list);

	for(i = 19; i >= 0; i--)
		elfList_removeIndex(list, i);
	print_list(list);
	printf("size: %d\n", elfList_size(list));

	for(i = 0; i < 20; i++)
		elfList_insert(list, ELF_INT_TO_POINTER(i%7));
	print_list(list);
	for(i = 19; i >= 0; i--)
		print(elfList_get(list, i));
	printf("\n");

	int n;
	for(i = 0, n = elfList_size(list); i < n; i++)
		elfList_removeIndex(list, 0);
	printf("Insert unique:\n");
	for(i = 0; i < 21; i++)
		elfList_insertUnique(list, ELF_INT_TO_POINTER(i%7));
	print_list(list);
	printf("%d\n", elfList_size(list));

	elfList_destroy(&list);

	return 0;
}
