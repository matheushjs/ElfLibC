#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define DIE(X) fprintf(stderr, "ERROR:%s:%d: %s\n", __FILE__, __LINE__, X), exit(EXIT_FAILURE)
#define ROTATED -3 //Should be any char outside interval [-2, 2]

typedef struct node_struct {
	void *data;
	struct node_struct *left, *right;
	char bf;	//balancing factor.
} node_t;

typedef struct tree_struct tree_t;
struct tree_struct {
	node_t *root;

	bool (*data_higher)(void *, void *);
	bool (*data_equal)(void *, void *);
	void (*data_free)(void *);
	void (*data_print)(void *);
};

static
node_t *node_create(void *data){
	node_t *res = (node_t *) calloc(sizeof(node_t), 1);
	res->data = data;
	return res;
}

tree_t *tree_alloc(bool (*higher)(void*,void*), bool (*equal)(void*,void*),
		void (*dfree)(void*), void (*print)(void *)){
	tree_t *res;
	res = (tree_t *) calloc(sizeof(tree_t), 1);
	res->data_higher = higher;
	res->data_equal = equal;
	res->data_free = dfree;
	res->data_print = print;
	return res;
}

static
void rotate_r(node_t **node){
	node_t *left = (*node)->left;
	(*node)->bf = 0;
	left->bf = 0;

	(*node)->left = left->right;
	left->right = *node;
	*node = left;
}

static
void rotate_l(node_t **node){
	node_t *right = (*node)->right;
	(*node)->bf = 0;
	right->bf = 0;

	(*node)->right = right->left;
	right->left = *node;
	*node = right;
}

//Returns the balancing factor of current node.
//If a rotation occured, returns ROTATED, signaling the previous function calls to
//not change any other node's balancing factor.
static
char insert_op(tree_t *tree, node_t **node_ptr, void *data){
	char value;
	node_t *node = *node_ptr;

	if(tree->data_higher(data, &node->data)){
		//Go to the right branch
		if(!node->right){
			node->right = node_create(data);
			node->bf--; //Always becomes 1, 0 or -1.
		} else {
			value = insert_op(tree, &node->right, data);
			if(value == ROTATED) return ROTATED;
			else if(value) node->bf--;
		}
	} else if (!tree->data_equal(data, &node->data)){
		//Go to the left branch
		if(!node->left){
			node->left = node_create(data);
			node->bf++; //Always becomes 1, 0 or -1.
		} else {
			value = insert_op(tree, &node->left, data);
			if(value == ROTATED) return ROTATED;
			else if (value) node->bf++;
		}
	} else return 0; //Data is equal and was not inserted.

	//check rebalancing.
	if(node->bf == -2){
		if(node->right->bf == 1) rotate_r(&node->right);
		rotate_l(node_ptr);
		return ROTATED;
	} else if(node->bf == 2){
		if(node->left->bf == -1) rotate_l(&node->left);
		rotate_r(node_ptr);
		return ROTATED;
	}

	return (*node_ptr)->bf;
}

void tree_insert(tree_t *tree, void *data){
	if(!tree || !data){ DIE("Invalid pointer"); return; }
	if(!tree->root) tree->root = node_create(data);
	else insert_op(tree, &tree->root, data);
}

bool tree_search(tree_t *tree, void *data){
	node_t *cur = tree->root;
	while(cur){
		if(tree->data_equal(data, cur->data))
			return true;
		else if(tree->data_higher(data, cur->data))
			cur = cur->right;
		else cur = cur->left;
	}
	return false;
}

static
void destroy_op(tree_t *tree, node_t *node){
	if(!node) return;
	destroy_op(tree, node->right);
	destroy_op(tree, node->left);
	tree->data_free(node->data);
	free(node);
}

void tree_destroy(tree_t **tree){
	if(!*tree) return;
	destroy_op(*tree, (*tree)->root);
	free(*tree);
	*tree = NULL;
}

static
void print_op(tree_t *tree, node_t *node){
	if(!node) return;
	if(node->left) printf("{");
	print_op(tree, node->left);
	if(node->left) printf("} ");
	tree->data_print(node->data);
	if(node->right) printf(" {");
	print_op(tree, node->right);
	if(node->right) printf("}");
}

void tree_print(tree_t *tree){
	if(!tree) return;
	print_op(tree, tree->root);
}
