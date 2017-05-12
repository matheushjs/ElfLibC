#include <stdio.h>
#include <elf_set.h>

void printSet(ElfSet *set){
	int i;
	int size = elfSet_size(set);

	for(i = 0; i < size; i++){
		printf("%3d ", i);
	}
	printf("\n");
	for(i = 0; i < size; i++){
		printf("%3d ", elfSet_find(set, i));
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	ElfSet *set;
	int a, b, size;

	printf("How many elements?\n");
	scanf("%d", &size);

	set = elfSet_new(size);

	printf("Just keep typing 2 numbers to perform union operations. Type a letter to end.\n");
	while( scanf("%d %d", &a, &b) == 2 ){
		if( ((unsigned char) a) > 19 || ((unsigned char) b) > 19) //Fancy array bounds-checking
			continue;
		elfSet_union(set, a, b);
		printSet(set);
	}

	elfSet_destroy(&set);
}
