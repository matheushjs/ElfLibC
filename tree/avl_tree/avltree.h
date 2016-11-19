#ifndef _AVLTREE_H
#define _AVLTREE_H

typedef struct tree_struct tree_t;

//printing and freeing functions may be given as NULL.
tree_t *tree_alloc(bool (*higher)(void*,void*), bool (*equal)(void*,void*),
		void (*dfree)(void*), void (*print)(void *));
void tree_insert(tree_t *, void *);
bool tree_search(tree_t *, void *);
void tree_destroy(tree_t **);
void tree_print(tree_t *);

#endif
