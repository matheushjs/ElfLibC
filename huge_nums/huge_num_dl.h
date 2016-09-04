#ifndef _HUGE_NUM_H
#define _HUGE_NUM_H

#define DIGITS 1000
#include <stdio.h>

typedef struct {
	//Most significant digit: d[DIGITS-1]
	//Least significant: d[0]
	unsigned char d[DIGITS];
	unsigned char sig;
} num_t;

typedef enum {
	FALSE,
	TRUE
} bool;

num_t *num_create();
void num_destroy(num_t **n);
num_t *num_copy(num_t *n);
num_t *num_fread(FILE *stream);
void num_print(num_t *n);
bool mag_equal(num_t *n1, num_t *n2);
bool num_equal(num_t *n1, num_t *n2);
bool mag_higher(num_t *n1, num_t *n2);
bool num_higher(num_t *n1, num_t *n2);
num_t *mag_sum(num_t *n1, num_t *n2);
num_t *mag_sub(num_t *n1, num_t *n2);
num_t *num_sum(num_t *n1, num_t *n2);
num_t *num_sub(num_t *n1, num_t *n2);
void num_shift_l(num_t *n1, int offset);
num_t *num_mult(num_t *n1, num_t *n2);
num_t *num_div(num_t *n1, num_t *n2);

#endif
