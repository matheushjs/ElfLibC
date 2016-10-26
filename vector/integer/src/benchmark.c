#include <stdlib.h>
#include <stdio.h>
#include <vector.h>
#include <time.h>
#include <countingsort.h>

int main(int argc, char *argv[]){
	int *vec, bound;
	unsigned int size;
	clock_t begin, end;
	int *R;

	bound = 100;
	for(size = 1000; size < 1<<30; size += 1000){
		vec = vector_create_random(size, bound);
		
		begin = clock();
		/* Code being benchmarked starts here */
		//quicksort(vec, size);
		R = countingsort(vec, size);
		/* Code being benchmarked ends here */
		end = clock();

		free(R);
		fprintf(stderr, "%d %lf\n", size, (end-begin)/(double) CLOCKS_PER_SEC);
		free(vec);
	}

	return 0;
}
