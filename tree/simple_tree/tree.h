typedef enum {FALSE, TRUE} bool;

typedef int data_t;

typedef struct{
	node_t *parent;
	node_t *curr;
	int size;
} tree_t;
