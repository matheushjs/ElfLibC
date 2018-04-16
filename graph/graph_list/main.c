/*
 * MIT License
 * 
 * Copyright (c) 2018 Matheus H. J. Saldanha <mhjsaldanha@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <elf_list.h>
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

void print(void *d){
	printf("(%d) ", ELF_POINTER_TO_INT(d));
}

//test SCC
void solve3(){
	ElfList **lists, **aux;
	int edges, size;

	scanf("%d %d", &size, &edges);
	
	ElfGraph *graph = elfGraph_new(size, true);
	elfGraph_readFromFileVE(graph, stdin, edges);

	aux = lists = elfGraph_SCC(graph);
	
	while(*aux != NULL){
		elfList_traverse(*aux, print);
		printf("\n");
		elfList_destroy(aux);
		aux++;
	}

	free(lists);
	elfGraph_destroy(&graph);
}

//Test Prim
void solve4(){
	int edges, size;
	
	scanf("%d %d", &size, &edges);
	ElfGraph *graph = elfGraph_new(size, false);
	elfGraph_readFromFileVEW(graph, stdin, edges);

	ElfGraph *mst = elfGraph_MST_prim(graph);
	elfGraph_print(mst);

	elfGraph_destroy(&graph);
	elfGraph_destroy(&mst);
}

//Test Kruskal
void solve5(){
	int edges, size;
	
	scanf("%d %d", &size, &edges);
	ElfGraph *graph = elfGraph_new(size, false);
	elfGraph_readFromFileVEW(graph, stdin, edges);

	ElfGraph *mst = elfGraph_MST_kruskal(graph);
	elfGraph_print(mst);

	elfGraph_destroy(&graph);
	elfGraph_destroy(&mst);
}

//Test Dijkstra
void solve6(){
	int edges, size;
	int *pred;

	scanf("%d %d", &size, &edges);
	ElfGraph *graph = elfGraph_new(size, true);
	elfGraph_readFromFileVEW(graph, stdin, edges);

	int dist;
	dist = elfGraph_dijkstra_withTarget(graph, 1, 4, &pred);
	print_path(pred, 1, 4);
	free(pred);

	elfGraph_destroy(&graph);
}

int main(int argc, char *argv[]){
	solve6();
	return 0;
}
