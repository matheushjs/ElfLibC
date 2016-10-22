#include <utils.h>
#include <vector.h>

#define ROOT 0
#define LEFT(X) X*2+1
#define RIGHT(X) X*2

void max_heapify(data_t *vec, int size, int index){
	int left, right, largest;

	largest = index;
	left = LEFT(index);
	right = RIGHT(index);

	if(left < size && vec[left] > vec[largest])
		largest = left;
	if(right < size && vec[right] > vec[largest])
		largest = right;

	if(largest != index){
		SWAP(vec[index], vec[largest]);
		max_heapify(vec, size, largest);
	}
}

void build_max_heap(int *vec, int size){
	int i;
	for(i = size/2-1; i >= 0; i--)
		max_heapify(vec, size, i);
}

void heapsort(data_t *vec, int size){
	int i;
	build_max_heap(vec, size);
	for(i = size-1; i; i--){
		SWAP(vec[ROOT], vec[i]);
		max_heapify(vec, i, ROOT);
	}
}
