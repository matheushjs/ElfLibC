#include <stdlib.h>
#include <stdio.h>
#include <elf_list.h>

void print(int d){
	printf("%d ", d);
}

void print_list(ElfList *l){
	elfList_traverse(l, print);
	printf("\n");
}

void test_iterators(ElfList *l){
	ElfListIt *it;

	printf("Using iterator: ");
	it = elfList_getIterator(l);
	while(it != NULL){
		printf("%d ", it->key);
		it = it->next;
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	ElfList *list = elfList_new();
	print_list(list);
	
	int i;
	for(i = 0; i < 20; i++)
		elfList_insert(list, i%7);
	print_list(list);
	printf("size: %d\n", elfList_size(list));
	printf("Count of 0: %d\n", elfList_count(list, 0));

	for(i = 19; i >= 0; i--)
		elfList_remove(list, i);
	print_list(list);

	for(i = 0; i < 20; i++)
		elfList_insert(list, i%7);
	print_list(list);
	for(i = 19; i >= 0; i--)
		print(elfList_get(list, i));
	printf("\n");
	test_iterators(list);

	for(i = 19; i >= 0; i--)
		elfList_remove(list, i);
	for(i = 0; i < 20; i++)
		elfList_insertUnique(list, i%7);
	printf("Unique:\n");
	print_list(list);
	printf("size: %d\n", elfList_size(list));

	for(i = 0; i < 10; i++){
		if(elfList_contains(list, i))
			printf("Found: %d\n", i);
		else printf("Didn't find: %d\n", i);
		printf("Count of %d: %d\n", i, elfList_count(list, i));
	}

	elfList_destroy(&list);

	return 0;
}
