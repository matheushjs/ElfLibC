#include <stdlib.h>
#include <stdio.h>
#include <vector.h>
#include <time.h>

int main(int argc, char *argv[]){
	int size, *vec;
	clock_t begin, end;

	for(size = 1000; size < 1<<30; size += 1000){
		begin = clock();
		
		/* Code being benchmarked starts here */
		vec = vector_create_random(size, 100);

		/* Code being benchmarked ends here */
		
		end = clock();
		printf("%d %lf\n", size, (end-begin)/(double) CLOCKS_PER_SEC);
		free(vec);
	}

	return 0;
}
