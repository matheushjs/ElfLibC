#ifndef _VECTOR_H
#define _VECTOR_H

//data_t must be char, int or long int.
//Has to be signed because of the maxmin function.
typedef int data_t;
#define PRINTMASK "%d"

data_t *vector_alloc(int size);
data_t *vector_calloc(int size);
data_t *vector_create_random(int size, int mod);
void vector_print(data_t *vector, int size);
void vector_maxmin(data_t *vector, int size, data_t *rmax, data_t *rmin);

#endif
