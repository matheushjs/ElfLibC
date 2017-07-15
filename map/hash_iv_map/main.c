#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <elf_iv_hash_map.h>

void print(int key, void *value){
	printf("(%d,%d) ", key, ELF_POINTER_TO_INT(value));
}

void test(ElfIVHashMap *map){
	int i;
	void *value;

	elfIVHashMap_traverse(map, print);
	printf("\n");
	
	int num[100];
	for(i = 0; i < 100; i++){
		num[i] = rand();
		elfIVHashMap_put(map, i%10, ELF_INT_TO_POINTER(num[i]));
	}

	printf("Numbers that should have been added: ");
	for(i = 90; i < 100; i++)
		printf("%d ", num[i]);
	printf("\n");

	elfIVHashMap_traverse(map, print);
	printf("\n");

	printf("Size: %d\n", elfIVHashMap_size(map));
	printf("Capacity: %d\n", elfIVHashMap_capacity(map));
	printf("Load: %lf\n", elfIVHashMap_loadFactor(map));
	elfIVHashMap_printBucketsStatus(map);

	for(i = 0; i < 25; i += 3){
		value = elfIVHashMap_get(map, i);
		if(value){
			printf("Key %d has value %d\n", i, ELF_POINTER_TO_INT(value));
		} else {
			printf("Key %d doesn't exist.\n", i);
		}
	}

	for(i = 0; i < 5; i++)
		elfIVHashMap_remove(map, i);

	printf("Size: %d\n", elfIVHashMap_size(map));
	printf("Capacity: %d\n", elfIVHashMap_capacity(map));
	printf("Load: %lf\n", elfIVHashMap_loadFactor(map));
	elfIVHashMap_printBucketsStatus(map);

	for(i = 0; i < 10; i++){
		value = elfIVHashMap_get(map, i);
		if(value){
			printf("Key %d has value %d\n", i, ELF_POINTER_TO_INT(value));
		} else {
			printf("Key %d doesn't exist.\n", i);
		}
	}

	for(i = 0; i < 100000; i++){
		elfIVHashMap_remove(map, i);
	}

	printf("Size: %d\n", elfIVHashMap_size(map));
	printf("Capacity: %d\n", elfIVHashMap_capacity(map));
	printf("Load: %lf\n", elfIVHashMap_loadFactor(map));
	elfIVHashMap_printBucketsStatus(map);
}

int main(int argc, char *argv[]){
	ElfIVHashMap *map;
	srand(time(NULL));

	printf("=======\n\nTEST 1\n\n=======\n");
	map = elfIVHashMap_new();
	test(map);
	elfIVHashMap_destroy(&map);
	printf("\n============================\n");

	printf("=======\n\nTEST 2\n\n=======\n");
	map = elfIVHashMap_new_withSize(1000);
	test(map);
	elfIVHashMap_destroy(&map);
	printf("\n============================\n");
	return 0;
}
