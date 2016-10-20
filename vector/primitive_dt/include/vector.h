#ifndef _VECTOR_H
#define _VECTOR_H

//data_t must be char, int or long int.
//Has to be signed because of the maxmin function.
typedef int data_t;
#define PRINTMASK "%d"

data_t *vector_alloc(int);
data_t *vector_calloc(int);
data_t *vector_create_random(int, int);
void vector_print(data_t *, int);
void vector_maxmin(data_t *, int, data_t *, data_t *);

#endif
