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


// Documented in header file.
ElfGraph *elfGraph_new(int N, bool oriented){
	if(N < 0) ELF_DIE("Number of vertixes cannot be negative");
	ElfGraph *new = malloc(sizeof(ElfGraph));
	new->size = N;
	new->oriented = oriented;
	new->array = calloc(sizeof(ElfList *), N);
	for(N--; N >= 0; N--)
		new->array[N] = elfList_newWithEqual(
			 elf_edge_greater, elf_edge_equal );
	return new;
}

// Documented in header file.
void elfGraph_destroy(ElfGraph **graph_p){
	int i;
	ElfGraph *graph = *graph_p;

	if(graph){
		for(i = 0; i < graph->size; i++)
			elfList_destroyF(graph->array + i, free);
		free(graph->array);
		free(graph);
		*graph_p = NULL;
	}
}

// Documented in header file.
int elfGraph_size(const ElfGraph *graph){
	return graph->size;
}

// Documented in header file.
void elfGraph_addEdge(ElfGraph *graph, int src, int dest, int weight){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(src < 0 || dest < 0 || src >= graph->size || dest >= graph->size) ELF_DIE("Invalid vertix indexes");
	
	Edge *edge = malloc(sizeof(Edge));
	edge->target = dest;
	edge->weight = weight;
	elfList_insertUnique(graph->array[src], edge);

	if(!graph->oriented){
		edge = malloc(sizeof(Edge));
		edge->target = src;
		edge->weight = weight;
		elfList_insertUnique(graph->array[dest], edge);
	}
}

// Documented in header file.
void elfGraph_removeEdge(ElfGraph *graph, int src, int dest){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(src < 0 || dest < 0 || src >= graph->size || dest >= graph->size) ELF_DIE("Invalid vertix indexes");
	
	Edge tmp = {.target = dest}, tmp2 = {.target = src};
	elfList_removeValue(graph->array[src], &tmp);
	if(!graph->oriented)
		elfList_removeValue(graph->array[dest], &tmp2);
}

// Documented in header file.
void elfGraph_print(const ElfGraph *graph){
	if(!graph) ELF_DIE("Received NULL pointer");
	int i, dimension = graph->size;
	for(i = 0; i < dimension; i++){
		printf("%d. ", i);
		elfList_traverse(graph->array[i], elf_edge_print);
		printf("\n");
	}
}

// Documented in header file.
void elfGraph_printAdjacent(const ElfGraph *graph, int subjectVertex){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(subjectVertex >= graph->size || subjectVertex < 0) ELF_DIE("Invalid subjectVertex");
	elfList_traverse(graph->array[subjectVertex], elf_edge_print);
}

// Documented in header file.
ElfGraph *elfGraph_transpose(const ElfGraph *graph){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(!graph->oriented){
		fprintf(stderr, "Received undirected graph. Can't transpose it.\n");
		return NULL;
	}

	ElfGraph *result;
	int i, n;

	result = elfGraph_new(graph->size, graph->oriented);

	for(i = 0, n = graph->size; i < n; i++){
		ElfList *list = graph->array[i];
		ElfListIt *it = elfList_getIterator(list);
		Edge *ed;

		while(it != NULL){
			ed = it->key;
			elfGraph_addEdge(result, ed->target, i, ed->weight);
			it = it->next;
		}
	}

	return result;
}

// Documented in header file.
void elfGraph_readFromFileVE(ElfGraph *graph, FILE *fp, int lim){
	if(!graph) ELF_DIE("Received NULL pointer");
	
	int i, src, dest;
	for(i = 0; i != lim; i++){ //lim = -1 means a forever loop.
		if(fscanf(fp, "%d %d", &src, &dest) != 2) break;
		if(src < 0 || dest < 0 || src >= graph->size || dest >= graph->size)
			ELF_DIE("Invalid vertixes in file.");
		elfGraph_addEdge(graph, src, dest, 0);
	}
}

// Documented in header file.
void elfGraph_readFromFileVEW(ElfGraph *graph, FILE *fp, int lim){
	if(!graph) ELF_DIE("Received NULL pointer");
	
	int i, src, dest, wei;
	for(i = 0; i != lim; i++){ //lim = -1 means a forever loop.
		if(fscanf(fp, "%d %d %d", &src, &dest, &wei) != 3) break;
		if(src < 0 || dest < 0 || src >= graph->size || dest >= graph->size)
			ELF_DIE("Invalid vertixes in file.");
		elfGraph_addEdge(graph, src, dest, wei);
	}
}

//Static global variables to use on DFS exploring,
//  to avoid overusing stack memory.
//TODO: Put all these structs inside the graph struct.
static int *dfs_pred = NULL;        //predecessor vector
static char *dfs_color = NULL;      //color vector
static int *dfs_time_vec = NULL;    //visit time vector
static int *dfs_finish_vec = NULL;  //finishing time vector
static int dfs_time = 0;            //records current "time"
static void (*dfs_after_func) (int vert, void *data) = NULL; // function to execute on vertix finishing
static void *dfs_after_data = NULL;

//Visiting procedure for the DFS.
//Should only be called after a call to elfGraph_DFS_visit_initialize().
//elfGraph_DFS_visit_finalize() should be called to
//  clean resources used and to retrieve the produced vectors.
static
void elfGraph_DFS_visit(const ElfGraph *graph, int current){
	ElfListIt *iterator = elfList_getIterator(graph->array[current]);
	Edge *edge;

	dfs_color[current] = 'g'; //gray
	dfs_time_vec[current] = dfs_time++;

	while(iterator != NULL){
		edge = iterator->key;
		if(dfs_color[edge->target] == 'w'){
			dfs_pred[edge->target] = current;
			elfGraph_DFS_visit(graph, edge->target);
		}
		iterator = iterator->next;
	}
	if(dfs_after_func) dfs_after_func(current, dfs_after_data);
	dfs_finish_vec[current] = dfs_time++;
	dfs_color[current] = 'b'; //black
}

//Initialize the static global variables for use in elfGraph_DFS_visit().
static inline
void elfGraph_DFS_visit_initialize(const ElfGraph *graph){
	int i, n;
	dfs_pred = malloc(sizeof(int) * graph->size);
	dfs_color = malloc(sizeof(char) * graph->size);
	dfs_time_vec = malloc(sizeof(int) * graph->size);
	dfs_finish_vec = malloc(sizeof(int) * graph->size);
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
//If 'finish_vec' is not NULL, it receives the vector of time finished.
static inline
void elfGraph_DFS_visit_finalize(int **pred_vec, int **time_vec, int **finish_vec){
	free(dfs_color);
	
	if(pred_vec) *pred_vec = dfs_pred;
	else free(dfs_pred);

	if(time_vec) *time_vec = dfs_time_vec;
	else free(dfs_time_vec);

	if(finish_vec) *finish_vec = dfs_finish_vec;
	else free(dfs_finish_vec);

	dfs_color = NULL;
	dfs_pred = NULL;
	dfs_time_vec = NULL;
	dfs_finish_vec = NULL;
	dfs_after_func = NULL;
	dfs_after_data = NULL;
}

// Documented in header file.
void elfGraph_DFS_registerAfterFunc(void (*func)(int vert, void *data), void *data){
	if(!func) ELF_DIE("Received NULL pointer");
	dfs_after_func = func;
	dfs_after_data = data;
}

// Documented in header file.
void elfGraph_DFS_src(const ElfGraph *graph, int src, int **pred_p, int **time_p, int **finish_p){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(src >= graph->size || src < 0) ELF_DIE("Invalid source vertix");

	elfGraph_DFS_visit_initialize(graph);
	elfGraph_DFS_visit(graph, src);
	elfGraph_DFS_visit_finalize(pred_p, time_p, finish_p);
}

// Documented in header file.
void elfGraph_DFS_all(const ElfGraph *graph, int **pred_p, int **time_p, int **finish_p){
	if(!graph) ELF_DIE("Received NULL pointer");

	elfGraph_DFS_visit_initialize(graph);

	int i, n;
	for(i = 0, n = elfGraph_size(graph); i < n; i++){
		if( dfs_color[i] == 'w')
			elfGraph_DFS_visit(graph, i);
	}

	elfGraph_DFS_visit_finalize(pred_p, time_p, finish_p);
}

static
void insert_vertix_into_list(int vert, void *data){
	printf("Adding: %d\n", vert);
	elfList_insert(data, ELF_INT_TO_POINTER(vert));
}

/* The idea is to return an array of lists, each list corresponding to a component
 * Each list is terminated by a '-1'.
 * Why this decision?
 *     - What I'm thinking is that the user might also want 2 graphs, 1 graph for connected components
 *     and 1 graph for the inner edges of each component. Returning an array of lists would make it
 *     easier to create another function that returns these 2 graphs.
 *     - TODO: To implement what is above:
 *     For each list V[i]:
 *         For each integer I in V[i]:
 *             Find the adjacency list of I in the original graph
 *             For each integer K in the adjacency list:
 *                 if K is in V: add the I-K edge to the graph inner[V[i]]
 *                 else: add the edge between components containing K and I to the
 *                       graph outer.
 */
ElfList **elfGraph_stronglyConnectedComponents(const ElfGraph *graph){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(!graph->oriented){
		fprintf(stderr, "Cannot get strongly connected components of an undirected graph.\n");
		return NULL;
	}

	int i, n, j, size, aux, *finish, *idx;
	ElfList **result;
	ElfGraph *trans;

	n = elfGraph_size(graph);
	idx = malloc(sizeof(int) * n); //Vector of indexes
	for(i = 0; i < n; i++)
		idx[i] = i; //Initialize indexes
	elfGraph_DFS_all(graph, NULL, NULL, &finish);

	//TODO: Add ElfVector to the project, with efficient sorting methods.

	// Sort indexes decrescently based on finish vector.
	for(i = 1; i < n; i++){
		for(j = i-1; j >= 0; j--){
			if(finish[j+1] < finish[j]){
				aux = finish[j];
				finish[j] = finish[j+1];
				finish[j+1] = aux;

				aux = idx[j];
				idx[j] = idx[j+1];
				idx[j+1] = aux;
			}
		}
	}
	free(finish);

	//vector idx now contains indexes ordered from earliest finish-time to latest.

	trans = elfGraph_transpose(graph);
	elfGraph_DFS_visit_initialize(trans);
	result = NULL;
	size = 0;
	for(i = n-1; i >= 0; i--){
		if(dfs_color[idx[i]] == 'w'){
			result = (ElfList **) realloc(result, sizeof(ElfList *) * (size+1));
			result[size] = elfList_new(ELF_POINTER_TO_INT_GREATER);
			elfGraph_DFS_registerAfterFunc(insert_vertix_into_list, result[size]);
			size++;
			elfGraph_DFS_visit(trans, idx[i]);
		}
	}
	elfGraph_DFS_visit_finalize(NULL, NULL, NULL);
	free(idx);
	elfGraph_destroy(&trans);

	result = (ElfList **) realloc(result, sizeof(ElfList *) * (size+1));
	result[size] = NULL;

	return result;
}

// Documented in header file.
int *elfGraph_BFS(const ElfGraph *graph, int src, int **dist_p){
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
	ElfQueue *queue = elfQueue_new();
	elfQueue_push(queue, ELF_INT_TO_POINTER(src)); //macro defined in elfQueue.h
	color[src] = 'g';

	int current;
	while(elfQueue_size(queue) != 0){
		current = ELF_POINTER_TO_INT(elfQueue_pop(queue));
		
		//works even for the first vertex, because distances start at -1.
		dist_vec[current] = dist_vec[pred[current]] + 1;
		
		//Traverse the nodes adjacent to the current node.
		ElfListIt *iterator = elfList_getIterator(graph->array[current]);
		while(iterator != NULL){
			int target = ((Edge *) iterator->key)->target;
			if(color[target] == 'w'){
				//If they are white, add to queue.
				color[target] = 'g';
				pred[target] = current;
				elfQueue_push(queue, ELF_INT_TO_POINTER(target));
			}
			iterator = iterator->next;
		}
		color[current] = 'b';
	}
	elfQueue_destroy(&queue);

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
