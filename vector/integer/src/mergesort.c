#include <stdlib.h>
#include <string.h>
#include <utils.h>

void merge(int *vec, int *dup1, int *dup2, int size1, int size2){
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

void mergesort(int *vec, int size){
	int *dup;
	int mid;

	if(size == 1) return;
	mid = size/2;
	
	dup = (int *) malloc(sizeof(int) * size);
	memcpy(dup, vec, sizeof(int) * size);

	mergesort(dup, mid);
	mergesort(dup+mid, size-mid);

	merge(vec, dup, dup+mid, mid, size-mid);

	free(dup);
}
