#include <stdlib.h>
#include <stdio.h>
#include <vector.h>

enum {
	PROGNAME,
	VECSIZE,
	NARGS
};

int main(int argc, char *argv[]){
	int *vec, size;

	if(argc != NARGS){
		printf("Usage: ./prog [size]\n");
		return 1;
	}

	size = atoi(argv[VECSIZE]);
	vec = vector_create_random(size, 100);
	vector_print(vec, size);

	free(vec);
	return 0;
}
