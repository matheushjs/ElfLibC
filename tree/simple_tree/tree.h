typedef enum {FALSE, TRUE} bool;
typedef int data_t;

//Node structure.
//Not supposed to be used by the user.
typedef struct tree_node node_t;
struct tree_node {
	node_t *right, *left;
	data_t data;
};

typedef struct {
	node_t *parent;
	node_t *curr;
	int size;
} tree_t;
