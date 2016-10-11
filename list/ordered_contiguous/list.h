#ifndef _LIST_H
#define _LIST_H

//The list will store data_t, not a pointer to it.
//If you want to store some data X, and you want the definition of X to be hidden from
//the list source code, then store a pointer to the opaque definition of X.

typedef int data_t;
typedef enum {FALSE, TRUE} bool;
typedef struct list_struct list_t;

//cmp_data() should yield TRUE when comparing cmp_data(first_node, last_node);
void list_set_free_data(void (*)(data_t *));
void list_set_cmp_data(bool (*)(data_t *, data_t *));
void list_set_print_data(void (*)(data_t *));

list_t *list_alloc();
void list_destroy(list_t **);
bool list_is_empty(list_t *);
void list_insert(list_t *, data_t *);

//Be aware that list_retrieve() returns a pointer to the data stored.
data_t *list_retrieve(list_t *, int);
void list_remove(list_t *, int);
void list_print(list_t *);
int list_size(list_t *list);

#endif
