/* This is a source code of a binary tree data structure.
 * 
 * The file provides the user with a tree structure and a customizable data structure.
 * Having estabilished whichever data type the user wants to store in the tree,
 * they then can only care about the tree structure, which comprises:
 * 	- a pointer to the root of the tree (first ancestor)
 *	- a pointer to the current position of the user in the tree
 *	- a counter of nodes within the tree
 *
 * Matheus H. J. Saldanha
 */

#include <string.h>
#include <tree.h>

//Node structure.
//Supposed to be hidden from the user.
typedef struct{
	node_t *right, *left;
	data_t data;
} node_t;

//Creates a node with NULL children and null data.
static node_t *node_create(){
	return (node_t *) calloc(sizeof(node_t));
}

//Destroy a node.
static void node_destroy(node_t **node){
	if(!node) return;
	free(*node);
	*node = NULL;
}

//Creates a node with NULL children and data_t 'data'.
static node_t *node_from_data(data_t *data){
	node_t *node = node_create();
	if(!node) return NULL;
	memcpy(&node->data, data, sizeof(data_t));
	return node;
}


/*
 * Tree functions
 */
tree_t *tree_create(){
	return (tree_t *) calloc(sizeof(tree_t));
}

void tree_destroy(){
}

//Returns the data at the current position of the given 'tree'.
data_t *tree_get_data(tree_t *tree){
}

//Updates data on current position in 'tree' with 'data'.
bool tree_change_data(tree_t *tree, data_t *data){
}

//Appends a node to the 'curr' node of the given 'tree'.
bool tree_add_node(tree_t *tree, bool rightSide){
}
