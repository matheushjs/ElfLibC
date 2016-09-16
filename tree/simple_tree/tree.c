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

#include <stdio.h>

typedef struct tree_node node_t;
struct tree_node {
	node_t *right, *left;
	data_t data;
};

typedef struct tree_struct {
	node_t *root;
	node_t *curr;
} tree_t;

//Creates a node with NULL children and null data.
static node_t *node_create(){
	return (node_t *) calloc(sizeof(node_t), 1);
}

//Destroy a node.
static void node_destroy(node_t **node){
	free(*node);
	*node = NULL;
}

//Destroy a node recursively.
static void node_destroy_r(node_t **node){
	if(!*node) return;
	node_destroy_r(&(*node)->right);
	node_destroy_r(&(*node)->left);
	node_destroy(node);
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
tree_t * tree_create(){
	return (tree_t *) calloc(sizeof(struct tree_struct), 1);
}

//Destroy tree and set it as a NULL pointer.
void tree_destroy(tree_t **tree){
	if(!*tree) return;
	node_destroy_r(&(*tree)->root);
	free(*tree);
	*tree = NULL;
}

//Return TRUE if tree is empty.
bool tree_is_empty(tree_t *tree){
	if(!tree) return FALSE;	//ERROR
	return tree->root == NULL ? TRUE : FALSE;
}

//Return TRUE if the current position is a node with no children.
bool tree_reached_leaf(tree_t *tree){
	if(!tree) return FALSE; //ERROR
	if(tree_is_empty(tree)) return FALSE;
	return tree->curr->right == NULL && tree->curr->left == NULL ? TRUE : FALSE;
}

//Returns the data at the current position of the given 'tree'.
data_t *tree_get_data(tree_t *tree){
	if(!tree) return NULL; //ERROR
	if(tree_is_empty(tree)) return NULL;
	data_t *data = (data_t *) malloc(sizeof(data));
	memcpy(data, &tree->curr->data, sizeof(data));
	return data;
}

//Updates data on current position in 'tree' with 'data'.
bool tree_change_data(tree_t *tree, data_t *data){
	if(!tree || !data) return FALSE; //ERROR
	if(tree_is_empty(tree)) return FALSE;
	memcpy(&tree->curr->data, data, sizeof(data));
	return TRUE;
}

//Appends a node to the 'curr' node of the given 'tree'.
bool tree_append_data(tree_t *tree, data_t *data, bool rightSide){
	node_t *node;
	if(!tree) return FALSE;

	if(data) node = node_from_data(data);
	else node = (node_t *) calloc(sizeof(node_t), 1);
	if(!node) return FALSE;

	if(tree_is_empty(tree)){
		tree->curr = tree->root = node;
		return TRUE;
	}

	if(rightSide){
		node_destroy_r(&tree->curr->right);
		tree->curr->right = node;
	} else {
		node_destroy_r(&tree->curr->left);
		tree->curr->left = node;
	}
	return TRUE;
}

//Updates the current position to one of the current node's children.
bool tree_descend(tree_t *tree, bool rightSide){
	if(!tree) return FALSE;
	if(tree_is_empty(tree)) return FALSE;
	if(rightSide){
		if(!tree->curr->right) return FALSE;
		tree->curr = tree->curr->right;
	} else {
		if(!tree->curr->left) return FALSE;
		tree->curr = tree->curr->left;
	}
	return TRUE;
}

//Updates the current position to the root node. 
bool tree_rewind(tree_t *tree){
	if(!tree) return FALSE;
	tree->curr = tree->root;
}

//Creates a new node with 'data', then set this node as the root of a new tree,
//	with children being the root nodes of 'treeR' and 'treeL'.
tree_t *tree_merge(tree_t* treeR, tree_t* treeL, data_t *data){
}

//Deletes the branch starting from, but not including, the current node.
bool tree_delete_branch(tree_t *tree, bool rightSide){
}

void tree_print_curr(tree_t *tree, void (*print)(data_t *)){
	if(!tree) return;
	if(tree_is_empty(tree)) return;
	print(&tree->curr->data);
}