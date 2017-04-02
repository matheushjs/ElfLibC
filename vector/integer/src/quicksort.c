#include <vector.h>
#include <utils.h>

/*
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
*/

#define MEDIAN(X, Y, Z) (X<=Y?(X>=Z?X:(Z<=Y?Z:Y)):(Y>=Z?Y:(X<=Z?X:Z)))
static
void partition(int *vector, int left, int right, int *di, int *dj){
	int pivot, i, j;

	pivot = MEDIAN(vector[(right+left)/2], vector[left], vector[right]);

	i = left;
	j = right;
	while(i <= j){
		while(vector[i] < pivot) i++;
		while(vector[j] > pivot) j--;
		if(i > j) break;

		swap(vector, i, j);
		i++;
		j--;
	}
	
	*di = i;
	*dj = j;
}

void quicksort_op(data_t *vec, int left, int right){
	int i, j;

	if(right-left < 1) return;

	partition(vec, left, right, &i, &j);
	quicksort_op(vec, left, j);
	quicksort_op(vec, i, right);
}

//Wrapper function
void quicksort(data_t *vec, int size){
	quicksort_op(vec, 0, size-1);
}
