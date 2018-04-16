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
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)
#define MAX(X,Y) X>Y?X:Y
#define MIN(X,Y) X<Y?X:Y

#define INVALID_WEIGHT INT_MIN

/*
 * Main Data Structed handled by this source file.
 */
typedef struct ElfGraphMatrix {
	int n;
	bool oriented;
	int **matrix;
} ElfGraphMatrix;


/* 
 * Static functions definitions
 */
static int **create_matrix_fill(int row, int col, int fill);
static void free_2d_matrix(int ***matrix, int row);


/*
 * Implementation of extern functions
 */

//Creates a graph with N vertexes, oriented or not.
ElfGraphMatrix *elfGraph_matrix_new(int N, bool oriented){
	if(N < 0) DIE("Number of vertixes cannot be negative");
	ElfGraphMatrix *new = (ElfGraphMatrix *) malloc(sizeof(ElfGraphMatrix));
	new->n = N;
	new->oriented = oriented;
	new->matrix = create_matrix_fill(N, N, INVALID_WEIGHT);
	return new;
}

//Deallocates all the memory used by 'graph', and sets its pointer to NULL.
//'graph' should be an ElfGraphMatrix* passed by reference (hence a double pointer).
void elfGraph_matrix_destroy(ElfGraphMatrix **graph){
	free_2d_matrix( &((*graph)->matrix), (*graph)->n );
	free(*graph);
	*graph = NULL;
}

//Adds to the graph an edge from 'src' to 'dest'.
//If the graph is not oriented, the inverse direction is also added.
void elfGraph_matrix_add_edge(ElfGraphMatrix *graph, int src, int dest, int weight){
	if(weight== INVALID_WEIGHT) DIE("Invalid weight value.");
	if(src < 0 || dest < 0 || src >= graph->n || dest >= graph->n) DIE("Invalid vertix indexes");
	graph->matrix[src][dest] = weight;
	if(!graph->oriented) graph->matrix[dest][src] = weight;
}

//Removes, if exists, the edge going from 'src' to 'dest'.
//If the graph is not oriented, the inverse is also removed.
void elfGraph_matrix_remove_edge(ElfGraphMatrix *graph, int src, int dest){
	if(src < 0 || dest < 0 || src >= graph->n || dest >= graph->n) DIE("Invalid vertix indexes");
	graph->matrix[src][dest] = INVALID_WEIGHT;
	if(!graph->oriented) graph->matrix[dest][src] = INVALID_WEIGHT;
}

//Prints the adjacency matrix of a graph, with 'failChar' representing absence of an edge,
//  'sep' being the delimeter between edges, and 'end' being the delimeter between lines in the matrix.
void elfGraph_matrix_print_matrix(const ElfGraphMatrix *graph, FILE *fp, char failChar, char sep, char end){
	int i, j, dimension = graph->n;
	for(i = 0; i < dimension; i++){
		for(j = 0; j < dimension; j++){
			int value = graph->matrix[i][j];
			if(value == INVALID_WEIGHT) fputc(failChar, fp);
			else fprintf(fp, "%d", value);

			if(j != dimension-1) fputc(sep, fp);
		}
		fputc(end, fp);
	}
}


//Prints indexes of vertices that are adjacent to 'subjectVertex'.
void elfGraph_matrix_print_adjacent_vertex(const ElfGraphMatrix *graph, int subjectVertex, FILE *fp){
	int i, counter, dimension = graph->n;
	if(subjectVertex >= graph->n || subjectVertex < 0) DIE("Invalid subjectVertex");
	const int *array = graph->matrix[subjectVertex];

	//'counter' will sinalize when we need to print a delimeter blank ' '.
	for(i = 0, counter = 0; i < dimension; i++)
		if(array[i] != INVALID_WEIGHT){
			fprintf(fp, counter != 0 ? " %d" : "%d", i);
			counter++;
		}
	fprintf(fp, "\n");
}

//Prints the lowest weight edge of the graph.
//If the graph is not oriented, the vertex indexes are printed in crescent order.
void elfGraph_matrix_print_edge_lowest_weight(const ElfGraphMatrix *graph, FILE *fp){
	int mini, minj, minVal = INT_MAX;
	const int **mat = (const int **) graph->matrix;

	int i, j, dimension;
	for(i = 0, dimension = graph->n; i < dimension; i++){
		for(j = 0; j < dimension; j++){
			int value = mat[i][j];
			if(value < minVal && value != INVALID_WEIGHT){
				minVal = value;
				mini = i;
				minj = j;
			}
		}
	}
	
	if(minVal == INT_MAX) fprintf(fp, "There are no edges.\n");
	else if(graph->oriented) fprintf(fp, "%d %d\n", mini, minj);
	else fprintf(fp, "%d %d\n", MIN(mini,minj), MAX(mini,minj));
}

//Prints the transposed adjacency matrix of a graph, with 'failChar' representing absence of an edge,
//  'sep' being the delimeter between edges, and 'end' being the delimeter between lines in the matrix.
void elfGraph_matrix_print_matrix_transposed(ElfGraphMatrix *graph, FILE *fp, char failChar, char sep, char end){
	if(!graph->oriented) return; //Ignoring the instruction, as told by the exercise.
	
	int i, j, dimension = graph->n;
	for(j = 0; j < dimension; j++){
		for(i = 0; i < dimension; i++){
			int value = graph->matrix[i][j];
			if(value == INVALID_WEIGHT) fputc(failChar, fp);
			else fprintf(fp, "%d", value);

			if(i != dimension-1) fputc(sep, fp);
		}
		fputc(end, fp);
	}
}


/*
 * Implementation of static functions
 */

//Creates a matrix filled with value 0.
static
int **create_matrix_fill(int row, int col, int fill){
	int i, j, **res;
	res = (int **) malloc(row * sizeof(int *));
	for(i = 0; i < row; i++){
		res[i] = (int *) malloc(col * sizeof(int));
		for(j = 0; j < col; j++) res[i][j] = fill;
	}
	return res;
}

//Frees a 2-dimensional matrix, and sets its pointer to NULL.
//'matrix' should be a int** passed by reference (hence the argument is int***).
static
void free_2d_matrix(int ***matrix, int row){
	int i;
	for(i = 0; i < row; i++) free( (*matrix)[i] );
	free( (*matrix) );
	*matrix = NULL;
}
