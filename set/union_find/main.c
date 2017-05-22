#include <stdio.h>
#include <elf_int_uf_set.h>

void printSet(ElfIntUFSet *set){
	int i;
	int size = elfIntUFSet_size(set);

	for(i = 0; i < size; i++){
		printf("%3d ", i);
	}
	printf("\n");
	for(i = 0; i < size; i++){
		printf("%3d ", elfIntUFSet_find(set, i));
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	ElfIntUFSet *set;
	int a, b, size;

	printf("How many elements?\n");
	scanf("%d", &size);

	set = elfIntUFSet_new(size);

	printf("Just keep typing 2 numbers to perform union operations. Type a letter to end.\n");
	while( scanf("%d %d", &a, &b) == 2 ){
		if( ((unsigned char) a) > 19 || ((unsigned char) b) > 19) //Fancy array bounds-checking
			continue;
		elfIntUFSet_union(set, a, b);
		printSet(set);
	}

	elfIntUFSet_destroy(&set);
}
