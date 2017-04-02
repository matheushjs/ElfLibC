#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avltree.h>
#include <time.h>

bool higher(const void *a, const void *b){
	return (*(int *)a > *(int *)b) ? true : false;
}

bool equal(const void *a, const void *b){
	return (*(int *)a == *(int *)b) ? true : false;
}

void print(const void *a){
	printf("%d", *(int *)a);
}

void data_free(void *a){
	free(a);
}

int random_int(int mod){
	static unsigned int i;
	srand(time(NULL)*i);
	i++;
	return rand()%mod;
}

int main(int argc, char *argv[]){
	int i;
	int add = 100;
	int search = 0;
	int remove = 75;
	int mod = 1000;
	int *p;
	tree_t *tree;

	tree = tree_alloc(higher, equal, data_free, print);
	
	for(i = 0; i < add; i++){
		p = (int *) malloc(sizeof(int));
		*p = random_int(mod);
		//tree_print(tree);
		//printf("\nAttempting to insert: %d\n", *p);
		tree_insert(tree, p);
	}

	for(i = 0; i < search; i++){
		p = (int *) malloc(sizeof(int));
		*p = random_int(mod);
		tree_search(tree, p);
		//printf("%s: %d\n", tree_search(tree, p) ? "Found" : "Did not find", *p);
		free(p);
	}

	for(i = 0; i < remove; i++){
		p = (int *) malloc(sizeof(int));
		*p = random_int(mod);
		//tree_print(tree);
		//printf("\nAttempting to remove: %d\n", *p);
		tree_remove(tree, p);
		free(p);
	}

	tree_print_inorder(tree);
	printf("\n");

	p = tree_highest(tree);
	printf("%d\n", *p);
	
	tree_print_inorder(tree);
	printf("\n");
	
	int num;
	printf("Successors.\n");
	do{
		scanf("%d", &num);
		p = tree_successor(tree, &num);
		if(!p) continue;
		printf("- %d\n", *p);
	} while(num != -1);
	
	printf("Predecessors.\n");
	do{
		scanf("%d", &num);
		p = tree_predecessor(tree, &num);
		if(!p) continue;
		printf("- %d\n", *p);
	} while(num != -1);

	tree_destroy(&tree);
	return 0;
}
