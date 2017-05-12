#include <stdio.h>
#include <elf_set.h>

void printSet(ElfSet *set, int size){
	int i;
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
	int a, b;

	set = elfSet_new(20);

	while( scanf("%d %d", &a, &b) == 2 ){
		if( ((unsigned char) a) > 19 || ((unsigned char) b) > 19) continue;
		elfSet_union(set, a, b);
		printSet(set, 20);
	}

	elfSet_destroy(&set);
}
