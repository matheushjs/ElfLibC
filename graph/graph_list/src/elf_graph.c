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
#include <limits.h>

#include <elf_graph.h>
#include <elf_list.h>
#include <elf_queue.h>
#include <elf_int_vector.h>
#include <elf_pqueue.h>
#include <elf_int_uf_set.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)
#define MAX(X,Y) X>Y?X:Y
#define MIN(X,Y) X<Y?X:Y

typedef struct _ArgsDFS {
	int  *dfs_pred;        //predecessor vector
	char *dfs_color;       //color vector
	int  *dfs_time_vec;    //visit time vector
	int  *dfs_finish_vec;  //finishing time vector
	int  dfs_time;         //records current "time"
	void (*dfs_after_func) (int vert, void *data); // function to execute on vertex finishing
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

/* Some needed static functions */
static
bool elf_edge_greater(void *a, void *b){
	return ((Edge *) a)->target > ((Edge *) b)->target ? true : false;
}
static
bool elf_edge_equal(void *a, void *b){
	return ((Edge *) a)->target == ((Edge *) b)->target ? true : false;
}
static
void elf_edge_print(void *a){
	printf("%d(%d) ", ((Edge *) a)->target, ((Edge *) a)->weight);
}
/**/

/* Some needed declarations (used before implemented) */
static inline void elfGraph_ArgsDFS_reset(const ElfGraph *graph);
/**/

// Documented in header file.
ElfGraph *elfGraph_new(int N, bool oriented){
	if(N < 0) ELF_DIE("Number of vertexes cannot be negative");
	ElfGraph *new = malloc(sizeof(ElfGraph));
	new->size = N;
	new->oriented = oriented;
	new->array = calloc(sizeof(ElfList *), N);
	for(N--; N >= 0; N--)
		new->array[N] = elfList_new_withEqual(
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
	if(src < 0 || dest < 0 || src >= graph->size || dest >= graph->size) ELF_DIE("Invalid vertex indexes");
	
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
	if(src < 0 || dest < 0 || src >= graph->size || dest >= graph->size) ELF_DIE("Invalid vertex indexes");
	
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
			ELF_DIE("Invalid vertexes in file.");
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
			ELF_DIE("Invalid vertexes in file.");
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
	if(src >= graph->size || src < 0) ELF_DIE("Invalid source vertex");

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
void insert_vertex_into_list(int vert, void *data){
	elfList_insert(data, ELF_INT_TO_POINTER(vert));
}

// Documented in header file.
ElfList **elfGraph_SCC(const ElfGraph *graph){
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

	// Get indexes of vertexes, ordered from earliest finish-time to latest.
	n = elfGraph_size(graph);
	ElfIntVector *finish_vec = elfIntVector_new_fromArray(&finish, n);
	ElfIntVector *indexes = elfIntVector_qsort_descendWithIndexes(finish_vec);

	elfIntVector_destroy(&finish_vec);

	//vector indexes now contains indexes ordered from earliest finish-time to latest.

	// Transpose graph
	trans = elfGraph_transpose(graph);
	elfGraph_ArgsDFS_initialize(trans); // Initialize inner DFS arguments structure.
	ArgsDFS *args = elfGraph_getArgsDFS(trans); //Get pointer to the inner DFS arguments structure.
	result = NULL;
	size = 0;
	for(i = 0; i < n; i++){
		idx = elfIntVector_get(indexes, i);
		
		//Find a white vertex
		if(args->dfs_color[idx] == 'w'){
			result = (ElfList **) realloc(result, sizeof(ElfList *) * (size+1));
			result[size] = elfList_new(ELF_POINTER_TO_INT_GREATER);

			// Visit that white vertex, adding all further white vertexes to this list.
			elfGraph_DFS_registerAfterFunc(trans, insert_vertex_into_list, result[size]);
			elfGraph_DFS_visit(trans, idx);
			size++;
		}
	}
	// Clean resources.
	elfGraph_ArgsDFS_finalize(trans, NULL, NULL, NULL);
	elfGraph_destroy(&trans);
	elfIntVector_destroy(&indexes);

	// NULL-terminate array of lists, where each list contains the vertexes of a component.
	result = (ElfList **) realloc(result, sizeof(ElfList *) * (size+1));
	result[size] = NULL;

	return result;
}

/* Function for getting the Minimum Spanning Tree of a given Graph.
 * PRIM algorithm is used.
 * If graph is not *connected*, returns NULL
 * Graph can be oriented or not.
 * Graph can be weighted or not. Vertexes on graphs that are not weighted receive same weight 0.
 * TODO: Function for finding MST from a given node only.
 * TODO: Function for finding all connected components of a graph (Similar to SCC, but that also works with non-digraphs)
 * TODO: Function for finding MST for each components
 *
 */
ElfGraph *elfGraph_MST_prim(const ElfGraph *graph){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(graph->oriented){
		fprintf(stderr, "Cannot get MST of a directed graph.\n");
		return NULL;
	}

	ElfGraph *MST = elfGraph_new(graph->size, graph->oriented);
	if(graph->size <= 1) return MST; //Ensures has at least 2 vertexes.

	// We have the limitation that our priority queue only stores a pair of integers (value, priority)
	// We will use as priority the wright of the edge.
	// As value, we will use a number that can be used to retrieve the Edge from another data structure.
	ElfPQueue *pqueue = elfPQueue_new_minFirst();
	
	// For now, there will be 2 Vectors, for storing 1) target and 2) source vertexes.
	// This solution is very fast as long as no edges are removed during the algorithm,
	//   which means high memory usage.
	// TODO: Change vectors for one of these:
	//   1) A hashTable of linked lists (Potentially veeeery fast. Find a good hashing function).
	//   2) A balanced search tree (log(n) insertion/removal, huge overheads, difficult implementation).
	ElfIntVector *targets = elfIntVector_new();
	ElfIntVector *sources = elfIntVector_new();

	// Array for tracking which vertexes are already in the MST.
	char *visited = calloc(sizeof(char), graph->size);

	// Add an edge to the initial vertex to the queue
	elfIntVector_pushBack(targets, 0);  //MUST BE 0
	elfIntVector_pushBack(sources, -1); //MUST BE NEGATIVE
	elfPQueue_push(pqueue, 0, 0);

	int i, n;
	int idx, wei, tgt, src;
	const ElfList *list;
	const Edge *edge;
	while(elfPQueue_size(pqueue) != 0){
		idx = elfPQueue_pop(pqueue, &wei);
		tgt = elfIntVector_get(targets, idx);
		src = elfIntVector_get(sources, idx);
		// 'src', 'tgt', 'wei' are information about the Edge popped from the queue.

		// If target is already in the MST, continue
		if(visited[tgt] != 0) continue;
		visited[tgt] = 1; // Mark as visited

		// Add edge to the MST
		if(src >= 0){ //If negative, it's the first vertex
			elfGraph_addEdge(MST, src, tgt, wei);
		}

		// Now analyzing edges parting from the target vertex.
		int current = tgt;

		// Get adjacency list of vertex
		list = graph->array[current];
		n = elfList_size(list);
		for(i = 0; i < n; i++){
			edge = elfList_get(list, i);

			// If target of edge has not been visted, add to the priority queue.
			if(!visited[edge->target]){
				idx = elfIntVector_size(targets);
				elfIntVector_pushBack(targets, edge->target);
				elfIntVector_pushBack(sources, current);
				elfPQueue_push(pqueue, idx, edge->weight);
			}
		}
	}

	//Clean Resources
	elfIntVector_destroy(&targets);
	elfIntVector_destroy(&sources);
	elfPQueue_destroy(&pqueue);
	free(visited);

	return MST;
}

// Documented in header file.
ElfGraph *elfGraph_MST_kruskal(const ElfGraph *graph){
	// Initialize disjoint set
	ElfIntUFSet *set = elfIntUFSet_new(graph->size);

	// Initialize 'targets', 'sources', 'weights' vector
	// TODO: Use a vector that permits construction with capacity.
	ElfIntVector *targets = elfIntVector_new();
	ElfIntVector *sources = elfIntVector_new();
	ElfIntVector *weights = elfIntVector_new();

	// Initialize a 'MST' graph
	ElfGraph *MST = elfGraph_new(graph->size, graph->oriented);

	// Add all edges to the vectors
	int i, m, j, n;
	for(i = 0, m = graph->size; i < m; i++){
		ElfList *list = graph->array[i];
		for(j = 0, n = elfList_size(list); j < n; j++){
			Edge *e = elfList_get(list, j); // O(1) because of optimizations in sequential get()'s.
			
			elfIntVector_pushBack(sources, i);
			elfIntVector_pushBack(targets, e->target);
			elfIntVector_pushBack(weights, e->weight);
		}
	}

	// Get sorted indexes of edges, with increasing weight
	ElfIntVector *indexes = elfIntVector_qsort_ascendWithIndexes(weights);

	// while pqueue.not_empty:
	//   k = pqueue.pop
	//
	//   if set(k.target) != set(k.source):
	//     MST.add_edge(k.source, k.target, k.weight)
	//     union(k.target, k.source)
	int idx, src, tgt, wei;
	for(i = 0, m = elfIntVector_size(indexes); i < m; i++){
		idx = elfIntVector_get(indexes, i);
		wei = elfIntVector_get(weights, i); // Because it was sorted too
		src = elfIntVector_get(sources, idx);
		tgt = elfIntVector_get(targets, idx);
		
		if(elfIntUFSet_find(set, src) != elfIntUFSet_find(set, tgt)){
			elfGraph_addEdge(MST, src, tgt, wei);
			elfIntUFSet_union(set, src, tgt);
		}
	}
	
	//Clean used resources
	elfIntVector_destroy(&targets);
	elfIntVector_destroy(&sources);
	elfIntVector_destroy(&weights);
	elfIntVector_destroy(&indexes);
	elfIntUFSet_destroy(&set);

	return MST;
}

// Documented in header file.
// TODO: Implement dijkstra from 'src' to all other vertexes
int elfGraph_dijkstra_withTarget(const ElfGraph *graph, int src, int tgt, int **pred_p){
	int *dist, *pred;
	bool *visited;

	//Allocate arrays
	dist = malloc(sizeof(int) * graph->size);
	pred = malloc(sizeof(int) * graph->size);
	visited = malloc(sizeof(bool) * graph->size);

	//Initialize arrays
	int i, n;
	for(i = 0, n = graph->size; i < n; i++){
		dist[i] = INT_MAX;
		pred[i] = i;
		visited[i] = false;
	}

	//Initialize PQueue
	ElfPQueue *pqueue = elfPQueue_new_minFirst();

	//Initialize source vertex
	dist[src] = 0;
	elfPQueue_push(pqueue, src, 0);

	const ElfList *list;
	const Edge *edge;
	int vertex, tentative;
	while(elfPQueue_size(pqueue) != 0){
		vertex = elfPQueue_pop(pqueue, NULL);
		
		//Don't visit same vertex again
		if(visited[vertex]) continue;
		visited[vertex] = true;
		
		//Reached target vertex, break loop
		if(vertex == tgt) break;

		list = graph->array[vertex];
		//ElfList is optimized for O(1) sequential get()'s
		for(i = 0, n = elfList_size(list); i < n; i++){
			edge = elfList_get(list, i);
			tentative = dist[vertex] + edge->weight;
			
			//Relax edge
			if(tentative < dist[edge->target]){
				dist[edge->target] = tentative;
				pred[edge->target] = vertex;

				//TODO: Use a priority queue that permits updating priorities
				elfPQueue_push(pqueue, edge->target, dist[edge->target]);
			}
		}
	}

	int distance = dist[tgt]; //Might be INT_MAX
	free(dist);
	free(visited);
	elfPQueue_destroy(&pqueue);

	if(pred_p)
		*pred_p = pred;
	else free(pred);

	return distance;
}

// Documented in header file.
int *elfGraph_BFS(const ElfGraph *graph, int src, int **dist_p){
	if(!graph) ELF_DIE("Received NULL pointer");
	if(src >= graph->size || src < 0) ELF_DIE("Invalid source vertex");

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
