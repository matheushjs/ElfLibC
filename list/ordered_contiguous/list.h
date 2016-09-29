#ifndef _LIST_H
#define _LIST_H

typedef int data_t;
typedef enum {FALSE, TRUE} bool;
typedef struct list_struct list_t;

list_t *list_alloc();
void list_destroy(list_t **);
bool list_is_empty(list_t *);
void list_insert(list_t *, data_t *, bool (*)(data_t *, data_t *));
data_t *list_retrieve(list_t *, int );
void list_remove(list_t *, int );
void list_print_r(list_t *, void (*)(data_t *));
int list_size(list_t *list);

#endif
