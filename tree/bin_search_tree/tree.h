#ifndef _TREE_H
#define _TREE_H

#include <stdbool.h>

typedef enum {
	PREORD,
	INORD,
	POSORD,
	BREADTH
} order_t;

typedef struct tree_struct tree_t;

tree_t *tree_alloc(bool (*higher)(void *, void *), bool (*equal)(void *, void *),
		void (*free)(void *), void (*print)(void *));
bool tree_insert(tree_t *, void *);
bool tree_search(tree_t *, void *);
bool tree_remove(tree_t *, void *);
void tree_print(tree_t *, order_t);
void tree_destroy(tree_t **);

#endif
