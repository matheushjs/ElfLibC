#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector.h>

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

	for(i = 0; i < size; i++)
		if(vector[i] > max) max = vector[i];
		else if(vector[i] < min) min = vector[i];

	*rmax = max;
	*rmin = min;
}

static int rmax, rmin;
void vector_maxmin_r_op(data_t *vector, int beg, int end){
	int mid = (end+beg)/2;
	if(mid == beg) return;
	if(end - beg == 1){
		
	}
	vector_maxmin_r_op(vector, mid, end);
	vector_maxmin_r_op(vector, mid-1, end);
}

void vector_maxmin_r(data_t *vector, int size, data_t *max, data_t *min){
	rmax = 1<<(sizeof(data_t) * 8 - 1);
	rmin = ~rmax;
	vector_maxmin_r_op(vector, 0, size-1);
	*max = rmax;
	*min = rmin;
}

int main(int argc, char *argv[]){
	data_t *vec = vector_create_random(20, 100);
	data_t max, min;

	vector_maxmin(vec, 20, &max, &min);
	printf("%d %d\n", max, min);
	vector_print(vec, 20);

	free(vec);
	return 0;
}
