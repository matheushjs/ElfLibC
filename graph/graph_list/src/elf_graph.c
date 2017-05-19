#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <elf_graph.h>
#include <elf_list.h>
#include <elf_queue.h>
#include <elf_vector.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)
#define MAX(X,Y) X>Y?X:Y
#define MIN(X,Y) X<Y?X:Y

typedef struct _ArgsDFS {
	int  *dfs_pred;        //predecessor vector
	char *dfs_color;       //color vector
	int  *dfs_time_vec;    //visit time vector
	int  *dfs_finish_vec;  //finishing time vector
	int  dfs_time;         //records current "time"
	void (*dfs_after_func) (int vert, void *data); // function to execute on vertix finishing
	void *dfs_after_data;  //Data to pass to dfs_after_func
} ArgsDFS;

typedef struct _Edge {
	int target;	//Vertix this edge 'points' to.
	int weight;	//Weight of the edge.
} Edge;

typedef struct _ElfGraph {
	int size;
	bool oriented;
	ElfList **array;	//Array of lists of edges.

	ArgsDFS dfs_args; //Mutable component (C++ concept).
	                  //If this is altered, ElfGraph should still be considered const.
} ElfGraph;

/* Some needed static functions declarations (Implemented later in this file). */
static bool elf_edge_greater(void *a, void *b);
static bool elf_edge_equal(void *a, void *b);
static void elf_edge_print(void *a);

static inline void elfGraph_ArgsDFS_reset(const ElfGraph *graph);
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

	elfGraph_ArgsDFS_reset(new);
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

//Returns a pointer to the graph's ArgsDFS, which is a mutable component of a graph.
static inline
ArgsDFS *elfGraph_getArgsDFS(const ElfGraph *graph){
	//There is an issue with const-qualifiers here.
	//We could consider ArgsDFS as a 'mutable' structure within the graph.
	return (ArgsDFS *) &(graph->dfs_args); 
}

//Visiting procedure for the DFS.
//Should only be called after a call to elfGraph_ArgsDFS_initialize().
//elfGraph_ArgsDFS_finalize() should be called to
//  clean resources used and to retrieve the produced vectors.
static
void elfGraph_DFS_visit(const ElfGraph *graph, int current){
	ElfListIt *iterator = elfList_getIterator(graph->array[current]);
	ArgsDFS *args = elfGraph_getArgsDFS(graph);
	Edge *edge;

	args->dfs_color[current] = 'g'; //gray
	args->dfs_time_vec[current] = (args->dfs_time)++;

	while(iterator != NULL){
		edge = iterator->key;
		if(args->dfs_color[edge->target] == 'w'){
			args->dfs_pred[edge->target] = current;
			elfGraph_DFS_visit(graph, edge->target);
		}
		iterator = iterator->next;
	}
	if(args->dfs_after_func)
		args->dfs_after_func(current, args->dfs_after_data);
	args->dfs_finish_vec[current] = (args->dfs_time)++;
	args->dfs_color[current] = 'b'; //black
}

//Initialize the ArgsDFS variables for use in DFS algorithms.
static inline
void elfGraph_ArgsDFS_initialize(const ElfGraph *graph){
	ArgsDFS *p;
	int i, n;

	p = elfGraph_getArgsDFS(graph);

	p->dfs_pred = malloc(sizeof(int) * graph->size);
	p->dfs_color = malloc(sizeof(char) * graph->size);
	p->dfs_time_vec = malloc(sizeof(int) * graph->size);
	p->dfs_finish_vec = malloc(sizeof(int) * graph->size);
	for(i = 0, n = graph->size; i < n; i++){
		p->dfs_pred[i] = i;
		p->dfs_color[i] = 'w'; //white
		p->dfs_time_vec[i] = -1;
	}
	p->dfs_time = 0;
}

//Sets ArgsDFS inner structures to a reset state, cleaning dangling pointers and such.
static inline
void elfGraph_ArgsDFS_reset(const ElfGraph *graph){
	ArgsDFS *p = elfGraph_getArgsDFS(graph);

	p->dfs_color = NULL;
	p->dfs_pred = NULL;
	p->dfs_time_vec = NULL;
	p->dfs_finish_vec = NULL;
	p->dfs_after_func = NULL;
	p->dfs_after_data = NULL;
}

//Deallocates resources used by DFS_visit, cleans the graph's ArgsDFS structure,
//  and returns the desired vectors.
//If 'pred' is not NULL, it receives the vector of predecessors.
//If 'time_vec' is not NULL, it receives the vector of time visited.
//If 'finish_vec' is not NULL, it receives the vector of time finished.
static inline
void elfGraph_ArgsDFS_finalize(const ElfGraph *graph, int **pred_vec, int **time_vec, int **finish_vec){
	ArgsDFS *args = elfGraph_getArgsDFS(graph);

	free(args->dfs_color);
	
	if(pred_vec) *pred_vec = args->dfs_pred;
	else free(args->dfs_pred);

	if(time_vec) *time_vec = args->dfs_time_vec;
	else free(args->dfs_time_vec);

	if(finish_vec) *finish_vec = args->dfs_finish_vec;
	else free(args->dfs_finish_vec);

	elfGraph_ArgsDFS_reset(graph);
}

// Documented in header file.
void elfGraph_DFS_registerAfterFunc(const ElfGraph *graph, void (*func)(int vert, void *data), void *data){
	if(!func) ELF_DIE("Received NULL pointer");

	ArgsDFS *p = elfGraph_getArgsDFS(graph);
	p->dfs_after_func = func;
	p->dfs_after_data = data;
}

// Documented in header file.
void elfGraph_DFS_src(const ElfGraph *graph, int src, int **pred_p, int **time_p, int **finish_p){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(src >= graph->size || src < 0) ELF_DIE("Invalid source vertix");

	elfGraph_ArgsDFS_initialize(graph);
	elfGraph_DFS_visit(graph, src);
	elfGraph_ArgsDFS_finalize(graph, pred_p, time_p, finish_p);
}

// Documented in header file.
void elfGraph_DFS_all(const ElfGraph *graph, int **pred_p, int **time_p, int **finish_p){
	if(!graph) ELF_DIE("Received NULL pointer");

	ArgsDFS *args = elfGraph_getArgsDFS(graph);
	elfGraph_ArgsDFS_initialize(graph);

	int i, n;
	for(i = 0, n = elfGraph_size(graph); i < n; i++){
		if( args->dfs_color[i] == 'w')
			elfGraph_DFS_visit(graph, i);
	}

	elfGraph_ArgsDFS_finalize(graph, pred_p, time_p, finish_p);
}


//Function for registering in the DFS algorithms.
static
void insert_vertix_into_list(int vert, void *data){
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

	int i, n, size, *finish, idx;
	ElfList **result;
	ElfGraph *trans;

	// Get finishing-time vector.
	elfGraph_DFS_all(graph, NULL, NULL, &finish);

	// Get indexes of vertixes, ordered from earliest finish-time to latest.
	n = elfGraph_size(graph);
	ElfVector *finish_vec = elfVector_new_fromArray(&finish, n);
	ElfVector *indexes = elfVector_qsort_descendWithIndexes(finish_vec);
	elfVector_destroy(&finish_vec);

	//vector indexes now contains indexes ordered from earliest finish-time to latest.

	// Transpose graph
	trans = elfGraph_transpose(graph);
	elfGraph_ArgsDFS_initialize(trans); // Initialize inner DFS arguments structure.
	ArgsDFS *args = elfGraph_getArgsDFS(trans); //Get pointer to the inner DFS arguments structure.
	result = NULL;
	size = 0;
	for(i = n-1; i >= 0; i--){
		idx = elfVector_get(indexes, i);
		
		//Find a white vertix
		if(args->dfs_color[idx] == 'w'){
			result = (ElfList **) realloc(result, sizeof(ElfList *) * (size+1));
			result[size] = elfList_new(ELF_POINTER_TO_INT_GREATER);

			// Visit that white vertix, adding all further white vertixes to this list.
			elfGraph_DFS_registerAfterFunc(trans, insert_vertix_into_list, result[size]);
			elfGraph_DFS_visit(trans, idx);
			size++;
		}
	}
	// Clean resources.
	elfGraph_ArgsDFS_finalize(trans, NULL, NULL, NULL);
	elfGraph_destroy(&trans);
	elfVector_destroy(&indexes);

	// NULL-terminate array of lists, where each list contains the vertexes of a component.
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
