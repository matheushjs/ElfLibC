#include <stdbool.h>
#include <utils.h>

void bubblesort(int *vec, int size){
	int i;
	bool good;
	do
		for(good = true, i = 1; i < size; i++)
			if(vec[i-1] > vec[i]) SWAP(vec[i-1], vec[i]), good = false;
	while(!good);
}
