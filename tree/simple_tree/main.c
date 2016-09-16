#include "tree.h"
#include <stdio.h>

int main(int argc, char *argv[]){
	void print(data_t *data){ printf("%d\n", *data); }
	void print2(data_t *data){ printf("%d", *data); }
	data_t data;

	tree_t *tree = tree_create();
	tree_print_curr(tree, print);

	printf("Adding 0 to the right (should just update root)\n");
	tree_append_data(tree, NULL, TRUE);
	tree_print_curr(tree, print);

	printf("Attempting to descend right\n");
	if(!tree_descend(tree, TRUE)) printf("Could not descend\n");
	else printf("Descended\n");

	printf("Adding 2 to the right\n");
	data = 2;
	tree_append_data(tree, &data, TRUE);
	
	printf("Attempting to descend right\n");
	if(!tree_descend(tree, TRUE)) printf("Could not descend\n");
	else printf("Descended\n");
	
	tree_print_curr(tree, print);
	data = 3;
	printf("Attempting to change data\n");
	tree_change_data(tree, &data);
	tree_print_curr(tree, print);

	printf("Adding 1 to the right\n");
	data = 4;
	tree_append_data(tree, &data, TRUE);
	
	printf("Attempting to descend right\n");
	if(!tree_descend(tree, TRUE)) printf("Could not descend\n");
	else printf("Descended\n");
	tree_print_curr(tree, print);

	printf("Rewinding and descending to the left\n");
	tree_rewind(tree);
	tree_descend(tree, FALSE);
	tree_print_curr(tree, print);

	printf("Adding 5 to the left\n");
	data = 5;
	tree_append_data(tree, &data, FALSE);
	tree_print_curr(tree, print);
	
	printf("Attempting to descend left\n");
	if(!tree_descend(tree, FALSE)) printf("Could not descend\n");
	else printf("Descended\n");
	tree_print_curr(tree, print);

	if(tree_reached_leaf(tree)) printf("We are on a leaf\n");

	tree_print(tree, print2);

	tree_destroy(&tree);
	tree_destroy(&tree);
	tree_destroy(&tree);
	tree_destroy(&tree);
	return 0;
}
