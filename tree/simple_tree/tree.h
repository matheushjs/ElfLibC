#ifndef _TREE_H
#define _TREE_H

/* Set the tree data_type here	*/
/* The declaration of the data_type should be in this file also	*/
typedef int data_t;

typedef enum {FALSE, TRUE} bool;
typedef struct tree_struct tree_t;

tree_t * tree_alloc();
void tree_destroy(tree_t **);
bool tree_is_empty(tree_t *);
bool tree_reached_leaf(tree_t *);
data_t *tree_get_data(tree_t *);
bool tree_change_data(tree_t *, data_t *);
bool tree_append_data(tree_t *, data_t *, bool);
bool tree_descend(tree_t *, bool);
bool tree_rewind(tree_t *);
void tree_print_curr(tree_t *, void (*)(data_t *));
void tree_print(tree_t *, void (*)(data_t *));

#endif
