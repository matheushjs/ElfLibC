#include "tree.h"
#include <stdio.h>

int main(int argc, char *argv[]){
	void print(data_t *data){ printf("%d\n", *data); }
	void print2(data_t *data){ printf("%d", *data); }
	data_t data;

	printf("Attempting to append node to a NULL tree\n");
	tree_append_data(NULL, NULL, TRUE);

	tree_t *tree = tree_alloc();
	tree_t *tree2 = tree_alloc();
	tree_print_curr(tree, print);

	printf("Adding 0 to the right (should just update root)\n");
	tree_append_data(tree, NULL, TRUE);
	tree_append_data(tree2, NULL, TRUE);
	tree_print_curr(tree, print);

	printf("Attempting to descend right\n");
	if(!tree_descend(tree, TRUE)) printf("Could not descend\n");
	else printf("Descended\n");
	if(!tree_descend(tree2, TRUE)) printf("Could not descend\n");
	else printf("Descended\n");

	printf("Adding 2 to the right\n");
	data = 2;
	tree_append_data(tree, &data, TRUE);
	tree_append_data(tree2, &data, TRUE);
	
	printf("Attempting to descend right\n");
	if(!tree_descend(tree, TRUE)) printf("Could not descend\n");
	else printf("Descended\n");
	if(!tree_descend(tree2, TRUE)) printf("Could not descend\n");
	else printf("Descended\n");
	
	tree_print_curr(tree, print);
	data = 3;
	printf("Attempting to change data\n");
	tree_change_data(tree, &data);
	tree_change_data(tree2, &data);
	tree_print_curr(tree, print);

	printf("Adding 1 to the right\n");
	data = 4;
	tree_append_data(tree, &data, TRUE);
	tree_append_data(tree2, &data, TRUE);
	
	printf("Attempting to descend right\n");
	if(!tree_descend(tree, TRUE)) printf("Could not descend\n");
	else printf("Descended\n");
	tree_print_curr(tree, print);
	if(!tree_descend(tree2, TRUE)) printf("Could not descend\n");
	else printf("Descended\n");

	printf("Rewinding and descending to the left\n");
	tree_rewind(tree);
	tree_rewind(tree2);
	tree_descend(tree, FALSE);
	tree_descend(tree2, FALSE);
	tree_print_curr(tree, print);

	printf("Adding 5 to the left\n");
	data = 5;
	tree_append_data(tree, &data, FALSE);
	tree_append_data(tree2, &data, FALSE);
	tree_print_curr(tree, print);
	
	printf("Attempting to descend left\n");
	if(!tree_descend(tree, FALSE)) printf("Could not descend\n");
	else printf("Descended\n");
	if(!tree_descend(tree2, FALSE)) printf("Could not descend\n");
	else printf("Descended\n");
	tree_print_curr(tree, print);

	if(tree_reached_leaf(tree)) printf("We are on a leaf\n");
	if(tree_reached_leaf(tree2)) printf("We are on a leaf\n");

	tree_print(tree, print2);
	tree_print(tree2, print2);

	data = 10;
	tree_t *mergedTree = tree_merge(&tree, &tree2, &data);

	tree_print(mergedTree, print2);

	tree_destroy(&tree);
	tree_destroy(&tree);
	tree_destroy(&tree);
	tree_destroy(&mergedTree);
	return 0;
}
