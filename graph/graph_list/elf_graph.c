#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <elf_graph.h>

#define DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)
#define MAX(X,Y) X>Y?X:Y
#define MIN(X,Y) X<Y?X:Y

typedef struct _Node {
	int adj;	//Vertix this node 'points' to.
	int weight;	//Weight of the edge.
	struct _Node *next;	//List will be ordered, with no duplicates.
} Node;

typedef struct _ElfGraph {
	int n;
	bool oriented;
	Node **array; //Array of lists.
} ElfGraph;


/* 
 * Static functions definitions
 */
static void elf_node_list_insert(Node **current, Node *node);
static bool elf_node_list_remove(Node **current, int adj);


//Creates a graph with N vertexes, oriented or not.
ElfGraph *elf_graph_new(int N, bool oriented){
	if(N < 0) DIE("Number of vertixes cannot be negative");
	ElfGraph *new = malloc(sizeof(ElfGraph));
	new->n = N;
	new->oriented = oriented;
	new->array = calloc(sizeof(Node *), N); //NULL pointers.
	return new;
}

//Deallocates all the memory used by 'graph', and sets its pointer to NULL.
//'graph' should be an ElfGraph* passed by reference (hence a double pointer).
void elf_graph_destroy(ElfGraph **graph){
	int i, n;
	Node **array_copy = (*graph)->array;

	//Frees the nodes on the lists.
	for(i = 0, n = (*graph)->n; i < n; i++){
		Node *aux, *ref = array_copy[i];
		while(ref != NULL){
			aux = ref->next;
			free(ref);
			ref = aux;
		}
	}

	free((*graph)->array);
	free(*graph);
	*graph = NULL;
}

//Adds to the graph an edge from 'src' to 'dest'.
//If the graph is not oriented, the inverse direction is also added.
void elf_graph_add_edge(ElfGraph *graph, int src, int dest, int weight){
	if(src < 0 || dest < 0 || src >= graph->n || dest >= graph->n) DIE("Invalid vertix indexes");
	
	Node *node = malloc(sizeof(Node));
	node->adj = dest;
	node->weight = weight;
	elf_node_list_insert(&graph->array[src], node);

	if(!graph->oriented){
		node = malloc(sizeof(Node));
		node->adj = src;
		node->weight = weight;
		elf_node_list_insert(&graph->array[dest], node);
	}
}

//Removes, if exists, the edge going from 'src' to 'dest'.
//If the graph is not oriented, the inverse is also removed.
void elf_graph_remove_edge(ElfGraph *graph, int src, int dest){
	if(src < 0 || dest < 0 || src >= graph->n || dest >= graph->n) DIE("Invalid vertix indexes");

	elf_node_list_remove(&graph->array[src], dest);
	if(!graph->oriented) elf_node_list_remove(&graph->array[dest], src);
}

//Prints the adjacency list of a graph
void elf_graph_print_list(const ElfGraph *graph, FILE *fp){
	int i, dimension = graph->n;
	for(i = 0; i < dimension; i++){
		Node *node = graph->array[i];
		fprintf(fp, "%d.", i);
		while(node != NULL){
			fprintf(fp, " %d(%d)", node->adj, node->weight);
			node = node->next;
		}
		fprintf(fp, "\n");
	}
}

//Prints indexes of vertices that are adjacent to 'subjectVertex'.
void elf_graph_print_adjacent_vertex(const ElfGraph *graph, int subjectVertex, FILE *fp){
	if(subjectVertex >= graph->n || subjectVertex < 0) DIE("Invalid subjectVertex");
	Node *node = graph->array[subjectVertex];

	int counter = 0; //Counter will control when to print a blank space.
	while(node != NULL){
		fprintf(fp, counter == 0 ? "%d" : " %d", node->adj);
		node = node->next;
		counter++;
	}
	fprintf(fp, "\n");
}

//Prints the lowest weight edge of the graph.
//If the graph is not oriented, the vertex indexes are printed in crescent order.
void elf_graph_print_edge_lowest_weight(const ElfGraph *graph, FILE *fp){
	int mini, minj, minVal = INT_MAX;
	
	int i, dimension;
	for(i = 0, dimension=graph->n; i < dimension; i++){
		Node *node = graph->array[i];
		while(node != NULL){
			if(node->weight < minVal){
				mini = i;
				minj = node->adj;
				minVal = node->weight;
			}
			node = node->next;
		}
	}

	if(minVal == INT_MAX) fprintf(fp, "There are no edges.\n");
	else if(graph->oriented) fprintf(fp, "%d %d\n", mini, minj);
	else fprintf(fp, "%d %d\n", MIN(mini,minj), MAX(mini,minj));
}

//Prints the transposed adjacency list of a graph
void elf_graph_print_matrix_transposed(ElfGraph *graph, FILE *fp){
	int current_col, dimension = graph->n;
	Node **node_records;

	if(!graph->oriented) return; //Ignoring as instructed to.

	//Copies the content of graph->array into node_records.
	node_records = malloc(sizeof(Node *) * dimension);
	memcpy(node_records, graph->array, sizeof(Node *) * dimension);

	//Will run through the records to find which nodes point to the node 0. Then print them.
	//Those that pointer to node 0 will be incremented (receive value of next node).
	//Then move on to check which nodes point to node 1... and so on.
	for(current_col = 0; current_col < dimension; current_col++){
		int i;
		fprintf(fp, "%d.", current_col);
		for(i = 0; i < dimension; i++){
			if(node_records[i] == NULL) continue;
			else if(node_records[i]->adj > current_col) continue;
			else {
				fprintf(fp, " %d(%d)", i, node_records[i]->weight);
				node_records[i] = node_records[i]->next;
			}
		}
		fprintf(fp, "\n");
	}

	free(node_records);
}

/*
 * Implementation of static functions
 */

//Inserts node into an ordered list of Nodes.
//If there is already a node with same 'adj' value, the previous node will be replaced.
static
void elf_node_list_insert(Node **current, Node *node){
	
	//Finds the point at which to insert 'node'.
	//Will stop once 'current' is NULL or 'current' is higher than 'node'.
	while(*current != NULL && node->adj > (*current)->adj)
		current = &(*current)->next;
	
	if(*current != NULL && node->adj == (*current)->adj){
		//Replaces 'current' for 'node'.
		node->next = (*current)->next;
		free(*current);
		*current = node;
	} else {
		//Inserts 'node' before 'current'
		node->next = *current;
		*current = node;
	}
}

//Removes a node from an ordered list of Nodes;
//If the node doesn't exist, return 'false'; 'true' otherwise.
static
bool elf_node_list_remove(Node **current, int adj){
	Node *aux;
	
	//Test the current node.
	if(*current == NULL) return false;
	if((*current)->adj == adj){
		aux = *current;
		*current = (*current)->next;
		free(aux);
		return true;
	}

	//Traverse the list to find the node with 'adj'.
	while((*current)->next != NULL && adj != (*current)->next->adj)
		current = &(*current)->next;

	if((*current)->next == NULL) return false;
	else {
		aux = (*current)->next;
		(*current)->next = aux->next;
		free(aux);
		return true;
	}
}
