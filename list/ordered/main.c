#include <stdlib.h>
#include <stdio.h>
#include <elf_list.h>

bool greater(void *d1, void *d2){
	return ELF_POINTER_TO_INT(d1) > ELF_POINTER_TO_INT(d2) ? true : false;
}

bool equal(void *d1, void *d2){
	return ELF_POINTER_TO_INT(d1) == ELF_POINTER_TO_INT(d2) ? true : false;
}

void print(void *d){
	printf("%d ", ELF_POINTER_TO_INT(d));
}

void print_list(ElfList *l){
	elf_list_traverse(l, print);
	printf("\n");
}

int main(int argc, char *argv[]){

	ElfList *list = elf_list_newWithEqual(greater, equal);
	print_list(list);
	
	int i;
	for(i = 0; i < 20; i++)
		elf_list_insert(list, ELF_INT_TO_POINTER(i%7));
	print_list(list);
	printf("size: %d\n", elf_list_size(list));
	for(i = 0; i < 7; i++)
		printf("Count of %d: %d\n", i, elf_list_count(list, ELF_INT_TO_POINTER(i)));

	printf("Removed 2: %d\n", elf_list_removeValue(list, ELF_INT_TO_POINTER(2)));
	printf("size: %d\n", elf_list_size(list));
	print_list(list);

	for(i = 19; i >= 0; i--)
		elf_list_removeIndex(list, i);
	print_list(list);
	printf("size: %d\n", elf_list_size(list));

	for(i = 0; i < 20; i++)
		elf_list_insert(list, ELF_INT_TO_POINTER(i%7));
	print_list(list);
	for(i = 19; i >= 0; i--)
		print(elf_list_get(list, i));
	printf("\n");

	int n;
	for(i = 0, n = elf_list_size(list); i < n; i++)
		elf_list_removeIndex(list, 0);
	printf("Insert unique:\n");
	for(i = 0; i < 21; i++)
		elf_list_insertUnique(list, ELF_INT_TO_POINTER(i%7));
	print_list(list);
	printf("%d\n", elf_list_size(list));

	elf_list_destroy(&list);

	return 0;
}
