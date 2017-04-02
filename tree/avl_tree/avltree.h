#ifndef _AVLTREE_H
#define _AVLTREE_H

typedef struct tree_struct tree_t;

//printing function may be given as NULL.
tree_t *tree_alloc(bool (*higher)(const void*,const void*), bool (*equal)(const void*,const void*),
		void (*dfree)(void*), void (*print)(const void *));
void tree_print_function(tree_t *tree, void (*print)(const void *));
void tree_insert(tree_t *, void *);
bool tree_search(const tree_t *, const void *);
bool tree_remove(tree_t *, const void *);
void tree_destroy(tree_t **);
void tree_print_inorder(const tree_t *);
void tree_print_postorder(const tree_t *);
void tree_print_preorder(const tree_t *);
void *tree_highest(const tree_t *);
void *tree_lowest(const tree_t *);
void *tree_successor(const tree_t *, void *);
void *tree_predecessor(const tree_t *, void *);

#endif
