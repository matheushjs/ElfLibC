#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <elf_graph.h>
#include <elf_list.h>
#include <elf_queue.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)
#define MAX(X,Y) X>Y?X:Y
#define MIN(X,Y) X<Y?X:Y

typedef struct _Edge {
	int target;	//Vertix this edge 'points' to.
	int weight;	//Weight of the edge.
} Edge;

typedef struct _ElfGraph {
	int size;
	bool oriented;
	ElfList **array;	//Array of lists of edges.
} ElfGraph;

/* Static functions declarations (Implemented on the bottom). */
static bool elf_edge_greater(void *a, void *b);
static bool elf_edge_equal(void *a, void *b);
static void elf_edge_print(void *a);
/**/


//Creates a graph with N vertexes, oriented or not.
ElfGraph *elf_graph_new(int N, bool oriented){
	if(N < 0) ELF_DIE("Number of vertixes cannot be negative");
	ElfGraph *new = malloc(sizeof(ElfGraph));
	new->size = N;
	new->oriented = oriented;
	new->array = calloc(sizeof(ElfList *), N);
	for(N--; N >= 0; N--)
		new->array[N] = elf_list_newWithEqual(
			 elf_edge_greater, elf_edge_equal );
	return new;
}

//Deallocates all the memory used by 'graph', and sets its pointer to NULL.
//'graph' should be an ElfGraph* passed by reference (hence a double pointer).
void elf_graph_destroy(ElfGraph **graph_p){
	int i;
	ElfGraph *graph = *graph_p;

	if(graph){
		for(i = 0; i < graph->size; i++)
			elf_list_destroyF(graph->array + i, free);
		free(graph->array);
		free(graph);
		*graph_p = NULL;
	}
}

//Returns the amount of vertixes in the graph.
int elf_graph_size(const ElfGraph *graph){
	return graph->size;
}

//Adds to the graph an edge from 'src' to 'dest'.
//If the graph is not oriented, the inverse direction is also added.
void elf_graph_addEdge(ElfGraph *graph, int src, int dest, int weight){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(src < 0 || dest < 0 || src >= graph->size || dest >= graph->size) ELF_DIE("Invalid vertix indexes");
	
	Edge *edge = malloc(sizeof(Edge));
	edge->target = dest;
	edge->weight = weight;
	elf_list_insertUnique(graph->array[src], edge);

	if(!graph->oriented){
		edge = malloc(sizeof(Edge));
		edge->target = src;
		edge->weight = weight;
		elf_list_insertUnique(graph->array[dest], edge);
	}
}

//Removes, if exists, the edge going from 'src' to 'dest'.
//If the graph is not oriented, the inverse is also removed.
void elf_graph_removeEdge(ElfGraph *graph, int src, int dest){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(src < 0 || dest < 0 || src >= graph->size || dest >= graph->size) ELF_DIE("Invalid vertix indexes");
	
	Edge tmp = {.target = dest}, tmp2 = {.target = src};
	elf_list_removeValue(graph->array[src], &tmp);
	if(!graph->oriented)
		elf_list_removeValue(graph->array[dest], &tmp2);
}

//Prints the adjacency list of a graph
void elf_graph_print(const ElfGraph *graph){
	if(!graph) ELF_DIE("Received NULL pointer");
	int i, dimension = graph->size;
	for(i = 0; i < dimension; i++){
		printf("%d. ", i);
		elf_list_traverse(graph->array[i], elf_edge_print);
		printf("\n");
	}
}

//Prints indexes of vertices that are adjacent to 'subjectVertex'.
void elf_graph_printAdjacent(const ElfGraph *graph, int subjectVertex){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(subjectVertex >= graph->size || subjectVertex < 0) ELF_DIE("Invalid subjectVertex");
	elf_list_traverse(graph->array[subjectVertex], elf_edge_print);
}

//Reads a sequence of 'lim' non-weighted edges from a file.
//Reads until EOF if 'lim' is -1.
//Will read any blank-character-separated sequence of integers,
//  following the order: source vertix - destiny vertix
//VE - vertix/edge
void elf_graph_readFromFileVE(ElfGraph *graph, FILE *fp, int lim){
	if(!graph) ELF_DIE("Received NULL pointer");
	
	int i, src, dest;
	for(i = 0; i != lim; i++){ //lim = -1 means a forever loop.
		if(fscanf(fp, "%d %d", &src, &dest) != 2) break;
		if(src < 0 || dest < 0 || src >= graph->size || dest >= graph->size)
			ELF_DIE("Invalid vertixes in file.");
		elf_graph_addEdge(graph, src, dest, 0);
	}
}

//Same as above, but also reads weights.
//VEW - vertix/edge/weight
void elf_graph_readFromFileVEW(ElfGraph *graph, FILE *fp, int lim){
	if(!graph) ELF_DIE("Received NULL pointer");
	
	int i, src, dest, wei;
	for(i = 0; i != lim; i++){ //lim = -1 means a forever loop.
		if(fscanf(fp, "%d %d %d", &src, &dest, &wei) != 3) break;
		if(src < 0 || dest < 0 || src >= graph->size || dest >= graph->size)
			ELF_DIE("Invalid vertixes in file.");
		elf_graph_addEdge(graph, src, dest, wei);
	}
}

//Static global variables to use on DFS exploring,
//  to avoid overusing stack memory.
static int *dfs_pred = NULL;      //predecessor vector
static char *dfs_color = NULL;    //color vector
static int *dfs_time_vec = NULL;  //visit time vector
static int dfs_time = 0;          //records current "time"

//Visiting procedure for the DFS.
//Should only be called after a call to elf_graph_DFS_visit_initialize().
//elf_graph_DFS_visit_finalize() should be called to
//  clean resources used and to retrieve the produced vectors.
static
void elf_graph_DFS_visit(const ElfGraph *graph, int current){
	ElfListIt *iterator = elf_list_getIterator(graph->array[current]);
	Edge *edge;
	
	dfs_color[current] = 'g'; //gray
	dfs_time_vec[current] = dfs_time++;

	while(iterator != NULL){
		edge = iterator->key;
		if(dfs_color[edge->target] == 'w'){
			dfs_pred[edge->target] = current;
			elf_graph_DFS_visit(graph, edge->target);
		}
		iterator = iterator->next;
	}

	dfs_color[current] = 'b'; //black
}

//Initialize the static global variables for use in elf_graph_DFS_visit().
static inline
void elf_graph_DFS_visit_initialize(const ElfGraph *graph){
	int i, n;
	dfs_pred = malloc(sizeof(int) * graph->size);
	dfs_color = malloc(sizeof(char) * graph->size);
	dfs_time_vec = malloc(sizeof(int) * graph->size);
	for(i = 0, n = graph->size; i < n; i++){
		dfs_pred[i] = i;
		dfs_color[i] = 'w'; //white
		dfs_time_vec[i] = -1;
	}
	dfs_time = 0;
}

//Cleans resources used by DFS_visit and returns the desired vectors.
//If 'pred' is not NULL, it receives the vector of predecessors.
//If 'time_vec' is not NULL, it receives the vector of time visited.
static inline
void elf_graph_DFS_visit_finalize(int **pred, int **time_vec){
	free(dfs_color);
	
	if(pred) *pred = dfs_pred;
	else free(dfs_pred);

	if(time_vec) *time_vec = dfs_time_vec;
	else free(dfs_time_vec);
}

//Performs a DFS in the graph, finding the time/path from all vertexes to 'src'.
//Args:
//	time_p: If 'time_p' is not NULL, store in it the vector of
//	        times at which each vertex was visited during the DFS.
//
//Returns:
//	Vector of predecessors generated by the DFS
//	    pred[i] == i means the vertix could not be reached,
//	      with the exception of the source vertix, obviously
//	    pred[i] != i means vertix i could be visited and it's predecessor is pred[i]
//	    The predecessor vector must be freed by the client of the function.
int *elf_graph_DFS(const ElfGraph *graph, int src, int **time_p){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(src >= graph->size || src < 0) ELF_DIE("Invalid source vertix");

	elf_graph_DFS_visit_initialize(graph);
	elf_graph_DFS_visit(graph, src);
	
	int *pred;
	elf_graph_DFS_visit_finalize(&pred, time_p);

	return pred;
}

//Performs a BFS in the graph, finding the distance/path from all vertexes to 'src'.
//Args:
//	dist_p: If 'dist_p' is not NULL, store in it 
//	        the vector of shortest distances to each vertex.
//	        - dist[i] == -1, vertex could not be reached from 'src'.
//	        - dist[i] != -1, shortest distance from 'src' to vertex i.
//Returns:
//  Vector of predecessors generated by the DFS.
//	-   pred[i] == i means the vertix could not be reached,
//	-     with the exception of the source vertix, obviously
//	-   pred[i] != i means vertix i could be visited and it's predecessor is pred[i]
//	-  The predecessor vector must be freed by the client of the function.
int *elf_graph_BFS(const ElfGraph *graph, int src, int **dist_p){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(src >= graph->size || src < 0) ELF_DIE("Invalid source vertix");

	//Allocate and initialize vectors
	int *pred, *dist_vec;
	char *color;
	pred = malloc(sizeof(int) * graph->size);
	dist_vec = malloc(sizeof(int) * graph->size);
	color = malloc(sizeof(char) * graph->size);
	int i, n;
	for(i = 0, n = graph->size; i < n; i++){
		pred[i] = i;
		color[i] = 'w'; //white
		dist_vec[i] = -1;
	}

	//Get a queue
	ElfQueue *queue = elf_queue_new();
	elf_queue_push(queue, ELF_INT_TO_POINTER(src)); //macro defined in elf_queue.h
	color[src] = 'g';

	int current;
	while(elf_queue_size(queue) != 0){
		current = ELF_POINTER_TO_INT(elf_queue_pop(queue));
		
		//works even for the first vertex, because distances start at -1.
		dist_vec[current] = dist_vec[pred[current]] + 1;
		
		//Traverse the nodes adjacent to the current node.
		ElfListIt *iterator = elf_list_getIterator(graph->array[current]);
		while(iterator != NULL){
			int target = ((Edge *) iterator->key)->target;
			if(color[target] == 'w'){
				//If they are white, add to queue.
				color[target] = 'g';
				pred[target] = current;
				elf_queue_push(queue, ELF_INT_TO_POINTER(target));
			}
			iterator = iterator->next;
		}
		color[current] = 'b';
	}
	elf_queue_destroy(&queue);

	free(color);
	if(dist_p != NULL) *dist_p = dist_vec;
	else free(dist_vec);

	return pred;
}





/*
 * Implementation of static functions
 */
static bool elf_edge_greater(void *a, void *b){
	return ((Edge *) a)->target > ((Edge *) b)->target ? true : false;
}
static bool elf_edge_equal(void *a, void *b){
	return ((Edge *) a)->target == ((Edge *) b)->target ? true : false;
}
static void elf_edge_print(void *a){
	printf("%d(%d) ", ((Edge *) a)->target, ((Edge *) a)->weight);
}
