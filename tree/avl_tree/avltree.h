#ifndef _AVLTREE_H
#define _AVLTREE_H

typedef int data_t;
typedef struct tree_struct tree_t;

//Mandatory to set these 2 functions.
void tree_set_higher(bool (*)(data_t *, data_t *));
void tree_set_equal(bool (*)(data_t *, data_t *));

//Optional to set these 3 functions.
//Use with caution. The freeing function must free every memory alloc'ed by the copy function.
void tree_set_copy(void (*)(data_t *, data_t *));
void tree_set_free(void (*)(data_t *));
void tree_set_print(void (*)(data_t *));

tree_t *tree_alloc();
void tree_insert(tree_t *, data_t *);
void tree_destroy(tree_t **);
void tree_print(tree_t *);

#endif
