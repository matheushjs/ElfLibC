#include <stdlib.h>
#include <stdio.h>
#include <vector.h>
#include <utils.h>
#include <countingsort.h>

enum {
	PROGNAME,
	VECSIZE,
	NARGS
};

int main(int argc, char *argv[]){
	int *vec, *R, size;

	if(argc != NARGS){
		printf("Usage: ./prog [size]\n");
		return 1;
	}

	size = atoi(argv[VECSIZE]);
	vec = vector_create_random(size, 100);
	vector_print(vec, size);

	/* Sorting function goes here */
	R = countingsort(vec, size);
	/*Sorting function ends here */

	vector_print(R, size);
	free(R);

	free(vec);
	return 0;
}
