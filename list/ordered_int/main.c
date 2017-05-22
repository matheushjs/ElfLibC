#include <stdlib.h>
#include <stdio.h>
#include <elf_list_int.h>

void print(int d){
	printf("%d ", d);
}

void print_list(ElfListInt *l){
	elfListInt_traverse(l, print);
	printf("\n");
}

void test_iterators(ElfListInt *l){
	ElfListIntIt *it;

	printf("Using iterator: ");
	it = elfListInt_getIterator(l);
	while(it != NULL){
		printf("%d ", it->key);
		it = it->next;
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	ElfListInt *list = elfListInt_new();
	print_list(list);
	
	int i;
	for(i = 0; i < 20; i++)
		elfListInt_insert(list, i%7);
	print_list(list);
	printf("size: %d\n", elfListInt_size(list));
	printf("Count of 0: %d\n", elfListInt_count(list, 0));

	for(i = 19; i >= 0; i--)
		elfListInt_remove(list, i);
	print_list(list);

	for(i = 0; i < 20; i++)
		elfListInt_insert(list, i%7);
	print_list(list);
	for(i = 19; i >= 0; i--)
		print(elfListInt_get(list, i));
	printf("\n");
	test_iterators(list);

	for(i = 19; i >= 0; i--)
		elfListInt_remove(list, i);
	for(i = 0; i < 20; i++)
		elfListInt_insertUnique(list, i%7);
	printf("Unique:\n");
	print_list(list);
	printf("size: %d\n", elfListInt_size(list));

	for(i = 0; i < 10; i++){
		if(elfListInt_contains(list, i))
			printf("Found: %d\n", i);
		else printf("Didn't find: %d\n", i);
		printf("Count of %d: %d\n", i, elfListInt_count(list, i));
	}

	elfListInt_destroy(&list);

	return 0;
}
