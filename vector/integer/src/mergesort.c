#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <vector.h>

void merge(data_t *vec, data_t *dup1, data_t *dup2, int size1, int size2){
	int total = size1 + size2;
	do{
		total--;
		if(!size1){
			size2--;
			SWAP(vec[total], dup2[size2]);
		} else if (!size2){
			size1--;
			SWAP(vec[total], dup1[size1]);
		} else if (dup1[size1-1] > dup2[size2-1]){
			size1--;
			SWAP(vec[total], dup1[size1]);
		} else {
			size2--;
			SWAP(vec[total], dup2[size2]);
		}
	} while(total);
}

void mergesort(data_t *vec, int size){
	data_t *dup;
	int mid;

	if(size == 1) return;
	mid = size/2;
	
	dup = (data_t *) malloc(sizeof(data_t) * size);
	memcpy(dup, vec, sizeof(data_t) * size);

	mergesort(dup, mid);
	mergesort(dup+mid, size-mid);

	merge(vec, dup, dup+mid, mid, size-mid);

	free(dup);
}
