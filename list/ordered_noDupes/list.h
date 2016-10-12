#ifndef _LIST_H
#define _LIST_H

//The list will store data_t entirely inside each node in the list.

typedef int data_t;
typedef enum {FALSE, TRUE} bool;
typedef struct list_struct list_t;

//cmp_data() should yield TRUE when comparing cmp_data(first_node, last_node);
void list_set_cmp_data(bool (*)(data_t *, data_t *));
void list_set_equal_data(bool (*)(data_t *, data_t *));
void list_set_print_data(void (*)(data_t *));

//Use with caution. Leave as default functions, if possible.
//free_data should free all memory blocks allocated by copy_data.
void list_set_free_data(void (*)(data_t *));
void list_set_copy_data(void (*)(data_t *dest, data_t *src));

list_t *list_alloc();
void list_destroy(list_t **);
bool list_is_empty(list_t *);
void list_insert(list_t *, data_t *);
data_t *list_search(list_t *, data_t *, int *);

//Be aware that list_retrieve() returns a pointer to the data stored.
data_t *list_retrieve(list_t *, int);
void list_remove(list_t *, int);
void list_remove_key(list_t *, data_t *);
void list_print(list_t *);
int list_size(list_t *list);

#endif
