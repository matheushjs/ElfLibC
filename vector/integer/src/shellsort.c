#include <utils.h>
#include <vector.h>

void shellsort(data_t *vec, int size){
	int i, j, h = 1;
	while(h < size) h = h*3 + 1;
	do{
		h /= 3;
		for(i = h; i < size; i++){
			j = i-h;
			while(j >= 0 && vec[j+h] < vec[j]){
				SWAP(vec[j+h], vec[j]);
				j -= h;
			}
		}
	}while(h != 1);
}
