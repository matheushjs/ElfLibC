#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <huge_num_dl.h>

static bool sig_overflow;	//overflow signal

num_t *num_create(){
	return (num_t *) calloc(sizeof(num_t), 1); 
}

void num_destroy(num_t **n){
	free(*n), *n = NULL;
}

num_t *num_copy(num_t *n){
	num_t *res = num_create();
	memcpy(res, n, sizeof(num_t));
	return res;
}

num_t *num_fread(FILE *stream){
	int a, i, count = 0;
	num_t *n = num_create();
	num_t *res = num_create();

	do{
		a = fgetc(stream);
	} while(isspace(a) || a == '\t');
	i = DIGITS-1;

	//Parses signal, if there is one.
	if(a == '-'){
		res->sig = 1;
	} else if(isdigit(a)){
		n->d[i] = a - 48;
		i--;
		count++;
	} else if(a != '+') return NULL;

	for(; i >= 0; i--){
		a = fgetc(stream);
		if(isdigit(a)){
			n->d[i] = a - 48;
			count++;
			continue;
		} else if(isblank(a)) continue;
		else if(a == '\n' || a == EOF) break;
		
		fprintf(stderr, "Invalid character: '%d'. To finish typing the number, type a newline.\n", a);
		num_destroy(&n), num_destroy(&res);
		return NULL;
	}
	memcpy(res->d, (n->d)+(DIGITS-count), sizeof(unsigned char) * count);
	num_destroy(&n);
	return res;
}

void num_print(num_t *n){
	int i;
	if(n->sig) printf("-");
	for(i = DIGITS-1; i >= 0 && n->d[i] == 0; i--);
	if(i < 0) printf("0");
	for(; i >= 0; i--)
		printf("%d", n->d[i]);
	printf("\n");
}

//Compares magnitude of n1 and n2.
bool mag_equal(num_t *n1, num_t *n2){
	int i;
	for(i = 0; i < DIGITS; i++)
		if(n1->d[i] != n2->d[i]) return FALSE;
	return TRUE;
}

//Compares 2 signed numbers n1 and n2.
bool num_equal(num_t *n1, num_t *n2){
	if(n1->sig != n2->sig) return FALSE;
	return mag_equal(n1, n2);
}

//Compares magnitude of n1 and n2.
bool mag_higher(num_t *n1, num_t *n2){
	int i;
	for(i = DIGITS-1; i >= 0; i--){
		if(n1->d[i] > n2->d[i]) return TRUE;
		else if(n1->d[i] < n2->d[i]) return FALSE;
	}
	return FALSE; //caso n1 == n2
}

//Compares 2 signed numbers n1 and n2.
bool num_higher(num_t *n1, num_t *n2){
	if(n1->sig != n2->sig) return n1->sig == 1 ? FALSE : TRUE;
	if(n1->sig == 0) return mag_higher(n1, n2);
	if(n1->sig == 1) return mag_higher(n2, n1);
	return mag_higher(n1, n2);
}

//Sums magnitude of n1 and n2.
num_t *mag_sum(num_t *n1, num_t *n2){
	int i, sum, car = 0;
	num_t *res = num_create();

	for(i = 0; i < DIGITS; i++){
		sum = n1->d[i] + n2->d[i] + car;
		car = sum/10;
		res->d[i] = sum % 10;
	}
	if(car != 0) printf("Overflow!\n");
	return res;
}

//Subtracts two positive numbers.
num_t *mag_sub(num_t *n1, num_t *n2){
	int i, sub, car = 0;
	num_t *dum, *res = num_create();

	if(mag_higher(n2, n1)){
		res->sig = 1;
		dum = n1;
		n1 = n2;
		n2 = dum;
	}

	for(i = 0; i < DIGITS; i++){
		sub = n1->d[i] - (n2->d[i] + car);
		if(sub < 0){
			sub += 10;
			car = 1;
		} else car = 0;
		res->d[i] = sub;
	}
	if(car != 0) printf("Error!\n");
	return res;
}

//Sums n1 and n2.
num_t *num_sum(num_t *n1, num_t *n2){
	num_t *res;

	if(n1->sig == 0 && n2->sig == 0) return mag_sum(n1, n2);
	if(n1->sig == 1 && n2->sig == 0) return mag_sub(n2, n1);
	if(n1->sig == 0 && n2->sig == 1) return mag_sub(n1, n2);
	//case both negatives:
	res = mag_sum(n1, n2);
	res->sig = 1;
	return res;
}

//Subtracts n2 from n1. (n1 - n2).
num_t *num_sub(num_t *n1, num_t *n2){
	num_t *res;
	if(n1->sig == 0 && n2->sig == 1) return mag_sum(n1, n2);
	if(n1->sig == 1 && n2->sig == 1) return mag_sub(n2, n1);
	if(n1->sig == 0 && n2->sig == 0) return mag_sub(n1, n2);
	//case both negatives:
	res = mag_sum(n1, n2);
	res->sig = 1;
	return res;
}

//Multiplies n1 by d
static	//static until you get rid of sig_overflow.
num_t *num_scalar_mult(num_t *n1, unsigned char d){
	int i, car = 0, prod;
	num_t *res = num_create();
	
	sig_overflow = FALSE;
	for(i = 0; i < DIGITS; i++){
		prod = n1->d[i] * d + car;
		car = prod/10;
		res->d[i] = prod%10;
	}
	if(car != 0) sig_overflow = TRUE;
	return res;
}

//shift left
void num_shift_l(num_t *n1, int offset){
	if(offset > DIGITS){
		memset(n1->d, 0, sizeof(n1->d));
		return;
	}

	int blocksize = (DIGITS - offset)*sizeof(unsigned char);
	void *p = malloc(blocksize);
	memcpy(p, n1->d, blocksize);
	memset(n1->d, 0, sizeof(n1->d) - blocksize);
	memcpy(n1->d + offset, p, blocksize);
	free(p);
}

//Multiplies n1 by n2 (n1*n2)
num_t *num_mult(num_t *n1, num_t *n2){
	int i;
	num_t *dum1, *dum2, *res = num_create();

	for(i = 0; i < DIGITS; i++){
		dum1 = num_scalar_mult(n1, n2->d[i]);
		num_shift_l(dum1, i);
		dum2 = num_sum(res, dum1);	//PROB: Could sum directly into res.
		num_destroy(&res);
		num_destroy(&dum1);
		res = dum2;
	}
	
	//PROB: Overflow being ignored.
	if(n1->sig != n2->sig) res->sig = 1;
	return res;
}

//Divides n1 by n2 (n1/n2)
//Idea for division n1/n2:
//	shift-left n2 as much as possible (shift X times)
//	then multiply n2 by D, that goes from 10 to 0, checking when (n2<<X)*D is lower than n1.
//	when it's lower than n1, we subtract the resulting number, which is (n2<<X)*D, from n1, adding digit D to the quocient->digit[X].
//	repeat this process shifting n2 from X=DIGITS-1 to 0
num_t *num_div(num_t *n1, num_t *n2){
	int i, j, maxShift;
	num_t *shift, *dum, *dum2, *num = num_copy(n1), *res = num_create();

	//find first digit in n2
	for(i = DIGITS-1; i >= 0 && !n2->d[i]; i--);
	if(i < 0) return res;
	maxShift = DIGITS - i - 1;	//Prevents overflowing shift

	for(i = maxShift; i >= 0; i--){
		shift = num_copy(n2);
		num_shift_l(shift, i);
		for(j = 9; j;  j--){
			dum = num_scalar_mult(shift, j);
			if(sig_overflow){ num_destroy(&dum); continue; }
			if(num_higher(num, dum) || num_equal(num, dum)){	//PROB: Could make a num_lower()
				dum2 = num_sub(num, dum);	//PROB: Could subtract directly from num.
				num_destroy(&num);
				num = dum2;
				res->d[i] = j;
				break;
			}
			num_destroy(&dum);
		}
		num_destroy(&shift);
	}
	num_destroy(&num);	//PROB: Ignoring remainder of division. 'num' would be the remainder.

	//PROB: Exceptions have been poorly checked.
	if(n1->sig != n2->sig) res->sig = 1;
	return res;
}
