#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define DIE(X) fprintf(stderr, "ERROR:%s:%d: %s\n", __FILE__, __LINE__, X), exit(EXIT_FAILURE)

typedef struct node_struct node_t;
struct node_struct {
	void *data;
	node_t *left, *right;
	char bf;	//balancing factor.
};

typedef struct tree_struct tree_t;
struct tree_struct {
	node_t *root;
	bool (*data_higher)(const void *, const void *);
	bool (*data_equal)(const void *, const void *);
	void (*data_free)(void *);
	void (*data_print)(const void *);
};

static
node_t *node_create(void *data){
	node_t *res = (node_t *) calloc(sizeof(node_t), 1);
	res->data = data;
	return res;
}

tree_t *tree_alloc(bool (*higher)(const void*,const void*), bool (*equal)(const void*,const void*),
		void (*dfree)(void*), void (*print)(const void *)){
	tree_t *res;
	if(!higher || !equal || !dfree) DIE("Only printing function may be NULL");

	res = (tree_t *) calloc(sizeof(tree_t), 1);
	res->data_higher = higher;
	res->data_equal = equal;
	res->data_free = dfree;
	if(print) res->data_print = print;
	return res;
}

void tree_print_function(tree_t *tree, void (*print)(const void *)){
	tree->data_print = print;
}

//All rotation functions return 1 if the height of rotated tree is lower than before.
static
char rotate_r(node_t **node){
	char retval;
	node_t *left = (*node)->left;

	//Assumes (*node)->bf = 2, and second->bf = 0 or 1
	
	(*node)->bf = left->bf == 1 ? 0 : 1;
	retval = left->bf == 0 ? 0 : 1;
	left->bf = left->bf == 1 ? 0 : -1;

	(*node)->left = left->right;
	left->right = *node;
	*node = left;

	return retval;
}

static
char rotate_l(node_t **node){
	char retval;
	node_t *right = (*node)->right;

	//Assumes (*node)->bf = -2, and second->bf = 0 or -1
	
	(*node)->bf = right->bf == -1 ? 0 : -1;
	retval = right->bf == 0 ? 0 : 1;
	right->bf = right->bf == -1 ? 0 : 1;

	(*node)->right = right->left;
	right->left = *node;
	*node = right;

	return retval;
}

static
char rotate_lr(node_t **node){
	node_t *second, *third;
	second = (*node)->left;
	third = second->right;

	//Assumes (*node)->bf = 2, and second->bf = -1
	
	(*node)->bf = third->bf == 1 ? -1 : 0;
	second->bf = third->bf == -1 ? 1 : 0;
	third->bf = 0;

	(*node)->left = third->right;
	second->right = third->left;
	third->right = *node;
	third->left = second;
	*node = third;

	return 1;
}

static
char rotate_rl(node_t **node){
	node_t *second, *third;
	second = (*node)->right;
	third = second->left;
	
	//Assumes (*node)->bf = -2, and second->bf = 1

	(*node)->bf = third->bf == -1 ? 1 : 0;
	second->bf = third->bf == 1 ? -1 : 0;
	third->bf = 0;

	(*node)->right = third->left;
	second->left = third->right;
	third->left = *node;
	third->right = second;
	*node = third;

	return 1;
}

//At every step, calculate the increment that should occur on the balancing factor of the parent of current node.
//Return that increment.
static
char insert_op(tree_t *tree, node_t **node_ptr, void *data){
	char retval;
	node_t *node = *node_ptr;
	enum { LEFT, RIGHT } whereIWent;

	//Insert node if needed.
	if(!node){
		*node_ptr = node_create(data);
		return 1;
	}

	//Descend on tree.
	if(tree->data_higher(data, node->data)){
		whereIWent = RIGHT;
		retval = insert_op(tree, &node->right, data);
	} else if (!tree->data_equal(data, node->data)) {
		whereIWent = LEFT;
		retval = insert_op(tree, &node->left, data);
	} else{ //Data exists in tree already.
		tree->data_free(data);
		return 0;
	}

	if(retval == 0) return 0; //Height of current node's subtrees didn't change.

	//Recalculate balancing factor.
	if(whereIWent == RIGHT) node->bf--;
	else node->bf++;

	//Rotate if needed.
	if(node->bf == 0) return 0;
	else if(node->bf == -2){
		if(node->right->bf == 1) rotate_rl(node_ptr);
		else rotate_l(node_ptr);
		return 0;
	} else if(node->bf == 2){
		if(node->left->bf == -1) rotate_lr(node_ptr);
		else rotate_r(node_ptr);
		return 0;
	} else return 1;
}

void tree_insert(tree_t *tree, void *data){
	if(!tree || !data) DIE("Invalid pointer");
	insert_op(tree, &tree->root, data);
}

//Returns a pointer to the searched node. NULL if not found.
static
node_t *tree_search_ptr(const tree_t *tree, const void *data){
	node_t *cur = tree->root;
	while(cur){
		if(tree->data_equal(data, cur->data)) return cur;
		else if(tree->data_higher(data, cur->data)) cur = cur->right;
		else cur = cur->left;
	}
	return NULL;
}

bool tree_search(const tree_t *tree, const void *data){
	return tree_search_ptr(tree, data) == NULL ? false : true;
}

//Substitutes data in 'base' for the lowest value of the right subtree.
//Returns 1 if parent needs to be incremented/decremented, 0 otherwise.
static
char tree_rem_maxmin(node_t *base, node_t **node_ptr){
	char retval;
	node_t *node = *node_ptr;

	if(!node->left){
		*node_ptr = node->right;
		base->data = node->data;
		free(node);
		return 1;
	}

	retval = tree_rem_maxmin(base, &node->left);

	if(retval == 0) return 0;
	node->bf--; //Left subtree lost height.

	if(node->bf == 0) return 1;
	else if(node->bf == -1) return 0;
	else { //node->bf equals -2
		if(node->right->bf == 1) return rotate_rl(node_ptr);
		else return rotate_l(node_ptr);
	}
}

#define NOTFOUND -1
//Searches for the node where 'data' is.
//Returns 1 if parent needs to be incremented/decremented, 0 otherwise.
static
char tree_rem_search(tree_t *tree, node_t **node_ptr, const void *data){
	char retval;
	node_t *node = *node_ptr;
	enum {RIGHT, LEFT} whereIWent;

	if(!node) return NOTFOUND;

	//Decide if keep searching or find maxmin.
	if(tree->data_equal(data, node->data)){
		tree->data_free(node->data);
		if(!node->left){
			*node_ptr = node->right;
			free(node);
			return 1;
		} else if(!node->right){
			*node_ptr = node->left;
			free(node);
			return 1;
		} else {
			whereIWent = RIGHT;
			retval = tree_rem_maxmin(node, &node->right);
		}
	} else if(tree->data_higher(data, node->data)){
		whereIWent = RIGHT;
		retval = tree_rem_search(tree, &node->right, data);
	} else {
		whereIWent = LEFT;
		retval = tree_rem_search(tree, &node->left, data);
	}

	if(retval == NOTFOUND) return NOTFOUND;
	if(retval == 0) return 0;

	//Recalculate balancing factor.
	if(whereIWent == RIGHT) node->bf++;
	else node->bf--;

	//Rotate if needed.
	if(node->bf == 0) return 1;//Removed a node from the higher subtree.
	else if(node->bf == -2){ //Removed a node from the lower subtree (left).
		if(node->right->bf == 1) return rotate_rl(node_ptr);
		else return rotate_l(node_ptr);
	} else if(node->bf == 2){ //Removed a node from the lower subtree (right).
		if(node->left->bf == -1) return rotate_lr(node_ptr);
		else return rotate_r(node_ptr);
	} else return 0; //Removed a node but the subtrees had same height.
}

bool tree_remove(tree_t *tree, const void *data){
	if(!tree || !data) DIE("Invalid pointers");
	return tree_rem_search(tree, &tree->root, data) == NOTFOUND ? false : true;
}
#undef NOTFOUND

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
void print_inorder_op(const tree_t *tree, const node_t *node, const char *sep){
	if(!node) return;
	print_inorder_op(tree, node->left, sep);
	tree->data_print(node->data);
	printf("%s", sep);
	print_inorder_op(tree, node->right, sep);
}

static
void print_postorder_op(const tree_t *tree, const node_t *node, const char *sep){
	if(!node) return;
	print_postorder_op(tree, node->left, sep);
	print_postorder_op(tree, node->right, sep);
	tree->data_print(node->data);
	printf("%s", sep);
}

static
void print_preorder_op(const tree_t *tree, const node_t *node, const char *sep){
	if(!node) return;
	tree->data_print(node->data);
	printf("%s", sep);
	print_preorder_op(tree, node->left, sep);
	print_preorder_op(tree, node->right, sep);
}

void tree_print_inorder(const tree_t *tree){
	if(!tree) return;
	if(!tree->data_print){
		fprintf(stderr, "Printing function undefined.\n");
		return;
	}
	print_inorder_op(tree, tree->root, " ");
}

void tree_print_postorder(const tree_t *tree){
	if(!tree) return;
	if(!tree->data_print){
		fprintf(stderr, "Printing function undefined.\n");
		return;
	}
	print_postorder_op(tree, tree->root, " ");
}

void tree_print_preorder(const tree_t *tree){
	if(!tree) return;
	if(!tree->data_print){
		fprintf(stderr, "Printing function undefined.\n");
		return;
	}
	print_preorder_op(tree, tree->root, " ");
}

//Returns a pointer to the memory where the highest element is.
void *tree_highest(const tree_t *tree){
	node_t *node;
	if(!tree || !tree->root) return NULL;
	node = tree->root;
	while(node->right)
		node = node->right;
	return node->data;
}

//Returns a pointer to the memory where the lowest element is.
void *tree_lowest(const tree_t *tree){
	node_t *node;
	if(!tree || !tree->root) return NULL;
	node = tree->root;
	while(node->left)
		node = node->left;
	return node->data;
}

enum ancestor_t { PREDECESSOR, SUCCESSOR };
static
void *tree_neighbor(const tree_t *tree, void *key, enum ancestor_t type){
	node_t *node;
	void *succ, *pred;
	if(!tree || !tree->root) return NULL;
	node = tree->root;

	succ = pred = NULL;
	while(node){
		if(tree->data_equal(key, node->data)) break;
		else if(tree->data_higher(key, node->data)){
			pred = node->data;
			node = node->right;
		} else { //key is lower
			succ = node->data;
			node = node->left;
		}
	}
	
	//node is NULL if 'key' isnt in the AVL tree.
	if(!node) return NULL;

	if(type == SUCCESSOR){
		if(node->right){
			node = node->right;
			while(node->left) node = node->left;
			return node->data;
		} else return succ;
	} else {
		if(node->left){
			node = node->left;
			while(node->right) node = node->right;
			return node->data;
		} else return pred;
	}
}

//Wrapper functions for tree_neighbor().
void *tree_successor(const tree_t *tree, void *key){ return tree_neighbor(tree, key, SUCCESSOR); }
void *tree_predecessor(const tree_t *tree, void *key){ return tree_neighbor(tree, key, PREDECESSOR); }
