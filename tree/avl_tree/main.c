#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avltree.h>

bool higher(int *a, int *b){
	return *a > *b ? true : false;
}

bool equal(int *a, int *b){
	return *a == *b ? true : false;
}

void print(int *a){
	printf("%d", *a);
}

int main(int argc, char *argv[]){
	int i, lim, add[] = {10, 7, 20, 15, 17, 25, 30, 5, 1};
	tree_t *tree;

	tree_set_higher(higher);
	tree_set_equal(equal);
	tree_set_print(print);

	lim = sizeof(add)/sizeof(int);
	tree = tree_alloc();
	for(i = 0; i < lim; i++){
		printf("Attempting to insert: %d\n", add[i]);
		tree_insert(tree, &add[i]);
		tree_print(tree);
		printf("\n");
	}

	return 0;
}
