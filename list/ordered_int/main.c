#include <stdlib.h>
#include <stdio.h>
#include <elf_int_list.h>

void print(int d){
	printf("%d ", d);
}

void print_list(ElfIntList *l){
	elfIntList_traverse(l, print);
	printf("\n");
}

void test_iterators(ElfIntList *l){
	ElfIntListIt *it;

	printf("Using iterator: ");
	it = elfIntList_getIterator(l);
	while(it != NULL){
		printf("%d ", it->key);
		it = it->next;
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	ElfIntList *list = elfIntList_new();
	print_list(list);
	
	int i;
	for(i = 0; i < 20; i++)
		elfIntList_insert(list, i%7);
	print_list(list);
	printf("size: %d\n", elfIntList_size(list));
	printf("Count of 0: %d\n", elfIntList_count(list, 0));

	for(i = 19; i >= 0; i--)
		elfIntList_remove(list, i);
	print_list(list);

	for(i = 0; i < 20; i++)
		elfIntList_insert(list, i%7);
	print_list(list);
	for(i = 19; i >= 0; i--)
		print(elfIntList_get(list, i));
	printf("\n");
	test_iterators(list);

	for(i = 19; i >= 0; i--)
		elfIntList_remove(list, i);
	for(i = 0; i < 20; i++)
		elfIntList_insertUnique(list, i%7);
	printf("Unique:\n");
	print_list(list);
	printf("size: %d\n", elfIntList_size(list));

	for(i = 0; i < 10; i++){
		if(elfIntList_contains(list, i))
			printf("Found: %d\n", i);
		else printf("Didn't find: %d\n", i);
		printf("Count of %d: %d\n", i, elfIntList_count(list, i));
	}

	elfIntList_destroy(&list);

	return 0;
}
