#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <avltree.h>

#define DIE(X) fprintf(stderr, "ERROR:%s:%d: %s\n", __FILE__, __LINE__, X), exit(EXIT_FAILURE)
#define ROTATED -3 //Should be any char outside interval [-2, 2]

typedef struct node_struct {
	struct node_struct *left, *right;
	data_t data;
	char bf;	//balancing factor.
} node_t;

struct tree_struct {
	node_t *root;
};

static bool (*data_higher)(data_t *, data_t *) = NULL;
static bool (*data_equal)(data_t *, data_t *) = NULL;
static void (*data_copy)(data_t *, data_t *) = NULL;
static void (*data_free)(data_t *) = NULL;
static void (*data_print)(data_t *) = NULL;

/*
 * Node functions
 */

static
node_t *node_alloc(){
	return (node_t *) calloc(sizeof(node_t), 1);
}

static
node_t *node_create(data_t *data){
	node_t *res = node_alloc();
	if(data_copy) data_copy(&res->data, data);
	else memcpy(&res->data, data, sizeof(data_t));
	return res;
}

static
void node_destroy(node_t **node){
	if(*node){
		if(data_free) data_free(&(*node)->data);
		free(*node);
		*node = NULL;
	}
}

//Function for merely freeing the memory used by the node.
static
void node_free(node_t **node){
	if(*node){
		free(*node);
		*node = NULL;
	}
}


/*
 * Tree functions
 */

void tree_set_higher(bool (*higher)(data_t *, data_t *)){ data_higher = higher; }
void tree_set_equal(bool (*equal)(data_t *, data_t *)){ data_equal = equal; }
void tree_set_copy(void (*copy)(data_t *, data_t *)){ data_copy = copy; }
void tree_set_free(void (*free_func)(data_t *)){ data_free = free_func; }
void tree_set_print(void (*print)(data_t *)){ data_print = print; }

tree_t *tree_alloc(){
	return (tree_t *) calloc(sizeof(tree_t), 1);
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
char insert_op(node_t **node_ptr, data_t *data){
	char value;
	node_t *node = *node_ptr;

	if(data_higher(data, &node->data)){
		//Go to the right branch
		if(!node->right){
			node->right = node_create(data);
			node->bf--; //Always becomes 1, 0 or -1.
		} else {
			value = insert_op(&node->right, data);
			if(value == ROTATED) return ROTATED;
			else if(value) node->bf--;
		}
	} else if (!data_equal(data, &node->data)){
		//Go to the left branch
		if(!node->left){
			node->left = node_create(data);
			node->bf++; //Always becomes 1, 0 or -1.
		} else {
			value = insert_op(&node->left, data);
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

void tree_insert(tree_t *tree, data_t *data){
	if(!tree || !data){ DIE("Invalid pointer");  return; }
	if(!tree->root) tree->root = node_create(data);
	else insert_op(&tree->root, data);
}

static
void destroy_op(node_t *node){
	if(!node) return;
	destroy_op(node->right);
	destroy_op(node->left);
	node_destroy(&node);
}

void tree_destroy(tree_t **tree){
	if(!*tree) return;
	destroy_op((*tree)->root);
	free(*tree);
	*tree = NULL;
}

static
void print_op(node_t *node){
	if(!node) return;
	if(node->left) printf("{");
	print_op(node->left);
	if(node->left) printf("} ");
	data_print(&node->data);
	if(node->right) printf(" {");
	print_op(node->right);
	if(node->right) printf("}");
}

void tree_print(tree_t *tree){
	if(!tree) return;
	print_op(tree->root);
}
