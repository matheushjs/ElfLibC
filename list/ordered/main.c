#include <stdlib.h>
#include <stdio.h>
#include <elf_list.h>

bool lower(void *d1, void *d2){
	return ELF_POINTER_TO_INT(d1) < ELF_POINTER_TO_INT(d2) ? true : false;
}

void print(void *d){
	printf("%d ", ELF_POINTER_TO_INT(d));
}

void print_list(ElfList *l){
	elf_list_traverse(l, print);
	printf("\n");
}

int main(int argc, char *argv[]){

	ElfList *list = elf_list_new(lower);
	print_list(list);
	
	int i;
	for(i = 0; i < 20; i++)
		elf_list_insert(list, ELF_INT_TO_POINTER(i%7));
	print_list(list);
	printf("size: %d\n", elf_list_size(list));

	for(i = 19; i >= 0; i--)
		elf_list_remove(list, i);
	print_list(list);

	for(i = 0; i < 20; i++)
		elf_list_insert(list, ELF_INT_TO_POINTER(i%7));
	print_list(list);
	for(i = 19; i >= 0; i--)
		print(elf_list_get(list, i));
	printf("\n");
	

	elf_list_destroy(&list);

	return 0;
}
