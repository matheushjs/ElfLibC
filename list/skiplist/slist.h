#ifndef _LIST_H
#define _LIST_H

//The list will store data_t entirely inside each node in the list.
typedef int data_t;
typedef struct slist_struct slist_t;
typedef enum { FALSE, TRUE } bool;

//cmp_data() should yield TRUE when comparing cmp_data(first_node, last_node);
void slist_set_cmp_data(bool (*)(data_t *, data_t *));
void slist_set_equal_data(bool (*)(data_t *, data_t *));
void slist_set_print_data(void (*)(data_t *));

//Use with caution. Leave as default functions, if possible.
//free_data should free all memory blocks allocated by copy_data.
void slist_set_free_data(void (*)(data_t *));
void slist_set_copy_data(void (*)(data_t *dest, data_t *src));

slist_t *slist_alloc();
void slist_destroy(slist_t **);
bool slist_is_empty(slist_t *);
void slist_insert(slist_t *, data_t *);
data_t *slist_search(slist_t *, data_t *, int *);

//Be aware that list_retrieve() returns a pointer to the data stored.
data_t *slist_retrieve(slist_t *, int);
void slist_remove(slist_t *, int);
void slist_remove_key(slist_t *, data_t *);
void slist_print(slist_t *);
int slist_size(slist_t *);

#endif
