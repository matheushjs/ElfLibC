#include <utils.h>
#include <vector.h>

void insertionsort(data_t *vec, int size){
	int i, j;
	for(i = 1; i < size; i++){
		j = i;
		while(j-- && vec[j+1] < vec[j])
			SWAP(vec[j+1], vec[j]);
	}
}
