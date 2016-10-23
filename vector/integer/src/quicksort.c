#include <vector.h>
#include <utils.h>

static
int partition(int *vector, int left, int right) {
	int i, j;

	i = left;
	for (j = i+1; j <= right; j++) {
		if (vector[j] < vector[left]) {
			++i;
			swap(vector, i, j);
		}
	}
	swap(vector, i, left);

	return i;
}

void quicksort(data_t *vec, int size){
	int mid;

	if(size < 2) return;

	mid = partition(vec, 0, size-1);
	quicksort(vec, mid);
	quicksort(vec+mid+1, size-mid-1);
}
