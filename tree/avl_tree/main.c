#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avltree.h>

bool higher(void *a, void *b){
	return (*(int *)a > *(int *)b) ? true : false;
}

bool equal(void *a, void *b){
	return (*(int *)a == *(int *)b) ? true : false;
}

void print(void *a){
	printf("%d", *(int *)a);
}

void data_free(void *a){
	free(a);
}

int main(int argc, char *argv[]){
	int i, lim;
	int add[] = {10, 7, 20, 15, 17, 25, 30, 5, 1};
	int search[] = {20, 15, 101};
	int *p;
	tree_t *tree;

	lim = sizeof(add)/sizeof(int);
	tree = tree_alloc(higher, equal, data_free, print);
	for(i = 0; i < lim; i++){
		p = (int *) malloc(sizeof(int));
		*p = add[i];
		printf("Attempting to insert: %d\n", *p);
		tree_insert(tree, p);
		tree_print(tree);
		printf("\n");
	}

	lim = sizeof(search)/sizeof(int);
	for(i = 0; i < lim; i++){
		p = (int *) malloc(sizeof(int));
		*p = search[i];
		printf("%s: %d\n", tree_search(tree, p) ? "Found" : "Did not find", *p);
		free(p);
	}

	tree_destroy(&tree);
	return 0;
}
