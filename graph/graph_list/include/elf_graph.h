#ifndef _ELF_GRAPH_LIST_H_
#define _ELF_GRAPH_LIST_H_

typedef struct _ElfGraph ElfGraph;

#include <stdio.h>
#include <stdbool.h>

#include <elf_list.h>

ElfGraph *elfGraph_new(int N, bool oriented);
void elfGraph_destroy(ElfGraph **graph_p);
int elfGraph_size(const ElfGraph *graph);
void elfGraph_addEdge(ElfGraph *graph, int src, int dest, int weight);
void elfGraph_removeEdge(ElfGraph *graph, int src, int dest);
void elfGraph_print(const ElfGraph *graph);
void elfGraph_printAdjacent(const ElfGraph *graph, int subjectVertex);
ElfGraph *elfGraph_transpose(const ElfGraph *graph);
void elfGraph_readFromFileVE(ElfGraph *graph, FILE *fp, int lim);
void elfGraph_readFromFileVEW(ElfGraph *graph, FILE *fp, int lim);

void elfGraph_DFS_src(const ElfGraph *graph, int src, int **pred_p, int **time_p, int **finish_p);
void elfGraph_DFS_all(const ElfGraph *graph, int **pred_p, int **time_p, int **finish_p);
void elfGraph_DFS_registerAfterFunc(const ElfGraph *graph, void (*func)(int vert, void *data), void *data);

ElfList **elfGraph_SCC(const ElfGraph *graph);
ElfGraph *elfGraph_MST_prim(const ElfGraph *graph);
ElfGraph *elfGraph_MST_kruskal(const ElfGraph *graph);

int *elfGraph_BFS(const ElfGraph *graph, int src, int **dist_p);

/* DOCUMENTATION

ElfGraph
	- Graph implemented with an adjacency list.
	- Can be oriented or not.
	- Size must be specified on instantiation.
	- Double edges are not allowed (cannot be MULTIGRAPH).
 
ElfGraph *elfGraph_new(int N, bool oriented);
	- Returns a graph with N vertixes, indexed from 0 to N-1.
	- Graph is oriented if 'oriented' is true.

void elfGraph_destroy(ElfGraph **graph_p);
	- Deallocates all the memory used by 'graph', and sets its pointer to NULL.
	- 'graph' should be an ElfGraph* passed by reference (hence a double pointer).

int elfGraph_size(const ElfGraph *graph);
	- Returns the amount of vertixes in the graph.

void elfGraph_addEdge(ElfGraph *graph, int src, int dest, int weight);
	- Adds to the graph an edge from 'src' to 'dest'.
	- If the graph is not oriented, the inverse direction is also added.

void elfGraph_removeEdge(ElfGraph *graph, int src, int dest);
	- Removes, if exists, the edge going from 'src' to 'dest'.
	- If the graph is not oriented, the inverse is also removed.

void elfGraph_print(const ElfGraph *graph);
	- Prints the adjacency list of a graph

void elfGraph_printAdjacent(const ElfGraph *graph, int subjectVertex);
	- Prints indexes of vertices that are adjacent to 'subjectVertex'.
 
ElfGraph *elfGraph_transpose(const ElfGraph *graph);
	- Returns the transposed graph of 'graph.
	- If 'graph' is not oriented, returns NULL.

void elfGraph_readFromFileVE(ElfGraph *graph, FILE *fp, int lim);
	- Reads a sequence of 'lim' non-weighted edges from a file.
	- Reads until EOF if 'lim' is -1.
	- Will read any blank-character-separated sequence of integers,
	    following the order: source vertix - destiny vertix
	- VE stands for vertix and edge

void elfGraph_readFromFileVEW(ElfGraph *graph, FILE *fp, int lim);
	- Same as above, but also reads weights.
	- VEW stands for vertix, edge and weight

void elfGraph_DFS_src(const ElfGraph *graph, int src, int **pred_p, int **time_p, int **finish_p);
	- Performs a DFS in the graph, finding the time/path from all vertexes to 'src'.
	- Args:
	-	graph: pointer to the graph in which to execute the DFS.
	-	src: number of vertex to start the DFS from.
	-	pred_p: if not NULL, receives the vector of predecessors.
	-	time_p: if not NULL, receives the vector of time visited.
	-	finish_p: if not NULL, receives the vector of time finished.

void elfGraph_DFS_all(const ElfGraph *graph, int **pred_p, int **time_p, int **finish_p);
	- Performs a DFS in the graph, until all vertixes are visited once.
	- Args:
		graph: graph in which to execute the DFS.
		pred_p: if not NULL, receives the vector of predecessors.
		time_p: if not NULL, receives the vector of time visited.
		finish_p: if not NULL, receives the vector of time finished.

void elfGraph_DFS_registerAfterFunc(const ElfGraph *graph, void (*func)(int vert, void *data), void *data);
	- Registers a function to be executed in each vertix after all adjacent vertexes have
		been visited.
	- Once registered, the function is executed only for ONE DFS, meaning multiple DFS requires
		multiple function registries.
	- For each vertex N, func will be called as func(N, data);
	- Args:
		func: function to register.
		data: data to be passed to 'func' when it's called.

ElfList **elfGraph_SCC(const ElfGraph *graph);
	- SCC standing for Strongly Connected Components
	- Finds all the strongly connected components of a graph.
	Return:
		A NULL-terminated array of lists, each of which contains a component.
		If graph is undirected, return NULL.

ElfGraph *elfGraph_MST_prim(const ElfGraph *graph);
	- Returns a Minimum Spanning Tree for the given graph.
	- The graph is supposed to be connected. This function does not check whether the graph is
	  connected or not. If it isn't, the behavior is undefined.
	- Given graph cannot be oriented.
	- Given graph can be weighted or not.
	- The Prim algorithm for finding MST is applied.

ElfGraph *elfGraph_MST_kruskal(const ElfGraph *graph);
	- Returns a Minimum Spanning Tree for the given graph.
	- The graph is supposed to be connected. This function does not check whether the graph is
	  connected or not. If it isn't, the behavior is undefined.
	- Given graph cannot be oriented.
	- Given graph can be weighted or not.
	- The Kruskal algorithm for finding MST is applied.
	- O(V*E), always worse than PRIM algorithm.

int *elfGraph_BFS(const ElfGraph *graph, int src, int **dist_p);
	- Performs a BFS in the graph, finding the distance/path from all vertexes to 'src'.
	- Args:
	-   dist_p: If 'dist_p' is not NULL, store in it 
	-	        the vector of shortest distances to each vertex.
	-	        - dist[i] == -1, vertex could not be reached from 'src'.
	-	        - dist[i] != -1, shortest distance from 'src' to vertex i.
	- Returns:
	-   Vector of predecessors generated by the BFS.
	-	pred[i] == i means the vertix could not be reached,
	-	  with the exception of the source vertix, obviously
	-	pred[i] != i means vertix i could be visited and it's predecessor is pred[i]
	-	The predecessor vector must be freed by the client of the function.

*/

#endif
