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

#include <stdlib.h>
#include <string.h>
#include <tree.h>

typedef struct tree_node node_t;
struct tree_node {
	node_t *right, *left;
	data_t data;
};

struct tree_structure {
	node_t *parent;
	node_t *curr;
	int size;
};

//Creates a node with NULL children and null data.
static node_t *node_create(){
	return (node_t *) calloc(sizeof(node_t), 1);
}

//Destroy a node.
static void node_destroy(node_t **node){
	if(!node) return;
	free(*node);
	*node = NULL;
}

//Destroy a node recursively.
static void node_destroy_r(node_t **node){
}

//Creates a node with NULL children and data_t 'data'.
static node_t *node_from_data(data_t *data){
	node_t *node = node_create();
	if(!node) return NULL;
	memcpy(&node->data, data, sizeof(data_t));
	return node;
}


/*
 * User-layer functions.
 */

//Create tree with 0 nodes.
tree_t *tree_create(){
	return (tree_t *) calloc(sizeof(tree_t), 1);
}

//Destroy tree and set it as a NULL pointer.
void tree_destroy(tree_t **tree){
}

//Return TRUE if tree is empty.
bool tree_empty(tree_t *tree){
}

//Return TRUE if the current position is a node with no children.
bool tree_is_leaf(tree_t *tree){
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

//Updates the current position to one of the current node's children.
bool tree_down(tree_t *tree, bool rightSide){
}

//Updates the current position to the current node's only parent.
bool tree_up(tree_t *tree){
}

//Creates a new node with 'data', then set this node as the parent of a new tree,
//	with children being the parent nodes of 'treeR' and 'treeL'.
tree_t *tree_merge(tree_t *tree1, tree_t *tree2, data_t *data){
}

//Moves the current position up to the parent,
//	then delete the old node recursively.
bool *tree_delete_branch(tree_t *tree){
}
