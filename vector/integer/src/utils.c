#include <vector.h>

void swap(data_t *vec, int i, int j){
	data_t aux = vec[i];
	vec[i] = vec[j];
	vec[j] = aux;
}
