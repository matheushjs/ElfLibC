#include <stdlib.h>
#include <vector.h>
#include <limits.h>

data_t *countingsort(data_t *vec, int size){
	data_t *count, *res;
	int i, max, min, aux;

	max = INT_MIN;
	min = INT_MAX;

	for(i = 0; i < size; i++){
		if(vec[i] > max) max = vec[i];
		if(vec[i] < min) min = vec[i];
	}
	
	aux = max-min+1;
	count = (data_t *) calloc(sizeof(data_t), aux);
	res = (data_t *) malloc(sizeof(data_t) * size);

	for(i = 0; i < size; i++)
		count[vec[i]-min]++;

	for(i = 1; i < aux; i++)
		count[i] += count[i-1];

	for(i = size-1; i >= 0; i--){
		aux = vec[i]-min;
		count[aux]--;
		res[count[aux]] = vec[i];
	}

	free(count);
	return res;
}
