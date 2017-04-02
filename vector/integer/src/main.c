#include <stdlib.h>
#include <stdio.h>
#include <vector.h>
#include <utils.h>
#include <quicksort.h>

enum {
	PROGNAME,
	VECSIZE,
	NARGS
};

int main(int argc, char *argv[]){
	int *vec, size;
	//int *R;

	if(argc != NARGS){
		printf("Usage: ./prog [size]\n");
		return 1;
	}

	size = atoi(argv[VECSIZE]);
	vec = vector_create_random(size, 100);
	vector_print(vec, size);

	/* Sorting function goes here */
	quicksort(vec, size);
	/*Sorting function ends here */

	vector_print(vec, size);

	free(vec);
	return 0;
}