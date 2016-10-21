#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector.h>

#define MAX(X,Y) X > Y ? X : Y
#define MIN(X,Y) X < Y ? X : Y

data_t *vector_alloc(int size){
	return (data_t *) malloc(sizeof(data_t) * size);
}

data_t *vector_calloc(int size){
	return (data_t *) calloc(sizeof(data_t), size);
}

data_t *vector_create_random(int size, int mod){
	int i;
	data_t *vector = vector_alloc(size);
	
	srand(time(NULL));
	for(i = 0; i < size; i++)
		vector[i] = rand()%mod;

	return vector;
}

void vector_print(data_t *vector, int size){
	int i;
	for(i = 0; i < size; i++)
		printf(PRINTMASK" ", vector[i]);
	printf("\n");
}

void vector_maxmin(data_t *vector, int size, data_t *rmax, data_t *rmin){
	int i;
	data_t max, min;

	max = 1<<(sizeof(data_t) * 8 - 1);
	min = ~max;

	for(i = 0; i < size; i++){
		if(vector[i] > max) max = vector[i];
		if(vector[i] < min) min = vector[i];
	}

	*rmax = max;
	*rmin = min;
}
