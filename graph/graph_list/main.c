#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <elf_graph.h>

//Recursive procedure that prints the path from 'source' to 'target',
//  using the predecessor vector 'pred'.
bool print_path(int *pred, int source, int target){
	if(target != source && target == pred[target]){
		//There is no path between source and target
		return false;
	} else if(target != source && target != pred[target]){
		if(print_path(pred, source, pred[target]) == false){
			//Further within the recursion, it was discovered
			//that there was no path between source and target.
			return false;
		}
	}

	printf("%d ", target);
	return true;
}

void solve(ElfGraph *graph){
	int src, dest;
	int **pred_array;

	//pred_array will record which predecessor vectors we already got.
	//This will prevent running DFS algorithm in the same vertix twice.
	pred_array = calloc(sizeof(int *), elfGraph_size(graph));

	while(scanf("%d %d", &src, &dest) == 2){
		if(pred_array[src] == NULL)
			elfGraph_DFS_src(graph, src, pred_array + src, NULL, NULL);
		print_path(pred_array[src], src, dest);
		printf("\n");
	}

	int i, n;
	for(i = 0, n = elfGraph_size(graph); i < n; i++)
		free(pred_array[i]);
	free(pred_array);
}

void solve2(ElfGraph *graph){
	int src, dest;
	int *pred_array;

	elfGraph_DFS_all(graph, &pred_array, NULL, NULL);
	
	while(scanf("%d %d", &src, &dest) == 2){
		print_path(pred_array, src, dest);
		printf("\n");
	}

	free(pred_array);
}

int main(int argc, char *argv[]){
	int size, edges;
	scanf("%d %d", &size, &edges);
	
	ElfGraph *graph = elfGraph_new(size, true);
	elfGraph_readFromFileVE(graph, stdin, edges);

	solve2(graph);

	elfGraph_destroy(&graph);
	return 0;
}
